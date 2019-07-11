#ifndef __BARE_H__
#define __BARE_H__

template <class T>
class Bare
{
public:
    Bare(T data = 0) : m_data(data) {}
    Bare& operator++() { ++m_data; return *this; }
    void operator++() volatile { ++m_data; }
    operator T() const volatile { return m_data; }
private:
    T volatile m_data;
};

#endif // __BARE_H__
