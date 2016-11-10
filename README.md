# Experimental

These are my small experiments with C++ language and constructs.

## Templates

- **count_indirections** : indirection counter for pointers (also known as "star counter"); also works with smart pointers.
- **count_elements** : counter of elements of a certain value in small constant compile-time arrays.
- **binary_search** : recursive binary search algorithm implemented with template metaprogramming for static and dynamic arrays of a known size.

## Coroutine

**Stackful coroutine** implementation for 32-bit code. This implementation also requires MASM compiler and frame pointer optimization disabled (in MSVC, it uses a MS-specific pragma to disable it locally). Coroutine is a template class, and can return different types of values. The return value should be either passed to yield(), or returned normally at the end of the coroutine function. If coroutine is finished, subsequent run calls return a default-constructed value. If enableRestart() is set to true, coroutine will restart from the very beginning instead (but this call doesn't affect a finished coroutine). There is also a specialized coroutine version that doesn't return a value. See examples in the cpp file for the details. [Learn more about coroutines...](https://en.wikipedia.org/wiki/Coroutine)

More experiments will be added, stay tuned!