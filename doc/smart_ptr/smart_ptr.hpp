namespace hgg
{
    class count_base
    {
        private:
            int _use_count;
            int _weak_count;
        public:
            void add_use_count()
            {
                ++_use_count;
            }
            count_base(): _use_count(1), _weak_count(1){}
            virtual ~count_base()=0;
    };
    count_base::~count_base(){}
    template <typename T>
    class count_dervied : public count_base
    {
        private:
            T *_ptr;
        public:
            count_dervied(T *ptr) :_ptr(ptr) {}
    };
    class shared_count
    {
        private:
            count_base *_pi;
        public:
            shared_count() : _pi(0){}
            shared_count(const shared_count &sc) : _pi(sc._pi){
                if (_pi != 0) _pi->add_use_count(); // isn't atom
            }
            template <typename T>
            shared_count(T *px) : _pi(0)
            {
                try
                {
                    _pi = new count_dervied<T>(px);
                }
                catch (...)
                {
                    return;
                }
            }
            void swap(shared_count &other)
            {
                count_base *tmp = other._pi;
                other._pi = _pi;
                tmp = other._pi;
            }
    };
    template <typename T>
        class shared_ptr
        {
            private:
                shared_count _pn;
                T *_px;
            public:
                typedef T value_type;
                typedef shared_ptr<T> this_type;
                /*依赖于定义的shared_count::swap()
                 * 交换我所指对象及其引用计数*/
                void swap(shared_ptr &other)
                {
                    _px = other._px;
                    _pn.swap(other._pn);
                }
                void construct_count(T *p, shared_count &pn)
                {
                    shared_count(p).swap(pn);
                }
                shared_ptr(T *p) : _px(p), _pn(){
                    construct_count(p, _pn);
                }
                shared_ptr(const shared_ptr &other) 
                    : _px(other._px), _pn(other._pn)
                {}
                //shared_ptr &operator = (const shared_ptr &other)
                //{
                //    this_type().swap();
                //}
        };
};
//int main()
//{
//    hgg::shared_ptr<int> sp(new int);
//    return 0;
//}
