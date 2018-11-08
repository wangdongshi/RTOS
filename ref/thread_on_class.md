# 如何将线程封装在类中？  
---
### Linux下的线程是如何封装的  
最初C++没有封装线程的类，在Linux中，要使用线程，需要使用Linux自己的线程库，即pthread库。  
该库需要包含pthread.h，并且在连接时需要加入-pthread参数。  
来看下面的例子：  
```
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
```
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
在上面的例子中，pthread.h不见了，取而代之的是thread，这家伙就是C++提供的线程接口。用这个类来封装线程有啥好处呢？好处很多：  
1. 支持lambda，创建线程可以传入lambda作为执行函数；  
2. 支持任意多个参数；  
3. 使用方便，各种函数都经过了良好设计，使用起来比posix方便。  
总之吧，从那时起C++中的线程就有了比较固定的样子。  

回过头来看某系统的设计方式。某系统的类设计过于复杂，主要是考虑了如何将模块独立为可执行程序的问题，这一部分其实和HMI的设计没有太大关系。  
在某系统类设计当中还有一个重要的思想，即将应用程序与线程分开，这样做的好处是让应用模块的实现者尽量少的去关心线程配置的问题。这一点其实我理解与上面C++线程库的封装思想是一致的，不仅如此，某系统的类设计当中也存在Thread这个类（类似于C++线程库的thread），而且它也具有join这个函数接口。  
这里有个小问题。没有run接口，C++线程库如何运行？答案是调用join就运行了，这些线程会自己排队的。那某系统为什么使用join和run？注意观察hmi_main.cpp中的thr.start和thr.join，这两个操作中join和线程库的join差不多，start其实不是运行Thread库中的start，而是RealHexThread类中覆写的start，这里面只进行线程ID的生成。  
这样分析一下就明白了，除去某系统的可执行文件封装类和Application封装类，其实某系统的线程和C++中线程库的封装方式大致是一样的。还有些细微区别，稍后再分析。  

接下来还是直奔主题，如何把RTOS上的线程封装在类中。也就是说假设我们现在只有Linux的pthread库，而没有C++的thread库，如何考虑把线程和应用隔离开来的事情。  
来看pthread库原本推荐的使用方式，它是这么干的：  
```
pthread_create(&pid, nullptr, thread_func, nullptr)
```
这样看上去挺方便啊，把线程的入口绑定在一个函数上，很简单不就OK了吗？但这里隐含着一件很麻烦的事情，每次加入一个线程的话都要考虑这个无脑出现的全局函数应该放在何处。换句话说，这种方式只能静态的绑定每个线程的主函数，而无法把启动线程的这个处理给隔离开。  
知道了这种方式不好，回过头来思考两个问题：  
1. 应该封装成啥样？  
2. 怎么实现？  
对于第一个问题，有片儿汤话这么说：thread接口可以使用户将注意力集中在线程所要执行的逻辑上，而不需要关注创建线程、销毁线程等细节问题上。这么说固然有道理，但是上面那种绑定线程入口函数的方式毕竟也没有多麻烦，一个熟悉多线程的程序员就认定这么使用的话又有啥非改不可的理由呢？  
要理解这个问题，必须要知道一个事实，上面这个pthread库是基于POSIX规范的，它的实现是基于C语言，而Linux因为符合POSIX规范，因此在Linux上就可以用这一套来摆弄线程。但是这有一个重大的Bug，如果底层操作系统换了咋办？比如换成Windows，又比如换成RTOS，这下找不到pthread_create接口了，代码咋办，重写吗？  
另外，pthread是个用C语言实现的库，在C++上面使用的时候必须各种小心，这也是一大麻烦。  
理解了这些，就明白为啥在C++11中，要搞出thread这个东东了。考虑我们的嵌入式开发环境，这是不是也很有必要？iTron有自己的线程接口cre_tsk，FreeRTOS也有自己的线程接口xTaskCreate，这些家伙林林总总，一个做应用程序开发的工程师要修改这些岂不疯掉？因此这部分最好来个统一的接口，这就是thread。  
既然如此，怎么把线程创建这些细节隐藏起来呢？还是来参考现成的thread类的设计。thread大致是下面这个样子：  
```
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
没看见某系统中的start方法和run方法对不？呵呵，再看下Java中的Tread类的定义。  
```Java
class Thread {
public:
	Thread();
	Thread(Runnable target);
	Thread(Runnable target, String name);
	Thread(String name);
	
	static int activeCount();
	void checkAccess();
	static Thread currentThread();
	long getId();
	String getName();
	int getPriority();
	Thread.State getState();
	static boolean holdsLock(Object obj);
	void interrupt();
	static boolean interrupted();
	boolean isAlive();
	boolean isDaemon();
	boolean isInterrupted();
	void join();
	void join(long millis);
	void join(long millis, int nanos);
	void run();
	void setName(String name);
	void setPriority(int newPriority);
	static void sleep(long millis);
	static void sleep(long millis, int nanos);
	void start();
	String toString();
	static void yield();
}
```
这里终于出现start方法和run了，某系统就是这个路子，因为有人熟悉Java嘛。但其实也差不多，C++只不过是在构造对象的时候就执行了start，而run也是在构造函数中传进去的而已。  
现在来想想这一套如何动起来。简单来说就是：  
用户一般在自己的程序中调用start方法来启动一个线程，这时此线程是处于就绪状态，并没有运行。然后通过此Thread类调用方法run来完成其运行操作的，这里方法run称为线程体，它包含了要执行的这个线程的内容.run方法运行结束，此线程终止。然后CPU再调度其它线程。 
