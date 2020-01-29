#pragma once

#include <cassert>

#define DEBUG true

#if DEBUG
    #define jassert( t ) ( assert( ( t ) ) )
#else
    #define jassert( t ) ()
#endif