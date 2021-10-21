/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2019        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various existing       */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "ff.h"			/* Obtains integer types */
#include "diskio.h"		/* Declarations of disk functions */
#include "stm32f746g_disco.h"

/* Definitions of physical drive number for each drive */
#define DEV_RAM		1	/* Example: Map RAMdisk to physical drive 0 */
#define DEV_MMC		0	/* Example: Map MMC/SD card to physical drive 1 */
#define DEV_USB		2	/* Example: Map USB MSD to physical drive 2 */

static DRESULT sd_ioctl (BYTE cmd, void *buff);

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive number to identify the drive */
)
{
	DSTATUS stat = RES_NOTRDY;
	int result = STA_NODISK;

	switch (pdrv) {
	case DEV_RAM :
		//result = RAM_disk_status();

		// translate the result code here
		stat = (result == 0) ? RES_OK : RES_NOTRDY;

		break;

	default:
	case DEV_MMC :
		//result = MMC_disk_status();
		result = 0;

		// translate the result code here
		stat = (result == 0) ? RES_OK : RES_NOTRDY;

		break;

	case DEV_USB :
		//result = USB_disk_status();

		// translate the result code here
		stat = (result == 0) ? RES_OK : RES_NOTRDY;

		break;
	}

	return stat;
}



/*-----------------------------------------------------------------------*/
/* Initialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive number to identify the drive */
)
{
	DSTATUS stat = RES_ERROR;
	int result = STA_NOINIT;

	switch (pdrv) {
	case DEV_RAM :
		//result = RAM_disk_initialize();

		// translate the result code here
		stat = (result == 0) ? RES_OK : RES_ERROR;

		break;

	default:
	case DEV_MMC :
	{
		char* msg[2] = {
			"Failed to identify SD card.\r\n",
			"SD card identification is success.\r\n"
		};
		//result = MMC_disk_initialize();
		result = initSDCard();
		TRACE(msg[result]);

		// translate the result code here
		stat = (result) ? RES_OK : RES_ERROR;
	}
		break;

	case DEV_USB :
		//result = USB_disk_initialize();

		// translate the result code here
		stat = (result == 0) ? RES_OK : RES_ERROR;

		break;
	}

	return stat;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive number to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	LBA_t sector,	/* Start sector in LBA */
	UINT count		/* Number of sectors to read */
)
{
	DSTATUS stat = RES_PARERR;
	int result = STA_NOINIT;

	switch (pdrv) {
	case DEV_RAM :
		// translate the arguments here

		//result = RAM_disk_read(buff, sector, count);

		// translate the result code here
		stat = (result == 0) ? RES_OK : RES_PARERR;

		break;

	default:
	case DEV_MMC :
		// translate the arguments here

		result = sdDMARead((uint32_t)sector, (uint32_t)count, (uint8_t*)buff);
		//result = sdPollingRead((uint32_t)sector, (uint32_t)count, (uint8_t*)buff);

		if (result && updateSDCardStatus()) {
			while (isSDCardInTansfer());
		}
		else {
			result = False;
		}

		// translate the result code here
		stat = (result) ? RES_OK : RES_PARERR;

		break;

	case DEV_USB :
		// translate the arguments here

		//result = USB_disk_read(buff, sector, count);

		// translate the result code here
		stat = (result == 0) ? RES_OK : RES_PARERR;

		break;
	}

	return stat;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if FF_FS_READONLY == 0

DRESULT disk_write (
	BYTE pdrv,			/* Physical drive number to identify the drive */
	const BYTE *buff,	/* Data to be written */
	LBA_t sector,		/* Start sector in LBA */
	UINT count			/* Number of sectors to write */
)
{
	DSTATUS stat = RES_PARERR;
	int result = STA_NOINIT;

	switch (pdrv) {
	case DEV_RAM :
		// translate the arguments here

		//result = RAM_disk_write(buff, sector, count);

		// translate the result code here
		stat = (result == 0) ? RES_OK : RES_PARERR;

		break;

	default:
	case DEV_MMC :
		// translate the arguments here

		result = sdDMAWrite((uint32_t)sector, (uint32_t)count, (uint8_t*)buff);
		//result = sdPollingWrite((uint32_t)sector, (uint32_t)count, (uint8_t*)buff);

		if (result && updateSDCardStatus()) {
			while (isSDCardInTansfer());
		}
		else {
			result = False;
		}

		// translate the result code here
		stat = (result) ? RES_OK : RES_PARERR;

		break;

	case DEV_USB :
		// translate the arguments here

		//result = USB_disk_write(buff, sector, count);

		// translate the result code here
		stat = (result == 0) ? RES_OK : RES_PARERR;

		break;
	}

	return stat;
}

#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive number (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	DSTATUS stat = RES_PARERR;
	int result = STA_NOINIT;

	switch (pdrv) {
	case DEV_RAM :

		// Process of the command for the RAM drive
		stat = (result == 0) ? RES_OK : RES_PARERR;

		break;

	default:
	case DEV_MMC :

		// Process of the command for the MMC/SD card
		stat = sd_ioctl(cmd, buff);

		break;

	case DEV_USB :

		// Process of the command the USB drive
		stat = (result == 0) ? RES_OK : RES_PARERR;

		break;
	}

	return stat;
}

static DRESULT sd_ioctl (
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	switch (cmd) {
	case GET_SECTOR_SIZE : // Get R/W sector size (WORD)
		*(WORD *)buff = SD_BLOCK_SIZE;
		break;
	case GET_BLOCK_SIZE : // Get erase block size in unit of sector (DWORD)
		//*(DWORD *)buff = SDCardInfo.CardBlockSize;
		*(DWORD *)buff = SD_BLOCK_SIZE; // TODO
		break;
	case GET_SECTOR_COUNT:
		//*(DWORD*)buff = SDCardInfo.CardCapacity/SDCardInfo.CardBlockSize;
		*(DWORD*)buff = SD_BLOCK_NUM; // TODO
		break;
	default:
	case CTRL_SYNC :
		break;
	}

	return RES_OK;
}
