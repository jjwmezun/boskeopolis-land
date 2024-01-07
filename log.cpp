#include "log.hpp"
#include <cstdarg>
#include <cstdio>

namespace BSL
{
    void log( const char * format, ... )
    {
        //#ifdef NASR_DEBUG
            va_list arg;
            va_start( arg, format );
            vfprintf( stderr, format, arg );
            va_end( arg );
            fprintf( stderr, "\n" );
        //#endif
    };
}