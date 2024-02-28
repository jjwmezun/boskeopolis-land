#ifndef DICT_H
#define DICT_H

#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

namespace BSL
{
    template <typename A>
    struct Dict
    {
        void add( const std::string & key, A value )
        {
            values_.push_back( { key, value } );
        }

        A get( const std::string & key ) const
        {
            for ( auto & value : values_ )
            {
                if ( value.first == key )
                {
                    return value.second;
                }
            }
            throw std::runtime_error( "Could not find Dict entry for key “" + key + "”." );
        }

        std::optional<A> getOptional( const std::string & key ) const
        {
            for ( auto & value : values_ )
            {
                if ( value.first == key )
                {
                    return value.second;
                }
            }
            return {};
        }

        std::vector<std::pair<std::string, A>> values_;
    };
}

#endif // DICT_H