//
//  AtomicValue.h
//  AtomicValue
//
//  Created by Jimmy Dee on 9/20/14.
//  Copyright (c) 2014-19 Jimmy Dee. All rights reserved.
//

#ifndef AtomicValue_AtomicValue_h
#define AtomicValue_AtomicValue_h

#include <ctime>

namespace AtomicValue
{

/*
 * Simple, fast, imperfect synchronization mechanism. This template is optimized for fast reading. It's used by the Treadmill
 * class in an audio unit input callback on a real-time thread, which cannot afford anything more complex than simple integer/
 * pointer arithmetic.
 *
 * The get() member function simply increments an integer counter and decrements it. When not being read, the counter is 0.
 * With a single reader, it occasionally is incremented to 1, to indicate that it's currently being read.
 *
 * The set() function first checks m_lockCounter. Once it detects that the counter is 0, it immediately sets m_data. If the
 * counter is positive, rather than using a more expensive concurrency mechanism like a condition wait, it simply sleeps for
 * a very short time, checking the counter each time it wakes up. If set begins while get is executing, it will be delayed.
 *
 * This class remains fast by never delaying get() longer than is necessary to increment and decrement an integer. There are
 * still questions regarding safety.
 *
 * This template should only be used with integer and pointer types, at most 64 bits.
 * DEBT: This could be enforced with template specializations.
 */

template <class T>
class FastAtomicReader
{
public:
    FastAtomicReader(T data=T()) : m_lockCounter(0), m_data(data), m_exiting(false)
    {
    }

    FastAtomicReader(FastAtomicReader const& o) : m_lockCounter(0), m_data(o.get()), m_exiting(false)
    {
    }

    ~FastAtomicReader()
    {
        /*
         * Not sure exactly what this accomplishes, but the app hangs on track transitions from time to time in set() in
         * the wait loop, with m_lockCounter somehow stuck at a nonzero value, and no other thread around to decrement it.
         * Since it only happens on track transition, I'm hoping this will release any stuck thread.
         */
        m_exiting = true;
        m_lockCounter = 0;

        timespec wait;
        wait.tv_sec = 0;
        wait.tv_nsec = 10;

        nanosleep(&wait, 0);
    }

    T get() const volatile
    {
        /*
         * Current hypothesis regarding the hang in set() when m_lockCounter gets stuck at a nonzero value (usually 1):
         *
         * Two threads (the input thread and the main thread)
         * come into get() at about the same time, properly increment m_lockCounter to 2, then read m_data,
         * and go to decrement m_lockCounter at exactly the same time. First, each reads the same value, 2. Then each
         * decrements 2 and writes 1, which is the value that's left there permanently.
         *
         * The only thing that casts doubt on that hypothesis is that it should be equally likely to happen at just
         * about any point. In this case, it seems to be limited to track transitions. For example, this is happening
         * when updating the static currentReader in CpynMusicPlayerController.mm. DEBT: I wonder if being static
         * exacerbates the issue somehow. Maybe it's only certain of these objects that are problematic for some
         * reason.
         *
         * A solution: Eliminate the while loop in set() and elsewhere, just replacing while with if everywhere. The
         * assumption is that if another thread was previously reading, it must be done after the nanosleep. It might also
         * be necessary to set m_lockCounter to 0 after each nanosleep, or it may never be cleared. Alternately, a loop count could
         * be added, to allow for a longer delay when necessary but also the possibility of waking up sooner.
         */
        ++ m_lockCounter;
        T data = m_data;
        -- m_lockCounter;
        return data;
    }

    FastAtomicReader& set(T data)
    {
        timespec wait;
        wait.tv_sec = 0;
        wait.tv_nsec = 10;

        int loopCount = 0;

        while (m_lockCounter > 0 && loopCount < 5)
        {
            nanosleep(&wait, 0);
            ++ loopCount;
        }

        if (m_exiting) return *this;

        // prevent other writers from colliding, at least
        ++ m_lockCounter;
        m_data = data;
        m_lockCounter = 0;
        return *this;
    }

    void set(T data) volatile
    {
        timespec wait;
        wait.tv_sec = 0;
        wait.tv_nsec = 10;

        int loopCount = 0;

        while (m_lockCounter > 0 && loopCount < 5)
        {
            nanosleep(&wait, 0);
            ++ loopCount;
        }

        if (m_exiting) return *this;

        // prevent other writers from colliding, at least
        ++ m_lockCounter;
        m_data = data;
        m_lockCounter = 0;
        return *this;
    }

    operator T() const volatile { return get(); }

    // atomic read-write operation
    FastAtomicReader& operator ++()
    {
        timespec wait;
        wait.tv_sec = 0;
        wait.tv_nsec = 10;

        int loopCount = 0;

        while (m_lockCounter > 0 && loopCount < 5)
        {
            nanosleep(&wait, 0);
            ++ loopCount;
        }

        if (m_exiting) return *this;

        // prevent other writers from colliding, at least
        ++ m_lockCounter;
        ++ m_data;
        m_lockCounter = 0;
        return *this;
    }

    // atomic read-write operation
    void operator ++() volatile
    {
        timespec wait;
        wait.tv_sec = 0;
        wait.tv_nsec = 10;

        int loopCount = 0;

        while (m_lockCounter > 0 && loopCount < 5)
        {
            nanosleep(&wait, 0);
            ++ loopCount;
        }

        if (m_exiting) return;

        // prevent other writers from colliding, at least
        ++ m_lockCounter;
        ++ m_data;
        m_lockCounter = 0;
    }

    // won't work with floating-point types?
    // atomic read-write operation
    FastAtomicReader& operator+=(int data)
    {
        timespec wait;
        wait.tv_sec = 0;
        wait.tv_nsec = 10;

        int loopCount = 0;

        while (m_lockCounter > 0 && loopCount < 5)
        {
            nanosleep(&wait, 0);
            ++ loopCount;
        }

        if (m_exiting) return *this;

        // prevent other writers from colliding, at least
        ++ m_lockCounter;
        m_data += data;
        m_lockCounter = 0;
        return *this;
    }

    // won't work with floating-point types?
    // atomic read-write operation
    void operator+=(int data) volatile
    {
        timespec wait;
        wait.tv_sec = 0;
        wait.tv_nsec = 10;

        int loopCount = 0;

        while (m_lockCounter > 0 && loopCount < 5)
        {
            nanosleep(&wait, 0);
            ++ loopCount;
        }

        if (m_exiting) return;

        // prevent other writers from colliding, at least
        ++ m_lockCounter;
        m_data += data;
        m_lockCounter = 0;
        return;
    }

    FastAtomicReader& operator=(T data) { set(data); return *this; }
    FastAtomicReader& operator=(FastAtomicReader const& o) { set(o.get()); return *this; }

    void operator=(T data) volatile { set(data); }
    void operator=(FastAtomicReader const& o) volatile { set(o.get()); }

    // won't compile if T is not a pointer type
    T operator->() const volatile { return get(); }

private:
    int volatile mutable m_lockCounter;
    T volatile m_data;
    bool volatile m_exiting;
};

/*
 * This is the flip side of the class above. It's used for the m_offset member variable in the AudioFileReader
 * class. This variable is set by the read() member function in the real-time input callback thread, so any
 * locking or delay when writing is unacceptable. It is read in other threads, usually the main one, to return
 * a value for [CpynMusicPlayerController musicPlayer].currentPlaybackTime. This can afford a delay of some
 * ns.
 */
template <class T>
class FastAtomicWriter
{
public:
    FastAtomicWriter(T data=T()) : m_lockCounter(0), m_data(data), m_exiting(false)
    {
    }

    ~FastAtomicWriter()
    {
        /*
         * Not sure exactly what this accomplishes, but the app hangs on track transitions from time to time in FAR::set() in
         * the wait loop, with m_lockCounter somehow stuck at a nonzero value, and no other thread around to decrement it.
         * Assuming it can happen here too, and since it only happens on track transition, I'm hoping this will release any stuck thread.
         */
        m_exiting = true;
        m_lockCounter = 0;

        timespec wait;
        wait.tv_sec = 0;
        wait.tv_nsec = 10;

        nanosleep(&wait, 0);
    }

    FastAtomicWriter(FastAtomicWriter const& o) : m_lockCounter(0), m_data(o.get()), m_exiting(false)
    {
    }

    T get() const volatile
    {
        timespec wait;
        wait.tv_sec = 0;
        wait.tv_nsec = 10;

        int loopCount = 0;

        while (m_lockCounter > 0 && loopCount < 5)
        {
            nanosleep(&wait, 0);
            ++ loopCount;
        }

        // DEBT: Throw an exception? This should be OK.
        if (m_exiting) return T(); // don't try to read from this address space again

        m_lockCounter = 0;

        return m_data;
    }

    FastAtomicWriter& set(T data)
    {
        ++ m_lockCounter;
        m_data = data;
        -- m_lockCounter;
        return *this;
    }

    void set(T data) volatile
    {
        ++ m_lockCounter;
        m_data = data;
        -- m_lockCounter;
    }

    operator T() const volatile { return get(); }

    FastAtomicWriter& operator=(T data) { set(data); return *this; }
    FastAtomicWriter& operator=(FastAtomicWriter const& o) { set(o.get()); return *this; }
    FastAtomicWriter& operator ++()
    {
        T data = get();
        return set(++data);
    }

    void operator=(T data) volatile { set(data); }
    void operator=(FastAtomicWriter const& o) volatile { set(o.get()); }
    void operator ++() volatile
    {
        T data = get();
        set(++data);
    }

    // won't work with floating-point types?
    // read-write operation
    FastAtomicWriter& operator+=(int data) { set(get() + data); return *this; }
    void operator+=(int data) volatile { set(get() + data); }

    // won't compile if T is not a pointer type
    T operator->() const volatile { return get(); }

private:
    int volatile mutable m_lockCounter;
    T volatile m_data;
    bool volatile m_exiting;
};

}

#endif
