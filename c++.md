# <center> The some question of C++ </center>#
1. 为何空类的大小不是零？<br>
为了确保两个不同对象的地址不同，<br>
new返回的指针总是指向不同的单个对象<br>

2. 我们是否可以将析构函数定义为虚函数？<br>
我们可以将一个析构函数定义为虚函数，<br>
而且当我们用一个基类对象的指针指向派生类在堆上new出来的内存时，我们必须这样做，<br>
如果不这样做，我们在delete 指针时就会调用基类的析构函数，但是我们的对象却是一个派生类的对象<br>
于是就有可能出现内存泄漏<br>

3. 如果我们用new去申请内存，但是却用delete[]去释放内存会出现问题吗？<br>
     * 内置类型， 自定义的没有显示的写析构函数的类型:<br>
因为我开辟的内存是连续的，而且我还能够知道这次我开辟了多少字节，<br>
所以在释放的时候会自己释放，不会造成内存泄漏<br>
![Alt text](doc/new2.png )
     * 自定义的自己写了析构函数的类型：<br>
有了自己的构造函数那么就不简简单单的只是开辟空间，还要调用自己写的构造函数， 来给我们的对象初始化。<br>
如果释放的时候是delete [];<br>
会根据当前对象的地址向上偏移拿到自己对象的个数，即使你对象是new A;也会这么做，然后根据这个数来调用析构函数，<br>
出错的位置是，我释放内存时rax减8,开始释放，于是就出现问题了 <br>
如果释放的时候是delete ;而开辟的时候是new A[];<br>
那么会出现从中间释放.<br>
![Alt text](doc/new1.png )
4. volatile的作用？<br>
    * 含义：<br>
    这个关键字的意思是易变的，于是就会要求编译器对于我们定义不能进行任何的优化<br>
    所谓的优化指的是，每次我们得到volatile定义的变量时，只能通过内存得到，<br>
    而不能通过寄存器得到，即使上一次取出的变量还存在于寄存器中。<br>
    * 使用：<br>
    使用时和const很像<br>
    非volatile可以赋给volatile变量反之不行<br>
    * 举例：<br>
    在STL中，二级配置器中的free_list就是volatile的，原因是他需要支持多线程.
5. c++中可以取代c的类型强转:<br>
	### static_cast<>()
	 
	1. 算术类型转化<br>
	```cpp
	int a = 10;
	float f = static_cast<float>(a);
	```
	2. 基类指针转为派生类指针<br>
	```cpp
	A *base;
	B *der;
	der = static_cast<B *>(base);
	```
	3. 转化为void<br>
	```cpp
	static_cast<void>(a);
	```
	### const_cast<>():	
	
	1. 去掉const属性，但是写未定义<br>
	2. 去掉volatile属性<br>
	```cpp
	int volatile *pa;
	int *pb = pa; // error
	int *pc = const_cast<int *>(pa); //right
	```
	### reinterpret_cast<>():
	
	1. 从bit位改变解析的方式<br>
	```cpp
	int a = 10;
	float f = *reinterpret_cast<float*> (&a);
	```
	2. 将int 转化位指针<br>
	```cpp
	void *p = reinterpret_cast<void *>(10);
	```
	3. pointer, pointer to function, reference<br>
	 
	### dynamic_cast<>()多用于动态类型识别:
	1. 继承关系中有虚函数<br>
	2. 基类指针指向派生类对象<br>
	```cpp
	/*A,B中必须有虚函数*/
	A *pbase = new B;
	B *pder = dynamic_cast<B *>(pbase);
	pder != NULL; // true;
	delete pder;
	pbase = new A;
	pder = dynamic_cast<B *>(pbase);
	pder == NULL; // true;
	delete pbase;
	```
