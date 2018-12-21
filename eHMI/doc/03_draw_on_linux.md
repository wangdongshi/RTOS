如何在Linux上画图？  
---
### X Window
X Window系统是一个基于网络的图形界面系统，它于1984年由MIT开发，有30多年的应用历史。X Window系统广泛的应用于桌面Linux（如Fedora、Debian、Ubuntu 等），嵌入式Linux等平台。  

大家都知道，Linux主打服务器市场，因此对于Linux而言，图形界面这个东西完全是个不必要的东西，相对于Windows的这个基于桌面的通用操作系统而言，Linux图形界面的重要性可谓非常之低，大家在Linux下全命令行的使用体验很好的说明了这一点。
但是，随着Linux的发展，显示的要求毕竟也是存在的，因此诞生了X Window系统。不同于Windows的图形界面，X Window是C/S架构的，也就是说，Linux上有个X Window的服务器，然后客户端的显示进程通过X11协议来和X Window服务器进行通信。从这种设计结构来看，显示的确算不上Linux的核心。  
X Window包括X Server、X Client、X协议三部分内容。其中X Client有三种开发模式：基于XLib、基于GTK、基于Qt。XLib是最底层的实现方式，GTK和QT想必大家都有所了解，就不罗嗦了。  

### X协议
X是协议，就像HTTP协议，IP协议一样。这个概念很多初学者，甚至学习Linux有一定时间的人都混淆。一个基于X的应用程序需要运行并显示内容时他就联接到X服务器，开始用X协议和服务器交谈。比如一个X应用程序要在屏幕上输出一个圆，X应用程序只负责告诉X服务器在屏幕的什么地方用什么颜色画一个多大的圆，而具体的”画”的动作，比如这个圆如何生成，用什么显卡的驱动程序去指挥显卡完成等等工作是由X服务器来完成的。X服务器还负责捕捉键盘和鼠标的动作，假设X服务器捕捉到鼠标的左键被按下了，它就告诉X应用程序：亲爱的应用程序先生，我发现鼠标被按下了，您有什么指示吗？  

有了协议就需要具体的软件来实现这个协议。Xfree86就是这样一个去根据法规实现协议的”交警”。在Linux上最常用的是XFree86，不过，现在的Linux发行版都用Xorg了。苹果电脑的图形界面用的也是X协议，而且被认为是做的最好的X协议图形界面，并且他对X协议的实施是做在系统内核里的，所以性能明显好很多，这就是为什么很多大型三维图形设计软件都是在苹果平台上的原因。  

既然X是协议，那么肯定和平台无关的，因此可以让X应用程序显示在任何装有X服务器的远程计算机上。甚至显示在装有Xnest服务器的windows系统上。只不过在大多数情况下，X应用程序都用本机的X服务器，然后X服务器将显示结果输出到本机的显示器。这也是很多人不理解X是一种协议的原因。  

### WM（window manager，窗口管理器）
先来看一个例子，用两种方式从字符界面进入图形界面。  
第一种在字符界面输入startx，起动图形界面，看到的是一个和平时使用一样的完整的图形界面操作环境，可以进行最大化，最小化，移动，关闭窗口等。   
按ctrl+alt+backspace反回字符界面。再输入xinit，再次启动图形界面，你看到一个XTERM，而且不能移动。但是你可以在这个XTERM中输入命令打开X应用程序，比如输入mozilla，这将打开浏览器，但是浏览器和平时有所不同，他在屏幕中间，不能移动，不能最小化，不能最大化，也没有边框。  
为什么同样一个X应用程序会有这样的不同呢？因为我们用startx起动图形界面的时候同时也启动了一个WM（即窗口管理器），如果你用KDE就起动了KDE，如果你用GNOME就起动了GNOME。（注意，也就是说KDE和GNOME都可以算是窗口管理器。）但是用xinit起动图形界面的时候却没有起动WM。现在明白窗口管理器的作用了吗？他的作用就是最大化，最小化，移动，关闭窗口等，而这些不是X服务器来负责完成的。  

LightDM是在Ubuntu中运行的显示管理器，最高版本为16.04LTS。虽然在后来的Ubuntu版本中它已被GDM取代，但默认情况下仍会在最新版本的几种Ubuntu版本中使用LightDM。  

### X的服务器与客户端  
搞清楚X server与X client关系很重要。一般X server很简单，就是/usr/bin/X11/X程序或Xorg程序，在Microsoft Windows上常用的X server有Xming，Xmanager，Exceed和X-win32等。  
而X client则花样繁多，从高级的CDE，GNOME，KDE，到低级一点的只有twm，Window Maker，blackbox等窗口管理器，再到最简陋的只有xterm，rxvt，xeyes等单个x程序。正是由于X client的各种搭配，使得我们的X Window系统看起来花样繁多。  

X服务器通常会在端口号为6000的TCP端口进行监听，而X客户端的描画就是向X服务器的6000端口发送描画请求的信息。

最后来总结一下在Linux中图形界面的一个层次关系：  
Linux本身–>X服务器<-[通过X协议交谈]->窗口管理器（综合桌面环境）–>X应用程序。   

### 在putty上使用X Window  
在产生向Linux移植图形界面的想法之前，自己一直都是通过putty/ssh登录Linux服务器来开发Linux程序的，现在突然发现此种方式好像没法操作图形界面。基于X Window的理念，想到完全可以在Windows端实现一个X Client，于是在网上搜了一下，果然是可以的，就是Xming。至于这个家伙怎么用大家可以自行百度。总之有了它配合putty，原来Linux的几乎所有图形应用就可以在Windows端再现了！  

### 一个简单的Xlib的例子  
网上有一篇介绍Xlib绘图的文章，很适合理解基础的概念。  
https://www.ibm.com/developerworks/cn/Linux/l-cn-xwin/index.html  
从这里可以看到，通过Xlib，可以完成在X Client上的绘图，也能从GUI上接收各种消息，非常适合这次GUI系统的模拟。  

### 为HMI提供基本描画元素  
#### 点和线  
现在已经有了在Linux上描画的基本方法，接下来需要考虑构建作为HMI基础的底层描画元素，换句话说，如何提供一个统一的接口来实现这些基础描画功能？回想下平时使用的GUI系统，似乎这里应该提供一个点、一条线、一个字符、一个图像的描画函数，这些东西构成基本描画元素，可以理解为最基本的方法，它们应该封装在一个HMI图形元素的基类中。在这之上可以构成种类的繁多的诸如Label、Combox、Button、Checkbox等等更为复杂的GUI元素。  

下面来具体分析一下这几个所谓的基本描画元素：点、直线、字符、图像。  

点的描画最简单，如果不考虑颜色的话，那么以一个函数来描述画点的描画方法，只需传入该点的坐标就够了。直线的描画是不是直需要传入一对坐标（开始坐标和结束坐标）就可以了？事情远没有这么Easy，如果直线不是水平的或是垂直的，那么应该如何去分配从开始点到结束点的角度变化，使这种变化是均匀分布在直线的每一点上吗，如果是这样那么应该以水平方向进行这种计算，还是以垂直方向进行这种计算呢？看来一个简单的直线描画真正实施起来并不容易。  
参考一下某系统的做法吧，很简单，它不提供斜线的描画，而只提供描画水平或垂直的直线的方法。换句话说，它的底层根本就没有提供画“直线”和画“点”的这种方法，而只有画“矩形”这种方法。只有画“矩形”的方法？这种设计乍听起来是不是很不可思议，但是仔细考虑下就会明白这其中的合理性。直线的描画由于上述所提到的那些问题造成了很麻烦的计算，这其实是一个矢量描画的问题。对于一个支持大屏幕的系统来讲，为了实现良好的显示效果它不得不支持这种计算，但是对于嵌入式系统，尤其是象某系统这样只有320x240这种小尺寸点阵LCD的系统来说则不然，在这种系统上，如果每描画一次直线都进行这种计算那也会是非常大的开销，而且，在这么小的屏幕上要描画复杂的各种斜线的情况本来就少得可怜，可以看看某系统的显示元素，诸如Label、Button、Textbox等，它们都根本不存在斜线的描画，因此专门考虑斜线描画显然是不必要的。如果我们只需要描画水平或垂直的直线，那么问题是不是大大的被简化了？再进一步思考一下，这种简化可以做到什么程度？画矩形。对了，只要支持了矩形的描画，直线就可以看作是长或宽为1个像素的矩形，而点也可以看作是长宽均为1个像素的矩形。  

某系统的底层显示设计中，对于这类元素（点、直线等）的描画，只提供了一个接口，就是矩形的描画。既然是画矩形，那么这个接口当然需要表示描画范围的参数，很容易想到，这可以由一个起始坐标（通常是矩形的左上角坐标）和矩形的长宽来表示。接下来还需要颜色参数，这里说的颜色就是将这个矩形全部都涂成一种颜色，有人可能会问，要是想描画边框颜色和内部颜色不同的矩形怎么办？很简单，边框以四条直线来描画，它们用边框的颜色，而矩形内部用则用另一个矩形来描画，它用矩形内部的颜色，而边框的四条线其实还是长或宽为1个像素的矩形，说来说去，它们还是都归结于矩形的描画。  
颜色的表示也是一个很大的问题，这其实主要是颜色的丰富性与存储容量之间的均衡考虑。简而言之，就是想要表现丰富的颜色就必须占用更大的存储空间。某系统的颜色表示采用的是调色板的方式，这个方法就不再这里进行展开了，总之它使用调色板的目的，是将256色的表示用一个字节的空间来解决，因为这里是在Linux上做模拟，就暂不考虑这种方式了，而是直接采用256色的RGB值，也就是一个4字节的存储空间来表示颜色信息。  

说到这里，点和线要考虑的问题基本上就差不多了，有人可能会想，如果真得需要描画“斜线”或“椭圆”之类的东西怎么办，很简单，一个点一个点描就是了，巧妇难为无米之炊啊。  

#### 字符  
好了，现在再来考虑另一类复杂的描画——字符。作为函数接口，直觉就是将ASCII字符和描画位置作为参数传入即可。但是字符怎么描画呢？如果重头实现HMI，不考虑这个问题就不行。从本质上来说其实就是先判断字符是什么，然后参照某个字体模板，一笔一画用点的描画堆砌起来。不过字符的写法其实多种多样，这样用什么风格来显示这个字符就成了问题。如果将字符的描画固定在程序中，那么希望改动字体时就要重新编译代码，这样的代价实在太大，因此可以将字符的描画抽象为一个字体文件，描画的时候按照字体文件中的描述来进行，就可以做到主程序不变，只改动字体文件就可以改动字符的描画风格，这也是大部分系统提供给应用开发者的基本结构。当然，这其中字体文件还有点阵和矢量的区分。  

某系统的底层描画函数提供了字符描画的接口，因为这是一个常用的必须实现的接口。本来在示例的HMI工程中也是想以字体文件的方式来模拟的，这样更接近嵌入式系统中的实际情况，但考虑到本次练习的目的，就不将问题过度复杂化了，这里简单以Linux提供的字符描画接口来实现。  

#### 图像  
其实一般还应提供图像描画函数，不过这个比字符描画就更复杂了，因为还有解析位图之类的操作。作为HMI的示例程序，这个函数没有的话其实也不影响示例工程的功能，因此这部分函数就不实现了，以免将问题复杂化。  

#### 设计上的考虑  
上面已经分析了在基本元素的描画层面要实现的几个方法，那么这些方法应该以什么样的形式提供给上层应用程序呢？当然，可以将这些方法（矩形、字符的描画）视为驱动程序提供的功能，如此一来这些方法就能够以全局函数的形式出现在HMI的工程中，这样做当然没问题，某系统的HMI也基本上是以这种思路完成的。（在DrawCom类中实现了一批静态方法，可以看作是全局方法。）  
但是，用静态方法类似于全局函数，并不是一个理想的实现，为此，可以参考大部分GUI系统的实现，既提供了一种所谓绘图上下文的类，这个类中维持着描画所需要的所有环境信息，比如，颜色、字体，当然也可以有我们这里需要封装的基本描画命令。  
这样做的好处是将来再出现任何描画上的新的基础功能时，我们可以用更灵活（继承或者修改特定的类，如颜色类、字体类、命令类）的方式进行处理，不至于在一个全局空间中随意的增加方法。  

下面就基于这种思想给出一个HMI的关键类描述。  
```C++
#include <X11/Xlib.h>

class SCDrawContext {
public:
	SCDrawContext();
	SCDrawContext(Display* d, Window& w);
	virtual ~SCDrawContext();

public:
	Display*	getDisplay(void) {return disp;};
	Window*		getWindow(void) {return &win;};
	GC*			getGC(void) {return &gc;};

protected:
	void		drawRect(const unsigned int x,
						const unsigned int y,
						const unsigned int width,
						const unsigned int height,
						const XColor& color);
	void		drawASCII(const unsigned int x,
						const unsigned int y,
						const char ascii,
						const XColor& fore_color,
						const XColor& back_color,
						const std::string font_name);

protected:
	Display*	disp;
	Window		win;
	GC			gc;
	
	static const std::string font_type[3];
};

#endif // __SCL_DRAW_CONTEXT_H__
```

```C++
#include "SCColor.h"
#include "SCDrawContext.h"

class SCDrawCommand : public SCDrawContext {
public:
	SCDrawCommand();
	SCDrawCommand(Display* d, Window& w);
	virtual ~SCDrawCommand();

public:
	bool		DrawPoint(const unsigned int x,
						const unsigned int y,
						const XColor& color = SC_COLOR("Black"));
	bool		DrawLine(const unsigned int x1,
						const unsigned int y1,
						const unsigned int x2,
						const unsigned int y2,
						const XColor& color = SC_COLOR("Black"));
	bool		FillRect(const unsigned int x,
						const unsigned int y,
						const unsigned int width,
						const unsigned int height,
						const XColor& color = SC_COLOR("Gray"));
	bool		DrawRect(const unsigned int x,
						const unsigned int y,
						const unsigned int width,
						const unsigned int height,
						const XColor& color = SC_COLOR("Black"));
	bool		DrawString(const unsigned int x,
						const unsigned int y,
						const std::string text,
						const std::string font = SC_FONT_MIDDLE,
						const XColor& fore_color = SC_COLOR("Black"),
						const XColor& back_color = SC_COLOR("White"));

private :
	SCColor*	pcolor;
};
```
