#ifndef __METADATA_H__
#define __METADATA_H__

#include <atomic>

template <class T>
class MySTLAtomic : public std::atomic<T>
{
public:
    typedef typename std::atomic<T> Base;
    MySTLAtomic(T t = T()) : Base(t) {}
    void operator++() volatile { Base::operator++(); }
    operator T() const volatile { return Base::operator T(); }
};

namespace AtomicValue
{
template <class> class FastAtomicReader;
template <class> class FastAtomicWriter;
}

template <class> class MutexWrapper;

template <template <class> class>
struct Metadata
{
};

template <>
struct Metadata<AtomicValue::FastAtomicReader>
{
    static constexpr const char* const testTitle = "FastAtomicReader";
};

template <>
struct Metadata<AtomicValue::FastAtomicWriter>
{
    static constexpr const char* const testTitle = "FastAtomicWriter";
};

template <>
struct Metadata<MutexWrapper>
{
    static constexpr const char* const testTitle = "mutex";
};

template <>
struct Metadata<MySTLAtomic>
{
    static constexpr const char* const testTitle = "std::atomic";
};

#endif // __METADATA_H__
