#ifndef DF3_PANIC
#define DF3_PANIC

// built-in
#include <iostream>

inline void panic(const char* const message)
{
    std::cout << message << std::endl;
    exit(1);
}

#endif