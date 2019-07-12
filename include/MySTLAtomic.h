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
    MySTLAtomic& operator=(T data) { return Base::operator=(data); }
};

#endif // __MYSTLATOMIC_H__
