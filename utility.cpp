#include <cstring>
#include "utility.hpp"

namespace BSL
{
    void memcpyrect
    (
        unsigned char * dest,
        const unsigned char * src,
        unsigned int w,
        unsigned int h,
        ArgList args
    )
    {
        const size_t size = GetArg( "blocksize", args, 1 );
        const unsigned int x = GetArg( "x", args, 0u );
        const unsigned int y = GetArg( "y", args, 0u );
        for ( unsigned int i = y; i < h; ++i )
        {
            memcpy( &dest[ w * ( y + i ) + x ], &src, w * size );
        }
    };
}