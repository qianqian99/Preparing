###About smart ptr of cpp###
在boost库中存在两个智能指针是比较常用
的`shared_ptr`和`weak_ptr`。
```cpp
namespace boost {
  template<typename T>
  class shared_ptr {
  public:
    template <class Y> explicit shared_ptr(Y* p);
    ~shared_ptr();
    shared_ptr(const shared_ptr & r);
    template <class Y> 
    explicit shared_ptr(const weak_ptr<Y>& r);
    shared_ptr& operator=(const shared_ptr& r);
    void reset();
    T& operator*() const;
    T* operator->() const;
    T* get() const;
    long use_count() const;
  };
}
```
> 上面是我们关注的几个函数
> 下面是我对于SGI中的源码的分析
>> shared_ptr
>>> T *px;
>>> shared_count pn;
>>>> int id_;
>>>> sp_counted_base *pi_;
>>>>> long use_count;
>>>>> long weak_count;

```cpp
void main()
{
	shared_ptr<int> sp = new int;
}
```
> 程序调用了
    shared_ptr( Y * p ): px( p ), pn() 
    boost::detail::sp_pointer_construct( this, p, pn );
    //接受的参数是智能指针， 裸指针p， 引用计数pn
    void sp_pointer_construct( 
    ppx, p, shared_count & pn )
    {//shared_count 中的_pi指向了一个具体的实例，两个计数都是1，包含了裸指针
   		shared_count( p ).swap( pn )；
    }
> 在 shared_count的默认构造中 pi_被初始化NULL，
> 在shared_count的拷贝构造中
    shared_count(shared_count const & r): pi_(r.pi_) 
    {
        if( pi_ != 0 ) pi_->add_ref_copy();
        //use_count++;// 原子操作
    }
> shared_ptr<int> sp2(sp);
> 调用copy constructor强引用计数+1
```cpp
namespace boost {
  template<typename T> class weak_ptr {
  public:
    template <typename Y>
    weak_ptr(const shared_ptr<Y>& r);
    weak_ptr(const weak_ptr& r);
    ~weak_ptr();
    bool expired() const; 
    shared_ptr<T> lock() const;
  };  
} 
```
> 可以看出weak_ptr 中不能通过裸指针构造 
> weak_ptr<int> wp(sp2);
> weak_ptr(const shared_ptr<Y>& r);---> px( r.px ), pn( r.pn )
> 指向相同的裸指针和引用计数    weak_count(shared_count const & r): pi_(r.pi_)
> pi_->weak_count++;
> weak_ptr<int> wwp(wp);
> 通过弱引用进行构造 weak_ptr( weak_ptr<Y> const & r );----->
> px(r.lock().get()), pn(r.pn)
```cpp
shared_ptr<T> lock() const BOOST_NOEXCEPT
{
	return shared_ptr<T>( *this, boost::detail::sp_nothrow_tag() );
}
// lock的用途是构造了一个临时shared_ptr，px指向了裸指针，引用计数指向了原来的引用计数，
// 并使use_count_++;产生了强指针
shared_ptr( weak_ptr<Y> const & r, boost::detail::sp_nothrow_tag )
    : px( 0 ), pn( r.pn, boost::detail::sp_nothrow_tag() )
    {
        if( !pn.empty() )
        {
            px = r.px;
        }
    }
inline shared_count::shared_count( weak_count const & r, sp_nothrow_tag )
		: pi_( r.pi_ ), id_(shared_count_id)
{
    if( pi_ != 0 && !pi_->add_ref_lock() )
    {
        pi_ = 0;
    }
}
bool add_ref_lock() 
{
   if( use_count_ == 0 ) return false;
   ++use_count_;
   return true;
}
T * get() const
{
   return px;
}
//由于在整个过程中，weak_ptr 只能传递,不能get，所以我们先lock得到我们的T*
//我们的引用计数会发生改变增加一次，等lock的对象结束之后，--；
weak_count(weak_count const & r): pi_(r.pi_) 
{
     if(pi_ != 0) pi_->weak_add_ref();//weak_ptr++;
}
```
> 经过以上分析可知道:
* shared_ptr->weak_ptr :
>> px回指, pn中的pi指向shared_ptr中的pi,  pi->weak_count++;
* weak_ptr->weak_ptr :
>> px需要回指, 但是weak_ptr不能直接get,需要先进行lock, 
>> pn中的pi指向shared_ptr中的pi,  pi->weak_count++;
* weak_ptr->shared_ptr :
>> px需要回指, pn指向weak_ptr中的pn， pn.pi->use_count++;
* shared_ptr->shared_ptr :
>> px需要回指, pn指向weak_ptr中的pn， pn.pi->use_count++;
* T *p->shared_ptr : 
>> px回指, pn(),什么都没有，sp_pointer_construct()的调用
>> shared_count( p ).swap( pn )；
>> 在堆空间上new了一个具体的shared_count_base继承类的实例
>> use_count=weak_ptr=1,Then 将管理权给了pn;
>
>
> 还有一些基本的函数这里给出定义:
```cpp
shared_ptr & shared_ptr::operator=( shared_ptr const & r ) 
{
	this_type(r).swap(*this);
	return *this;
}
void shared_ptr::swap( shared_ptr & other )
{
	std::swap(px, other.px);
	pn.swap(other.pn);
}
void shared_count::swap(shared_count & r)
{
    sp_counted_base * tmp = r.pi_;
    r.pi_ = pi_;
    pi_ = tmp;
}
bool shared_count::empty() const
{
    return pi_ == 0;
}
/*只有以个强指针引用*/
bool shared_count::unique() const
{
     return use_count() == 1;
}
long shared_count::use_count() const 
{
     return pi_ != 0? pi_->use_count(): 0;
}
/*都只是一层封装*/
long shared_ptr::use_count() const
{
    return pn.use_count();
}
/*在pn的pi中重新new一个*/
void shared_ptr::reset() 
{
   this_type().swap(*this);
}
value_type operator* () const
{
   return *px;
}
point_type operator-> () const 
{
       
   return px;
}   
```
##在doc中我也会实现自己的smart_ptr






