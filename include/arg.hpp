#ifndef ARG_H
#define ARG_H

#include <string>
#include <variant>
#include <unordered_map>

namespace BSL
{
    typedef std::variant<bool, int, unsigned int, float, std::string> Arg;
    typedef std::unordered_map<std::string, Arg> ArgList;

    template<typename T>
    T GetArg( const char * key, ArgList args, T backup )
    {
        auto it = args.find( key );
        return ( it != args.end() )
            ? static_cast<T> ( std::get<T>( it->second ) )
            : backup;
    }

    template<typename T, typename U>
    T GetArgConvert( const char * key, ArgList args, T backup )
    {
        auto it = args.find( key );
        return ( it != args.end() )
            ? static_cast<T> ( std::get<U>( it->second ) )
            : backup;
    }

    template<typename T, typename U>
    T GetArgConvert( const char * key, ArgList args, U backup )
    {
        auto it = args.find( key );
        return ( it != args.end() )
            ? static_cast<T> ( std::get<U>( it->second ) )
            : static_cast<T> ( backup );
    }
}

#endif // ARG_H