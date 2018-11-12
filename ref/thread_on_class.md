# 如何将线程封装在类中？  
---
### Linux下的线程是如何封装的  
最初C++没有封装线程的类，在Linux中，要使用线程，需要使用Linux自己的线程库，即pthread库。  
该库需要包含pthread.h，并且在连接时需要加入-pthread参数。  
来看下面的例子：  
```C++
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <iostream>

void *thread_func(void* arg)
{
	for(int i = 0; i < 1000000; ++i)
		cout << i << endl;
	pthread_exit(nullptr);
}

int main()
{
	for(int i = 0; i < 10; ++i)
	{
		pthread_t pid;
		void *ret;
		pthread_create(&pid, nullptr, thread_func, nullptr);
		//pthread_join(pid, &ret);
		//pthread_detach(pid);
	}
	sleep(10);
	return 0;
}
```
上面这段代码会生成10个线程，每个线程都会输出1到1000000之间的数，运行后会发现所有线程的输出处理是交织在一起的。  
由于pthread_create和pthread_join这类接口设计得并不是很好，因此在C++11中，C++提供了自己的线程接口。即thread类。thread中比较重要的就是join方法，当然，这里还可以看出pthread库的影子。  
再来看下面的例子：  
```C++
#include <iostream>
#include <thread>

using namespace std;

void myFirstThread()
{
	cout << "Hello thread!" << endl;
}

int main()
{
	thread myThread(myFirstThread);
	myThread.join();
	return 0;
}
```
在上面的例子中，pthread.h不见了，取而代之的是thread，这家伙就是C++提供的线程接口。这种设计中蕴含着一个重要的思想，即将应用程序与线程分开，这样做的好处是让应用模块的实现者尽量少的去关心线程配置的问题。  
### RTOS下线程的封装  
先说个小插曲。某系统关于线程类的设计过于复杂，主要是考虑了如何将模块独立为可执行程序的问题，这一部分其实和HMI的设计没有太大关系。除去关于可执行文件封装类和Application封装类，其实某系统的线程类和C++中thread类的封装方式大致是一样的。但还有些细微区别，稍后再分析。  

接下来还是直奔主题，如何把RTOS上的线程封装在类中。也就是说假设我们现在只有Linux的pthread库，而没有C++的thread类，考虑如何把线程和应用隔离开来。其实就是思考两个问题：  
1. 应该封装成啥样？  
2. 怎么实现？  

对于第一个问题，模糊的答案是：thread接口可以使用户将注意力集中在线程所要执行的逻辑上，而不需要关注创建线程、销毁线程等细节问题上。这么说固然有道理，但是上面那种绑定线程入口函数的方式毕竟也没有多麻烦，一个熟悉多线程的程序员就认定这么使用的话又有啥非改不可的理由呢？  
要理解这个问题，必须要知道一个事实，上面这个pthread库是基于POSIX规范的，它的实现是基于C语言的，因为Linux符合POSIX规范，所以在Linux上就可以用这一套来摆弄线程。但这有一个重大的Bug，如果底层操作系统换了咋办？比如换成Windows，又比如换成RTOS，这下找不到pthread_create接口了，代码咋办，重写吗？  
另一个问题是，pthread是个用C语言实现的库，在C++上面使用的时候就必须各种小心，这对C++程序员来说通常也是恶心人的地方。  
理解了这些，就明白为啥在C++11中要搞出thread这个东东了。考虑一下嵌入式开发环境，这是不是也很有必要？iTron有自己的线程接口cre_tsk，FreeRTOS也有自己的线程接口xTaskCreate，这些家伙林林总总，一个做应用程序开发的工程师要修改这些岂不疯掉？因此这部分最好来个统一的接口，这就是thread。  

既然如此，怎么把线程创建的这些细节隐藏起来呢？还是来参考现成的thread类的设计。C++的thread类大致是下面这个样子：  
```C++
class thread {

public:
	class id;
	typedef /*implementation-defined*/ native_handle_type;
 
	thread() noexcept;
	template <class F, class ...Args> explicit thread(F&& f, Args&&... args);
	~thread();
	thread(const thread&) = delete;
	thread(thread&&) noexcept;
	thread& operator=(const thread&) = delete;
	thread& operator=(thread&&) noexcept;

	void swap(thread&) noexcept;
	bool joinable() const noexcept;
	void join();
	void detach();
	id get_id() const noexcept;
	native_handle_type native_handle();

	static unsigned hardware_concurrency() noexcept;
};
```
这怎么看不到某系统中的start方法和run方法？不着急，拓展一下，来看看Java中的Tread类的定义。  
```Java
class Thread {

public:
	Thread();
	Thread(Runnable target);
	Thread(Runnable target, String name);
	......
	void interrupt();
	static boolean interrupted();
	boolean isAlive();
	void join();
	void run();
	void setName(String name);
	void setPriority(int newPriority);
	static void sleep(long millis);
	void start();
}
```
这里终于出现start方法和run了，某系统就是这个路子，因为熟悉Java嘛。但其实C++也差不多，只不过C++是在构造对象的时候就执行了start，而run也是在构造函数中直接传给了thread对象而已。  

现在来想想这一套是如何动起来的。简单来说就是：用户在自己的程序中生成一个Thread类的对象，然后调用start方法，此时thread内部启动了一个线程，并处于就绪状态（没运行）。然后Thread类调用方法run来启动该线程。实际上，start方法的作用是通知操作系统该线程已经准备就绪，以便让系统安排一个时间来调用其run方法，这就是这个基本的thread封装的底层逻辑。  
这里run方法称为线程体，它包含了要执行的这个线程的内容。run方法运行结束，此线程也终止，然后CPU再调度其它线程。在很多语言的封装中，run方法并非直接来自于Thread类，而是来自Runable接口。某系统就是这么做的。当然，简单起见，也可以不要这个Runable。  
其它一些主要接口还有join和destroy。join函数用于等待线束线程，而destroy则是用于销毁该线程所占用的资源。  

再来考虑一下第二个问题，即如何实现该接口。假设现在只有Linux的pthread库，没有thread类，自己设法编写一个通用的thread类，怎么做？可能很快会想到如此这般：  
```C++
class Thread {

public:
	void start() {
		int status;
		status = pthread_create(_pThread, NULL, Thread::run, NULL);
		if(status != 0) err_abort("creating thread failure", status);
	}
}
```
但很快就会发现，这么搞编译过程是会报错的！报错的根源在于pthread_create要求传入的线程入口函数的形式为：  
```C++
void *(*thread_routin)(void *args);
```
也就是说这里需要一个全局函数。但实际上，上述Thread::run提供的线程接口形式为：  
```C++
void Thread::run()
```
这是一个类的成员函数，这个矛盾该如何解决呢？  
有人会说我还用个全局函数不就完了吗？是可以，但我们之前用thread类的run方法作为线程体的设想不就泡汤了吗？而且，作为C++这种面向对象的设计，你一想到线程体作为一个个全局函数在你的一堆类之间满天飞，是不是很抓狂呢？  
这个咋处理呢，具体方法就不介绍了，推荐一个链接，可以再仔细看看。  
[https://blog.csdn.net/maotoula/article/details/18501963](https://blog.csdn.net/maotoula/article/details/18501963)  
[http://www.cppblog.com/Chosen/archive/2013/10/07/203567.html](http://www.cppblog.com/Chosen/archive/2013/10/07/203567.html)

最后总结一下，C++中thread类的封装不如Java中Thread类封装来得彻底。C++的thread类使用时，线程体仍然作为一个全局函数暴露在全局空间之中，而Java的Thread类使用时，线程体被巧妙的封装在了Thread类的run虚函数中，因此在Java中，Thread类的用法更加贴近面向对象的处理方式。

好了，当把上述这些问题都搞明白了以后，再回过头来看看某系统的线程类封装，是不是有种豁然开朗的赶脚？  