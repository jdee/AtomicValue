#ifndef __METADATA_H__
#define __METADATA_H__

namespace AtomicValue
{
template <class> class FastAtomicReader;
template <class> class FastAtomicWriter;
}

template <class> class MutexWrapper;

template <template <class> class T>
struct Metadata
{
};

template <>
struct Metadata<AtomicValue::FastAtomicReader>
{
    static constexpr const char* const testTitle = "AtomicValue";
};

template <>
struct Metadata<MutexWrapper>
{
    static constexpr const char* const testTitle = "mutex";
};

#endif // __METADATA_H__
