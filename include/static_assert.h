#ifndef __STATIC_ASSERT_H
#define __STATIC_ASSERT_H

template <bool expr>
struct StaticAssertion
{
    static void test() {}
};

template <>
struct StaticAssertion<true>
{
    static void test(...) {}
};

#define STATIC_ASSERT(expr, errstr) \
    { \
        constexpr bool const bexpr(expr); \
        int errstr(0); \
        StaticAssertion<bexpr>::test(errstr); \
    }

#endif // __STATIC_ASSERT_H
