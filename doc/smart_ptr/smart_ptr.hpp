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
            void add_weak_ref()
            {
                ++_weak_count;
            }
            void reliease() {
                if (--_use_count == 0) {
                    delete this;
                }
            }
            void weak_reliease() {
                /*原则上这句话是不会发生的*/
                if (--_weak_count == 0) {
                    delete this;
                }
            }
            count_base(): _use_count(1), _weak_count(1){}
            virtual ~count_base()=0;
    };
    count_base::~count_base(){
    }
    template <typename T>
    class count_dervied : public count_base
    {
        private:
            T *_ptr;
        public:
            count_dervied(T *ptr) :_ptr(ptr) {}
            ~count_dervied(){
            }
    };
    class weak_count;
    class shared_count
    {
        private:
            friend class weak_count;
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
            shared_count(const weak_count &);
            void swap(shared_count &other)
            {
                count_base *tmp = other._pi;
                other._pi = _pi;
                _pi = tmp;
            }
            ~shared_count() {
                if (_pi != 0) _pi->reliease();
            }
    };
    template <typename Type>
    class weak_ptr;
    template <typename T>
        class shared_ptr
        {
            private:
                shared_count _pn;
                T *_px;
            public:
                template <typename Type> friend class weak_ptr;
                typedef T value_type;
                typedef shared_ptr<T> this_type;
                /*依赖于定义的shared_count::swap()
                 * 交换我所指对象及其引用计数*/
                void swap(shared_ptr &other)
                {
                    _px = other._px;
                    _pn.swap(other._pn);
                }
                T *get(){return _px;}
                void construct_count(T *p, shared_count &pn)
                {
                    /*it will produce a new count_base obj*/
                    shared_count(p).swap(pn);
                }
                // p -> shared_ptr : construct_count() -> 
                // shared_count()->new count_base: weak_count=use_count=1
                shared_ptr(T *p) : _px(p), _pn(){
                    construct_count(p, _pn);
                }
                // shared_ptr -> shared_ptr: _pn.constructor-> copy
                shared_ptr(const shared_ptr &other) 
                    : _px(other._px), _pn(other._pn)
                {}
                // assignment : shared_ptr -> shared_ptr ->this_type().swap();
                shared_ptr &operator = (const shared_ptr &other)
                {
                    /*from self to construct a shared_ptr obj
                     * for destruct old obj
                     *
                     * we must know I won't change the value of other
                     * I only to change the value of this*/
                    this_type(other).swap(*this);
                    return *this;
                }
                shared_ptr(const weak_ptr<T> &);
                T &operator *() {
                    return *_px;
                }
                T *operator ->() {
                    return _px;
                }
                //shared_ptr();
                /*because int the shared_ptr, we haven,t do new or malloc, 
                 * so I need't to write destructor*/
                //~shared_ptr();
        };
};
namespace hgg
{
    class weak_count
    {
        private:
            friend class shared_count;
            count_base *_pi;
        public:
            weak_count(const shared_count &sc) : _pi(sc._pi)
            {
                if (_pi != 0) _pi->add_weak_ref();
            }
            weak_count(const weak_count &wp) : _pi(wp._pi) {
                if (_pi != 0) _pi->add_weak_ref();
            }
            void swap(weak_count &other)
            {
                count_base *tmp = other._pi;
                other._pi = _pi;
                _pi = tmp;
            }
            ~weak_count(){
                if (_pi != 0) _pi->weak_reliease();
            }
    };
    template <typename T>
    class weak_ptr
    {
        private:
            template <typename Type> friend class shared_ptr;
            weak_count _pn;
            T *_px;
            weak_ptr(T *);
            /*refuse the pointer copy*/
        public:
            shared_ptr<T> lock() const
            {
                return shared_ptr<T>(*this);
            }
            void swap(weak_ptr &wp) {
                _px = wp._px;
                _pn.swap(wp._pn);
            }
            weak_ptr(const shared_ptr<T> &sp) 
                : _px(sp._px), _pn(sp._pn){}
            weak_ptr(const weak_ptr &wp)
                : _px(wp.lock().get()), _pn(wp._pn){}
            weak_ptr &operator = (const weak_ptr & wp) {
                weak_ptr(wp).swap(*this);
                return *this;
            }
    };
    template <typename T>
    shared_ptr<T>::shared_ptr(const weak_ptr<T> &wp) 
        : _px(wp._px), _pn(wp._pn){}
    shared_count::shared_count(const weak_count &wc) : _pi(wc._pi){
        if (_pi != 0) _pi->add_use_count();
    }
};
