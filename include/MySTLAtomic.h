#ifndef __MYSTLATOMIC_H__
#define __MYSTLATOMIC_H__

#include <atomic>

template <class T>
class MySTLAtomic : public std::atomic<T>
{
public:
    typedef typename std::atomic<T> Base;
    MySTLAtomic(T t = T()) : Base(t) {}
    void operator=(T data) volatile { Base::operator=(data); }
    MySTLAtomic& operator=(T data) { Base::operator=(data); return *this; }
};

#endif // __MYSTLATOMIC_H__
