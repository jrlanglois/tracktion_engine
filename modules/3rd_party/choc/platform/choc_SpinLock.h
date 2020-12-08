/*
    ██████ ██   ██  ██████   ██████
   ██      ██   ██ ██    ██ ██         Clean Header-Only Classes
   ██      ███████ ██    ██ ██         Copyright (C)2020 Julian Storer
   ██      ██   ██ ██    ██ ██
    ██████ ██   ██  ██████   ██████    https://github.com/julianstorer/choc

   This file is part of the CHOC C++ collection - see the github page to find out more.

   The code in this file is provided under the terms of the ISC license:

   Permission to use, copy, modify, and/or distribute this software for any purpose with
   or without fee is hereby granted, provided that the above copyright notice and this
   permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO
   THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT
   SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR
   ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF
   CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE
   OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef CHOC_SPINLOCK_HEADER_INCLUDED
#define CHOC_SPINLOCK_HEADER_INCLUDED

#include <atomic>

namespace choc::threading
{

//==============================================================================
/**
    A minimal no-frills spin-lock.

    To use an RAII pattern for locking a SpinLock, it's compatible with the normal
    std::lock_guard class.
*/
struct SpinLock
{
    SpinLock() = default;
    ~SpinLock() = default;

    void lock();
    bool try_lock();
    void unlock();

private:
    std::atomic_flag flag = ATOMIC_FLAG_INIT;
};


//==============================================================================
//        _        _           _  _
//     __| |  ___ | |_   __ _ (_)| | ___
//    / _` | / _ \| __| / _` || || |/ __|
//   | (_| ||  __/| |_ | (_| || || |\__ \ _  _  _
//    \__,_| \___| \__| \__,_||_||_||___/(_)(_)(_)
//
//   Code beyond this point is implementation detail...
//
//==============================================================================

inline void SpinLock::lock()     { while (flag.test_and_set (std::memory_order_acquire)) {} }
inline bool SpinLock::try_lock() { return ! flag.test_and_set (std::memory_order_acquire); }
inline void SpinLock::unlock()   { flag.clear(); }

} // choc::fifo

#endif
