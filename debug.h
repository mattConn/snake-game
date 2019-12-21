#pragma once
#include <iostream>

// debug msg printing
#define DEBUG

#ifdef DEBUG
#define DEBUG_MSG(str) do { std::cout << str << std::endl; } while( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

// debug rectangle intersection
//#define DEBUG_INTERSECTION
