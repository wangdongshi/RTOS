#include <string.h>
#include "stm32f746_register.h"

typedef unsigned int			uint32_t;

#define DELAY_COUNT				(10000000) // delay should be smaller than 1 second
#define BUF_SIZE				(1000)

#define PLLM					((uint32_t)( 25 <<  0))
#define PLLN					((uint32_t)(432 <<  6))
#define PLLP					((uint32_t)(  0 << 16)) // pay attention to this value!!
#define PLLSRC					((uint32_t)(  1 << 22))
#define PLLQ					((uint32_t)(  9 << 24))

#define USART1_IRQn				(37)

#define GPIO_MODER_IN			(0x0)
#define GPIO_MODER_OUT			(0x1)
#define GPIO_MODER_MULTI		(0x2)
#define GPIO_MODER_SIM			(0x3)
#define GPIO_OTYPER_PP			(0x0)
#define GPIO_OTYPER_OD			(0x1)
#define GPIO_OSPEEDR_LOW		(0x0)
#define GPIO_OSPEEDR_MID		(0x1)
#define GPIO_OSPEEDR_HIGH		(0x2)
#define GPIO_OSPEEDR_FULL		(0x3)
#define GPIO_PUPDR_NONE			(0x0)
#define GPIO_PUPDR_UP			(0x1)
#define GPIO_PUPDR_DOWN			(0x2)
#define GPIO_PUPDR_RESERVE		(0x3)
#define GPIO_AFR_AF7			(0x7)

void initFPU(void);
void initSystemClock(void);
void initNVICPriorityGroup(void);
void initSystick(void);
void initLED1(void);
void initUSART1(void);
void enableSystickInt(void);
void enableUSART1Int(void);

void delay(const unsigned int count);
void executeCmd(const char* cmd);
void toggleLED1(void);
void usart1SendChar(const char character);
void usart1SendBuffer(const char* message);

static void initGPIOA9(void);
static void initGPIOB7(void);
static void initGPIOI1(void);

static uint32_t count = 1;
static char character = 0;
static char buffer[BUF_SIZE];

// The following function is interrupt handler
void SysTick_Handler(void)
{
	if (count % 501 == 0) {
		toggleLED1();
		count = 1;
	} else {
		count++;
	}

}

void USART1_IRQHandler(void)
{
	if((*((uint32_t *)USART1_ISR) & 0x00000020) == 0x00000020) { // is RXNE set?
		character = (char)(*((uint32_t *)USART1_RDR) & 0x000000FF);
		usart1SendChar(character); // echo received character
	}
}

// main function for LED test
int main(void)
{
	char banner[] = "Welcome to STM32F746G-DISCO !\r#";

	// initialize board
	initFPU();
	initNVICPriorityGroup();
	initSystemClock();
	initLED1();
	initUSART1();
	initSystick();

	// print banner by USART1
	usart1SendBuffer(banner);

	// command processing
	uint32_t index = 0;
	memset(buffer, 0x00, BUF_SIZE);
	while(1) {
		if (character != 0) { // character has been received
			buffer[index++] = character;
			if (character == '\r') {
				char cmd[256] = "";
				buffer[--index] = '\n';
				strcpy(cmd, buffer); // copy command text
				memset(buffer, 0x00, BUF_SIZE);
				executeCmd(cmd);
				usart1SendChar('#'); // print next prompt
				index = 0;
			}
			character = 0;
			/*
			// flick LED1
			toggleLED1();
			delay(1000000);
			toggleLED1();
			*/
		}
	}

	return 0;
}

void executeCmd(const char* cmd)
{
	uint32_t len = strlen(cmd) - 1;
	if (strncmp(cmd, "help", len) == 0) {
		usart1SendBuffer("This is help command.\r\r");
	} else {
		usart1SendBuffer("This is a wrong command.\r\r");
	}
}

// The following initialization process is write for STM32F746G
// FPU initialization
void initFPU(void)
{
	*((uint32_t *)SCR_CPACR) |= (uint32_t)0x00F00000;
}

// System clock initialization
void initSystemClock(void)
{
	// 1. Set HSE and reset RCC_CIR
	*((uint32_t *)RCC_CR) |= (uint32_t)0x00040000; // set HSEBYP bit
	*((uint32_t *)RCC_CR) |= (uint32_t)0x00010000; // set HSEON bit
	while((*((uint32_t *)RCC_CR) & 0x00020000) == 0); // wait HSERDY
	*((uint32_t *)RCC_CIR) = (uint32_t)0x00000000; // disable all RCC interrupts

	// 2. Set FLASH latency
	*((uint32_t *)FLASH_ACR) |= 0x00000007; // must be set in 216MHz
	while((*((uint32_t *)FLASH_ACR) & 0x0000000F) != 7); // wait latency set to 7
	// *((uint32_t *)FLASH_ACR) |= 0x00000300; // set ARTEN, PRFTEN
	//*((uint32_t *)PWR_CR1) |= 0x0000C000; // PWR_VOS default value has set to 0x11 in reset

	// 3. Enable PWR clock
	*((uint32_t *)RCC_APB1ENR) |= 0x10000000;

	// 4. Activation OverDrive Mode
	*((uint32_t *)PWR_CR1) |= 0x00010000; // set ODEN
	while((*((uint32_t *)PWR_CSR1) & 0x00010000) == 0); // wait ODRDY

	// 5. Activation OverDrive Switching
	*((uint32_t *)PWR_CR1) |= 0x00020000; // set ODSWEN
	while((*((uint32_t *)PWR_CSR1) & 0x00020000) == 0); // wait for ODSWRDY

	// 6. Main PLL configuration and activation
	*((uint32_t *)RCC_PLLCFGR) = PLLM | PLLN | PLLP | PLLSRC | PLLQ; // reset PLLCFGR register
	*((uint32_t *)RCC_CR) |= (uint32_t)0x01000000; // set PLLON
	while((*((uint32_t *)RCC_CR) & 0x02000000) == 0); // wait for PLLRDY

	// 7. System clock activation on the main PLL
	*((uint32_t *)RCC_CFGR) |= (uint32_t)0x00000000; // set HPRE  (AHB  pre-scaler) to 1 (216 MHz)
	*((uint32_t *)RCC_CFGR) |= (uint32_t)0x00001400; // set PPRE1 (APB1 pre-scaler) to 4 (54  MHz)
	*((uint32_t *)RCC_CFGR) |= (uint32_t)0x00008000; // set PPRE2 (APB2 pre-scaler) to 2 (108 MHz)
	while((*((uint32_t *)RCC_CFGR) & 0x0000FC00) != 0x00009400); // Before switching System clock to PLL, it must wait pre-scale value set to CFGR.
	*((uint32_t *)RCC_CFGR) |= (uint32_t)0x00000002; // set SW to PLL
	while((*((uint32_t *)RCC_CFGR) & 0x0000000C) != 0x00000008); // wait for system clock set to PLL
}

// set global NVIC priority group
void initNVICPriorityGroup(void)
{
	// "|=" can not be used here, because VECTKEY write key is "0x05FA" and "|=" will change it
	*((uint32_t *)SCR_AIRCR) = (uint32_t)0x05FA0500; // PRIGROUP = 0b011 (main priority = 16, sub priority = 16)
}

// enable USART1 interrupt
void enableUSART1Int(void)
{
	// set USART1 interrupt priority
	*((uint32_t *)NVIC_IPR9) |= (uint32_t)(0xFE << 8); // main priority = None, sub priority = 127

	// enable USART1 global interrupt
	*((uint32_t *)NVIC_ISER1) |= (uint32_t)0x00000020; // enable USART1 global interrupt
}

// enable Systick interrupt
void enableSystickInt(void)
{
	// set Systick interrupt priority
	*((uint32_t *)SCR_SHPR3) |= (uint32_t)(0xFF << 24); // main priority = None, sub priority = 128

	// enable Systick global interrupt
	*((uint32_t *)SYST_CSR) |= (uint32_t)0x00000002; // enable Systick interrupt
}

// System tick initialization
void initSystick(void)
{
	// enable Systick global interrupt
	enableSystickInt();

	// set RELOAD value (216000000 / 1000 = 216000)
	*((uint32_t *)SYST_RVR) = (uint32_t)(0x00034BC0 - 1); // 1ms interrupt
	while((*((uint32_t *)SYST_RVR) & 0x00FFFFFF) != (uint32_t)(0x00034BC0 - 1)); // wait for RELOAD value

	// set current value
	*((uint32_t *)SYST_CVR) = (uint32_t)0x00000000;

	// set CLKSOURCE, ENABLE
	*((uint32_t *)SYST_CSR) |= (uint32_t)0x00000005;
	while((*((uint32_t *)SYST_CSR) & 0x00000007) != (uint32_t)(0x00000007)); // wait for enable
}

// LED1 initialization
void initLED1(void)
{
	initGPIOI1();
}

// USART1 initialization
void initUSART1(void)
{
	// initialize USART1 TX/RX pin
	initGPIOA9(); // TX
	initGPIOB7(); // RX

	// enable APB2 USART1 RCC
	*((uint32_t *)RCC_APB2ENR) |= 0x00000010;
	while((*((uint32_t *)RCC_APB2ENR) & 0x00000010) == 0); // wait APB2 USART1 RCC set

	// set USART1 parameter
	//*((uint32_t *)USART1_BRR) |= 0x00002BF2; // baud rate = 9600(fCK=108MHz, CR1/OVER8=0, 0x2BF2 = 108000000/9600)
	*((uint32_t *)USART1_BRR) |= 0x000003AA; // baud rate = 115200(fCK=108MHz, CR1/OVER8=0, 0x03AA = 108000000/115200)
	*((uint32_t *)USART1_CR1) |= 0x00000000; // data bits = 8
	*((uint32_t *)USART1_CR2) |= 0x00000000; // stop bits = 1
	*((uint32_t *)USART1_CR1) |= 0x00000000; // parity = none (odd:0x00000600, even:0x00000400)
	*((uint32_t *)USART1_CR3) |= 0x00000000; // disable CTS and RTS

	// enable USART1 global interrupt
	enableUSART1Int();

	// enable RX interrupt
	*((uint32_t *)USART1_CR1) |= 0x00000020; // set RXNEIE

	// enable USART1
	*((uint32_t *)USART1_CR1) |= 0x00000001; // enable USART1

	// enable RE
	*((uint32_t *)USART1_CR1) |= 0x00000004; // enable RE
}

// delay time is depend on system clock
void delay(const unsigned int count)
{
	for(uint32_t i = 0; i < count; i++);
}

void toggleLED1(void)
{
	uint32_t pin = 1;
	uint32_t temp, data;

	temp = *((uint32_t *)GPIOI_ODR);
	data = !((temp & (0x1 << pin)) >> pin);
	temp &= ~(0x1 << pin);
	temp |= (data << pin);
	*((uint32_t *)GPIOI_ODR) = temp;
}

void usart1SendChar(const char character)
{
	*((uint32_t *)USART1_CR1) |= 0x00000008; // enable TE
	*((uint32_t *)USART1_TDR) = (0x000000FF & character); // set character to TX buffer
	while((*((uint32_t *)USART1_ISR) & 0x00000040) == 0); // wait TC set
	*((uint32_t *)USART1_CR1) &= ~0x00000008; // disable TE
}

void usart1SendBuffer(const char* message)
{
	*((uint32_t *)USART1_CR1) |= 0x00000008; // enable TE
	for(uint32_t i = 0; i < strlen(message); i++) {
		*((uint32_t *)USART1_TDR) = (0x000000FF & message[i]); // set character to TX buffer
		while((*((uint32_t *)USART1_ISR) & 0x00000080) == 0); // wait TXE set
	}
	while((*((uint32_t *)USART1_ISR) & 0x00000040) == 0); // wait TC set
	*((uint32_t *)USART1_CR1) &= ~0x00000008; // disable TE
}

// USART TX pin initialization
static void initGPIOA9(void)
{
	uint32_t pin  = 9;
	uint32_t temp = 0x00000000;

	// enable AHB1 GPIOA RCC
	*((uint32_t *)RCC_AHB1ENR) |= 0x00000001;
	while((*((uint32_t *)RCC_AHB1ENR) & 0x00000001) == 0); // wait AHB1 GPIOA RCC set

	// set GPIO MODER register
	temp = *((uint32_t *)GPIOA_MODER);
	temp &= ~(0x3 << (pin * 2)); // clear bits
	temp |= (GPIO_MODER_MULTI << (pin * 2)); // set bits
	*((uint32_t *)GPIOA_MODER) = temp;

	// set GPIO OTYPER register
	temp = *((uint32_t *)GPIOA_OTYPER);
	temp &= ~(0x1 << pin);
	temp |= (GPIO_OTYPER_PP << pin);
	*((uint32_t *)GPIOA_OTYPER) = temp;

	// set GPIO OSPEEDR register
	temp = *((uint32_t *)GPIOA_OSPEEDR);
	temp &= ~(0x3 << (pin * 2));
	temp |= (GPIO_OSPEEDR_FULL << (pin * 2));
	*((uint32_t *)GPIOA_OSPEEDR) = temp;

	// set GPIO PUPDR register
	temp = *((uint32_t *)GPIOA_PUPDR);
	temp &= ~(0x3 << (pin * 2));
	temp |= (GPIO_PUPDR_UP << (pin * 2));
	*((uint32_t *)GPIOA_PUPDR) = temp;

	// set GPIO AFRH register
	temp = *((uint32_t *)GPIOA_AFRH);
	temp &= ~(0x7 << ((pin - 8) * 4));
	temp |= (GPIO_AFR_AF7 << ((pin - 8) * 4));
	*((uint32_t *)GPIOA_AFRH) = temp;
}

// USART RX pin initialization
static void initGPIOB7(void)
{
	uint32_t pin  = 7;
	uint32_t temp = 0x00000000;

	// enable AHB1 GPIOB RCC
	*((uint32_t *)RCC_AHB1ENR) |= 0x00000002;
	while((*((uint32_t *)RCC_AHB1ENR) & 0x00000002) == 0); // wait AHB1 GPIOB RCC set

	// set GPIO MODER register
	temp = *((uint32_t *)GPIOB_MODER);
	temp &= ~(0x3 << (pin * 2)); // clear bits
	temp |= (GPIO_MODER_MULTI << (pin * 2)); // set bits
	*((uint32_t *)GPIOB_MODER) = temp;

	// set GPIO OTYPER register
	temp = *((uint32_t *)GPIOB_OTYPER);
	temp &= ~(0x1 << pin);
	temp |= (GPIO_OTYPER_PP << pin);
	*((uint32_t *)GPIOB_OTYPER) = temp;

	// set GPIO AFRL register
	temp = *((uint32_t *)GPIOB_AFRL);
	temp &= ~(0x7 << (pin * 4));
	temp |= (GPIO_AFR_AF7 << (pin * 4));
	*((uint32_t *)GPIOB_AFRL) = temp;

	// RX line need not to set OSPEEDR and PUPDR
}

// LED1 pin initialization
static void initGPIOI1(void)
{
	uint32_t pin  = 1;
	uint32_t temp = 0x00000000;

	// enable AHB1 GPIOI RCC
	*((uint32_t *)RCC_AHB1ENR) |= 0x00000100;
	while((*((uint32_t *)RCC_AHB1ENR) & 0x00000100) == 0); // wait AHB1 GPIOI RCC set

	// set GPIO MODER register
	temp = *((uint32_t *)GPIOI_MODER);
	temp &= ~(0x3 << (pin * 2)); // clear bits
	temp |= (GPIO_MODER_OUT << (pin * 2)); // set bits
	*((uint32_t *)GPIOI_MODER) = temp;

	// set GPIO OTYPER register
	temp = *((uint32_t *)GPIOI_OTYPER);
	temp &= ~(0x1 << pin);
	temp |= (GPIO_OTYPER_PP << pin);
	*((uint32_t *)GPIOI_OTYPER) = temp;

	// set GPIO OSPEEDR register
	temp = *((uint32_t *)GPIOI_OSPEEDR);
	temp &= ~(0x3 << (pin * 2));
	temp |= (GPIO_OSPEEDR_FULL << (pin * 2));
	*((uint32_t *)GPIOI_OSPEEDR) = temp;

	// set GPIO PUPDR register
	temp = *((uint32_t *)GPIOI_PUPDR);
	temp &= ~(0x3 << (pin * 2));
	temp |= (GPIO_PUPDR_UP << (pin * 2));
	*((uint32_t *)GPIOI_PUPDR) = temp;
}
