如何实现HMI与其它线程间的通信？  
---
### 事件&队列  
先来看看Linux线程间通信的常用方式有什么？  

### 自己实现一个MessageQueue  
上面讲了一堆，感觉是不是很麻烦？Linux、各种RTOS之间的线程通信方式不尽相同，而且接口函数也各异，怎么把这个过程也给封装起来，让它们能以一个统一的面目展现在应用程序的开发者面前？  
有两个思路，第一，利用各种操作系统提供的MesQueue或者DQueue之类的资源，封装一个统一的MessageQueue类，提供统一操作接口。第二，干脆从Queue的实现做起，采用最基本的C++语言元素，自行搭建一套MessageQueue类。某系统采用了第二种方式，原因我想也很简单，因为搭建MessageQueue所用到的OS资源的种类非常少，所用到的C++特性也很简单，因此，自行封装并不复杂。  
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