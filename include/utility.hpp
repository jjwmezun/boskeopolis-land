#ifndef UTILITY_H
#define UTILITY_H

#include "arg.hpp"

namespace BSL
{
    void memcpyrect
    (
        unsigned char * dest,
        const unsigned char * src,
        unsigned int w,
        unsigned int h,
        ArgList args = {}
    );
}

#endif // UTILITY_H