#对于智能指针的线程安全的分析
在同篇中shared_ptr,weak_ptr的源码已经分析了一遍<br>
##为什么boost库中的这两个只能指针是线程安全的?
thr1 and thr2 and thr3表示三个线程<br>
```cpp
/*伪代码*/
	case 1:
		thr1.add_use_count();
		thr2.add_use_count();
		//there is no thread try to delete msg
	case 2:
		thr1.release();
		thr2.release();
		//the last thread try to delete the msg, It's doesn't matter;
	case 3:
		thr1.add_use_count();
		thr2.release();
		//It's safe
	case 4:
		thr1.release();
		thr2.add_use_count();
		//when I begin to add, but in the same time,
		// thr other thread is ready to release()
		// Does it mean unsafe?
```
So in the case 4, It's unsafe;
-------
In the blog of [chenshuo](http://www.cppblog.com/Solstice/archive/2013/01/28/197597.html), he deal with it;<br>
But we can use weak_ptr to pass msg, Then use shared_ptr to use,<br>
It's safe;
```cpp
shared_count::shared_count( weak_count const & r ): pi_( r.pi_ )
{
    if( pi_ == 0 || !pi_->add_ref_lock() )
    {
        boost::throw_exception( boost::bad_weak_ptr() );
    }
}
bool add_ref_lock()
{
    return atomic_conditional_increment( &use_count_ ) != 0;
}	
/*操作类似， 只是不是真正的调用还是原子的*/
bool add_ref_lock()
{
     if( use_count_ == 0 ) return false;
     ++use_count_;
     return true;
}
case 3:
	thr2.release();
	thr1.copy();
```
when thr1 ready to add_ref_lock()<br>
thr2 release(), use_count == 0;<br>
In the add_ref_lock() return false;<br>
so it's safe;
##线程安全指的是在传递的时候利用weak_ptr,使用时lock()
  
  
  
  
              
