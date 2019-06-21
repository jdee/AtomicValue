#ifndef __MUTEXWRAPPER_H__
#define __MUTEXWRAPPER_H__

#include <pthread.h>

class ScopeLock
{
public:
    ScopeLock(pthread_mutex_t volatile& mutex) :
        m_mutex(const_cast<pthread_mutex_t&>(mutex))
    {
        pthread_mutex_lock(&m_mutex);
    }

    ~ScopeLock()
    {
        pthread_mutex_unlock(&m_mutex);
    }

private:
    pthread_mutex_t& m_mutex;
};

template <class T>
class MutexWrapper
{
public:
    MutexWrapper(T data = T()) : m_data(data)
    {
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);

        pthread_mutex_init(&m_mutex, &attr);
        pthread_mutexattr_destroy(&attr);
    }

    ~MutexWrapper()
    {
        pthread_mutex_destroy(&m_mutex);
    }

    void set(T data) volatile
    {
        ScopeLock _l(m_mutex);
        m_data = data;
    }

    T get() const volatile
    {
        ScopeLock _l(m_mutex);
        return m_data;
    }

    MutexWrapper volatile& operator++() volatile
    {
        ScopeLock _l(m_mutex);
        ++m_data;
        return *this;
    }

    operator T() const volatile
    {
        ScopeLock _l(m_mutex);
        return get();
    }

private:
    pthread_mutex_t mutable m_mutex;
    T volatile m_data;
};

#endif // __MUTEXWRAPPER_H__
