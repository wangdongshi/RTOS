如何在Linux上画图？  
---
### X Window
X Window系统是一个基于网络的图形界面系统，它于1984年由MIT开发，有30多年的应用历史。X Window系统广泛的应用于桌面Linux（如Fedora、Debian、Ubuntu 等），嵌入式Linux等平台。  

大家都知道，Linux主打服务器市场，因此对于Linux而言，图形界面这个东西完全是个不必要的东西，相对于Windows的这个基于桌面的通用操作系统而言，Linux图形界面的重要性可谓非常之低，大家在Linux下全命令行的使用体验很好的说明了这一点。
但是，随着Linux的发展，显示的要求毕竟也是存在的，因此诞生了X Window系统。不同于Windows的图形界面，X Window是C/S架构的，也就是说，Linux上有个X Window的服务器，然后客户端的显示进程通过X11协议来和X Window服务器进行通信。从这种设计结构来看，显示的确算不上Linux的核心。  
X Window包括X Server、X Client、X协议三部分内容。其中X Client有三种开发模式：基于XLib、基于GTK、基于Qt。XLib是最底层的实现方式，GTK和QT想必大家都有所了解，就不罗嗦了。  

### 在putty上使用X Window
在产生向Linux移植图形界面的想法之前，自己一直都是通过putty/ssh登录Linux服务器来开发Linux程序的，现在突然发现此种方式好像没法操作图形界面。基于X Window的理念，想到完全可以在Windows端实现一个X Client，于是在网上搜了一下，果然是可以的，就是Xming。至于这个家伙怎么用大家可以自行百度。总之有了它配合putty，原来Linux的几乎所有图形应用就可以在Windows端再现了！  

### 一个简单的Xlib的例子  
网上有一篇介绍Xlib绘图的文章，很适合理解基础的概念。  
https://www.ibm.com/developerworks/cn/linux/l-cn-xwin/index.html  
从这里可以看到，通过Xlib，可以完成在X Client上的绘图，也能从GUI上接收各种消息，非常适合这次GUI系统的模拟。  

### 为HMI提供基本描画元素  
现在已经有了在Linux上描画的基本方法，接下来需要考虑构建作为HMI基础的底层描画元素，换句话说，如何提供一个统一的接口来实现这些基础描画功能？按照某系统的设计，此处应该提供一个点、一条线、一个矩形区域、一个字符串的描画函数，而这些基本的描画方法应该封装在一个HMI图形元素的基类中。其实一般还应提供图像描画函数，不过这个稍微复杂了一些，因为有解析位图之类的操作，作为HMI的示例程序，这个可以先不处理，以免一开始将问题复杂化。  
画点最简单，如果只考虑黑白两色，那么以一个函数来考虑的话，只需要传入该点的坐标就够了。描画直线需要传入一对坐标，描画一个矩形区域需要一个起始坐标和矩形区域的长宽。接下来如果再加上颜色的话，情况就稍微复杂些，尤其是描画矩形还需要考虑前景色和背景色。   再进一步，点的坐标一定是一个常用的结构，应该抽象为类，颜色如果以RGB来表示的话，也应该将其抽象为一个类。  
最后说一下字符串描画问题。作为函数接口，直觉就是将ASCII字符数组构成的C格式字符串作为参数传入，挨个描画字符即可。但是字符怎么描画呢？如果重头实现HMI，不考虑这个问题就不行了。从根本上来说其实就是先判断字符是什么，然后一笔一画用点的描画堆砌起来。不过字符的写法其实多种多样，这样用什么风格来显示这个字符就成了问题，如果字符描画固定在程序中，那么希望改动字体时就要重新编译代码，这样代价实在是太大了，就可以抽象为一个字体文件，描画的时候按照字体文件中的描述来进行，就可以做到主程序不变，只改动字体文件就可以改动字符的描画风格，这也是大部分系统提供给应用开发者的基本结构。当然，这其中字体文件还有点阵和矢量的区分。幸好现在是在Linux上来实现字符串描画，系统当然提供了字体，也就不用从字体文件开始实现了。   

下面做出这个HMI描画基类的头文件。   
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