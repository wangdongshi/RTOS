如何实现HMI与其它线程间的通信？  
---
### 进程与线程
在进行下一步的说明之前，先来区分一个基础的概念，即进程和线程。  
Unix是诞生于上世纪60年代的操作系统，彼时线程还是一个新提出的概念，因此传统的Unix操作系统不支持多线程。这就是为什么《Unix环境高级编程》这种经典中也找不到关于多线程编程相关说明的原因。而Linux从诞生起就是基于Posix规范的，所以天生就支持多线程，这是Linux和Unix之间的一个显著区别。  
RTOS中通常不支持多进程，因为RTOS的设计理念和通用操作系统本来就不同，让RTOS支持进程管理这样的工作会带来巨大的系统开销，同时会显著降低调度的实时性，况且，多进程所依赖的虚拟内存结构在大部分RTOS环境下也不存在，因此大部分RTOS中都不存在进程的概念。不过并行处理依然是嵌入式系统中常见的需求，因此RTOS通常以提供线程调度的方式来支持这种应用。  

Linux同时支持多进程和多线程，这给程序开发者带来了更多便利，但我们有必要清楚的认识二者间的区别。进程和线程间的区别主要是它们对系统的开销有显著不同，进程是基于虚拟内存的，它用有“自己”的4GB地址空间、“自己”的全局变量、“自己”的文件描述符、“自己”的栈空间等等，这一切对于每个进程来说都是独享的，不存在和其它进程共用的问题。但线程除了拥有“自己”的栈空间之外，其它资源都与同进程的其它线程所共享。这种“轻量级”的变通处理对于每个线程来说当然是危险的，因为在一个线程中的某个不经意操作就会破坏其它线程的状态，这给程序开发者带来了更多的挑战。但是，线程间这种灵活的访问方式让不同任务间的通信开销显著的降低，从这一点来说，它又有着明显优于进程的地方。  

举个例子来说，假如计划在Linux上打算实现一个仪表的应用程序，其中既包括HMI，又包括测量等仪表的基本逻辑，这时候有两种选择：  
1. HMI和测量逻辑在同一进程中，采用不同线程。  
2. HMI和测量逻辑采用不同进程。  

两种方案没有本质的优劣之分，如果HMI特别复杂，画面数量庞大，则采用第2种方案可能更为合适。如果HMI比较简单，那么专门将其和测量逻辑分割为两个进程无异于自找麻烦，此时还是选择两个线程更为合适。  

### Linux线程同步  
本次的例子打算采用线程的方式来实现HMI和控制逻辑，即将HMI和控制逻辑放在同一个进程，两个不同的线程之中。
先来看看Linux线程间同步的常用方式有什么？  
1. 信号量  
2. 互斥锁  
3. 条件变量  

还有其它提供给进程间同步的各种系统资源：  
1. 管道   
2. 消息队列   
3. 共享内存   

这里面就有消息队列，可以直接拿来用。不过RTOS上显然没有提供这么多资源，但换个角度考虑，本质上所有的同步手段都可以通过上述基本的同步资源自己实现出来。某系统开发时就面临这个局面，因此设计之初就定下了一条规则，除了mutex，其它系统提供的同步资源都不允许使用。这也有个好处，就是在改换平台时系统具备很好的可移植性。  

### 自己实现MessageQueue  
上面讲了一堆，感觉是不是很麻烦？Linux、各种RTOS之间的线程通信方式不尽相同，而且接口函数也各异，怎么把这个过程也给封装起来，让它们能以一个统一的面目展现在应用程序的开发者面前？  
有两个思路，第一，利用各种操作系统提供的MesQueue或者DataQueue之类的资源，封装一个统一的MessageQueue类，提供统一操作接口。第二，干脆从Queue的实现做起，采用最基本的C++语言元素，自行搭建一套MessageQueue类。某系统采用了第二种方式，原因我想也很简单，因为搭建MessageQueue所用到的OS资源的种类非常少，所用到的C++特性也很简单，因此，自行封装并不复杂。  
具体说来，封装这个MessageQueue究竟用到了哪些资源呢？Queue肯定是需要的，这是MessageQueue的核心结构，但是这个可以自行实现。另外，为了同步，还需要Mutex。就这两种，没错就这两种，很简单吧。  

下面是某系统中的一个完整的Queue的实现，一共就60多行代码，真的是很简洁。  
```C++
template<class ELEMENT, unsigned int m_size>
class Queue {
	private:
		bool full;
		unsigned int front;
		unsigned int back;
		ELEMENT m_e[m_size];

		unsigned int next(unsigned int num) {
			return (num + 1) % m_size;
		}
	public:
		Queue() {
			front = 0;
			back = 0;
			full = false;
		}
		bool add(ELEMENT e) {
			if(full) {
				return false;
			} else {
				m_e[back] = e;
				back = next(back);
				if(front == back) {
					full = true;
				}
				return true;
			}
		}
		ELEMENT remove() {
			if(front != back || full) {
				if(full) {
					full = false;
				}
				ELEMENT e = m_e[front];
				front = next(front);
				return e;
			} else {
				throw std::out_of_range("Empty Queue.");
			}
		}
		ELEMENT element() {
			if(front != back || full) {
				return m_e[front];
			} else {
				throw std::out_of_range("Empty Queue.");
			}
		}
		std::size_t count()
		{
			if(full) {
				return m_size;
			} else {
				return front <= back ? back - front : back + m_size - front;
			}
		}
		void clear() {
			front = 0;
			back = 0;
			full = false;
		}
		bool empty() {
			return front == back && !full;
		}
		unsigned int size() {
			return m_size;
		}
};
```
Mutex的实现就离不开操作系统调用了，这个必须和OS结合起来，不过某系统也对Mutex的接口给出了定义，如下。  
```C++
class Mutex {
	private:
		void *mtx; /*!< OS-specific Mutex object pointer */
	public:
		Mutex();
		~Mutex();
		void set(void **_mtx);
		uint32_t *getId();
};
```

由于目前的目标是在Linux跑起来这个HMI系统，而Linux/GNU上的C++已经提供了完整的Queue和Mutex等OS资源，因此暂时就不自己来实现这些资源，直接用Linux的资源就好了。（至于在RTOS上实现这类C++接口，这个工作并不轻松，以后有时间的时候再做吧。）  

### HMI线程和控制线程  
按照上次介绍的C++封装的thread类库的实现方法，先生成HMI和控制逻辑的两个线程。下例中主程序在生成了HMI线程后，自己担当了控制逻辑的任务。  
```C++
#include <thread>
#include <unistd.h>
#include <termio.h>
#include "EHmiMain.h"

using namespace std;

void hmiMain(EHmiMain* pHmi)
{
	printf("HMI main thread is setup!\n");
	pHmi->Start();
}

int main( void )
{
	// create hmi main thread
	EHmiMain* pHmi = new EHmiMain();
	thread hmi_main(hmiMain, pHmi);
	hmi_main.detach();

	while(1) {
		sleep(1);
		int in = getchar();		
		EHmiEvent ev(HMI_EV_KEYDOWN, (unsigned long)in);
		{
			lock_guard<mutex> lock(pHmi->Mutex());
			pHmi->SetReady(true);
			pHmi->AddQueue(ev);
		}
	}
	return 0;
}
```

上面的例子当然还不能单独运行起来，因为还有很多新的东东没有定义，比如EHmiMain、EHmiEvent等等。在这里，只要看懂HMI线程的生成，以及控制线程（main函数）如何向HMI线程发消息就OK了。  

现在再来想想，HMI线程应该处理些什么？  
1. 它也需要一个主循环，在主循环中，它需要从Queue中获取一个消息，然后解释这个消息并作出相应的动作。  
2. 考虑到封装性，HMI线程最好封装在一个类之中，今后HMI的开发者就可以只关心它的HMI类了。  
3. Queue应该放在哪里？因为是HMI使用，当然应该由HMI的类来管理。  
4. Event应该设计成啥样？怎么把它装进Queue里再取出来？  

给出一个HMI类的大致框架，具体的实现自己填进去就好。
```C++
#include <queue>
#include <mutex>
#include "EHmiEvent.h"

#ifndef __EHMI_MAIN_H__
#define __EHMI_MAIN_H__

class EHmiMain {
public :
    EHmiMain();
    virtual ~EHmiMain();
	
public :
	void Start(void) {main();}
    std::mutex& Mutex(void) {return(mtx);}
    void AddQueue(EHmiEvent ev) {deq.push_back(ev);}
	
private :
	void main(void);
	void eventHandler(EHmiEvent& ev);

private :
    std::deque<EHmiEvent>	deq;
    std::mutex				mtx;
};

#endif // __EHMI_MAIN_H__
```
这里有个细节稍微说明一下，C++11的STL中有两种Queue，一种是普通的queue，另一种是双端口queue（名称为deque），考虑到要实现的消息队列应该是FIFO型，采用deque更为方便。  
上面这个HMI的类中，自己维持着一个消息队列，以及一个同步消息队列用的互斥锁。因为这将是属于HMI对象的存储空间（消息队列），因此，需要为其它线程访问这个消息队列提供接口，这就是AddQueue方法。当然，这个HMI类中最主要的处理将是main方法，这里面会从消息队列中依次取出消息并做相应动作。  

现在还有一件事未完成，就是消息本身该如何定义？  
这里的实现也是用一个类来封装，即EHmiEvent类。这个类的定义如下。  
```C++
#ifndef __EHMI_EVENT_H__
#define __EHMI_EVENT_H__

typedef union _EHmiEventParam{
	unsigned long	lp;
	unsigned int    ip[2];
	unsigned short  sp[4];
	_EHmiEventParam() {}
	_EHmiEventParam(unsigned long ul) {lp = ul;}
} EHmiEventParam;

typedef enum {
    HMI_EV_NONE,
    HMI_EV_KEYDOWN, // for test
} EHmiEventType;

class EHmiEvent {
public :
    EHmiEvent();
    EHmiEvent(EHmiEventType ev);
    EHmiEvent(EHmiEventType ev, unsigned long lp);
    ~EHmiEvent();
	
public :	
    EHmiEventType GetEvent() {return(type);}
    unsigned long GetULArg() {return(arg.lp);}
	void SetULArg(unsigned long ul) {arg.lp = ul;}
	
private :
    EHmiEventType	type;
    EHmiEventParam	arg;
};
#endif // __EHMI_EVENT_H__
```
事件类的结构比较简单，主要理解类中的两个属性的，一个是消息类型，即type，一个是消息参数（随消息发送的变量），即arg。  

EHmiMain、EHmiEvent就构成了HMI线程和其它线程交互的结构，并且，这里用类的方式把HMI和其它部分的处理隔离了开来。  