
#include <cstring>
#include "json.hpp"
#include <stdexcept>
#include "io.hpp"

namespace BSL
{
    void JSONArray::forEach( const std::function<void( JSONItem )> & callable ) const
    {
        for ( uint_fast16_t i = 0; i < length_; ++i )
        {
            callable( { values_[ i ] } );
        }
    };

    void JSONArray::forEach( const std::function<void( JSONItem, uint_fast16_t )> & callable ) const
    {
        for ( uint_fast16_t i = 0; i < length_; ++i )
        {
            callable( { values_[ i ] }, i );
        }
    };

    uint_fast16_t JSONArray::getLength() const
    {
        return length_;
    };

    int JSONItem::asInt() const
    {
        if ( value_->type != json_integer )
        {
            throw std::runtime_error( "JSON Item not expected type integer." );
        }
        return value_->u.integer;
    };

    float JSONItem::asFloat() const
    {
        if ( value_->type != json_double )
        {
            throw std::runtime_error( "JSON Item not expected type float." );
        }
        return value_->u.dbl;
    };

    std::string JSONItem::asString() const
    {
        if ( value_->type != json_string )
        {
            throw std::runtime_error( "JSON Item not expected type string." );
        }
        return std::string( value_->u.string.ptr );
    };

    JSONObject JSONItem::asObject() const
    {
        if ( value_->type != json_object )
        {
            throw std::runtime_error( "JSON Item not expected type object." );
        }
        return { value_ };
    };

    int JSONObject::getInt( const std::string & name ) const
    {
        for ( uint_fast16_t i = 0; i < data_->u.object.length; ++i )
        {
            const json_object_entry entry = data_->u.object.values[ i ];
            if ( std::strcmp( name.c_str(), entry.name ) == 0 )
            {
                if ( entry.value->type != json_integer )
                {
                    throw std::runtime_error( "JSON value “" + name + "” is not an integer." );
                }
                return entry.value->u.integer;
            }
        }
        throw std::runtime_error( "JSON file missing “" + name + "”." );
    };

    float JSONObject::getFloat( const std::string & name ) const
    {
        for ( uint_fast16_t i = 0; i < data_->u.object.length; ++i )
        {
            const json_object_entry entry = data_->u.object.values[ i ];
            if ( std::strcmp( name.c_str(), entry.name ) == 0 )
            {
                if ( entry.value->type != json_double )
                {
                    throw std::runtime_error( "JSON value “" + name + "” is not a float, but is a " + std::to_string( entry.value->type ) + "." );
                }
                return entry.value->u.dbl;
            }
        }
        throw std::runtime_error( "JSON file missing “" + name + "”." );
    };

    bool JSONObject::getBool( const std::string & name ) const
    {
        for ( uint_fast16_t i = 0; i < data_->u.object.length; ++i )
        {
            const json_object_entry entry = data_->u.object.values[ i ];
            if ( std::strcmp( name.c_str(), entry.name ) == 0 )
            {
                if ( entry.value->type != json_boolean )
                {
                    throw std::runtime_error( "JSON value “" + name + "” is not a boolean, but is a " + std::to_string( entry.value->type ) + "." );
                }
                return entry.value->u.boolean;
            }
        }
        throw std::runtime_error( "JSON file missing “" + name + "”." );
    };

    std::string JSONObject::getString( const std::string & name ) const
    {
        for ( uint_fast16_t i = 0; i < data_->u.object.length; ++i )
        {
            const json_object_entry entry = data_->u.object.values[ i ];
            if ( std::strcmp( name.c_str(), entry.name ) == 0 )
            {
                if ( entry.value->type != json_string )
                {
                    throw std::runtime_error( "JSON file missing “" + name + "”." );
                }
                return std::string( entry.value->u.string.ptr );
            }
        }
        throw std::runtime_error( "JSON file missing “" + name + "”." );
    };

    int JSONObject::getIntOptional( const std::string & name, int fallback ) const
    {
        for ( uint_fast16_t i = 0; i < data_->u.object.length; ++i )
        {
            const json_object_entry entry = data_->u.object.values[ i ];
            if ( std::strcmp( name.c_str(), entry.name ) == 0 )
            {
                if ( entry.value->type != json_integer )
                {
                    throw std::runtime_error( "JSON value “" + name + "” is not an integer." );
                }
                return entry.value->u.integer;
            }
        }
        return fallback;
    };

    bool JSONObject::getBoolOptional( const std::string & name, bool fallback ) const
    {
        for ( uint_fast16_t i = 0; i < data_->u.object.length; ++i )
        {
            const json_object_entry entry = data_->u.object.values[ i ];
            if ( std::strcmp( name.c_str(), entry.name ) == 0 )
            {
                if ( entry.value->type != json_boolean )
                {
                    throw std::runtime_error( "JSON value “" + name + "” is not a boolean, but is a " + std::to_string( entry.value->type ) + "." );
                }
                return entry.value->u.boolean;
            }
        }
        return fallback;
    };

    float JSONObject::getFloatOptional( const std::string & name, float fallback ) const
    {
        for ( uint_fast16_t i = 0; i < data_->u.object.length; ++i )
        {
            const json_object_entry entry = data_->u.object.values[ i ];
            if ( std::strcmp( name.c_str(), entry.name ) == 0 )
            {
                if ( entry.value->type != json_double )
                {
                    throw std::runtime_error( "JSON value “" + name + "” is not a float, but is a " + std::to_string( entry.value->type ) + "." );
                }
                return entry.value->u.dbl;
            }
        }
        return fallback;
    };

    std::string JSONObject::getStringOptional( const std::string & name, const std::string & fallback ) const
    {
        for ( uint_fast16_t i = 0; i < data_->u.object.length; ++i )
        {
            const json_object_entry entry = data_->u.object.values[ i ];
            if ( std::strcmp( name.c_str(), entry.name ) == 0 )
            {
                if ( entry.value->type != json_string )
                {
                    throw std::runtime_error( "JSON file missing “" + name + "”." );
                }
                return std::string( entry.value->u.string.ptr );
            }
        }
        return fallback;
    };

    JSONArray JSONObject::getArray( const std::string & name ) const
    {
        for ( uint_fast16_t i = 0; i < data_->u.object.length; ++i )
        {
            const json_object_entry entry = data_->u.object.values[ i ];
            if ( std::strcmp( name.c_str(), entry.name ) == 0 )
            {
                if ( entry.value->type != json_array )
                {
                    throw std::runtime_error( "JSON file missing “" + name + "”." );
                }
                return { entry.value->u.array.length, entry.value->u.array.values };
            }
        }
        throw std::runtime_error( "JSON file missing “" + name + "”." );
    };

    JSONObject JSONObject::getObject( const std::string & name ) const
    {
        for ( uint_fast16_t i = 0; i < data_->u.object.length; ++i )
        {
            const json_object_entry entry = data_->u.object.values[ i ];
            if ( std::strcmp( name.c_str(), entry.name ) == 0 )
            {
                if ( entry.value->type != json_object )
                {
                    throw std::runtime_error( "JSON file missing “" + name + "”." );
                }
                return { entry.value };
            }
        }
        throw std::runtime_error( "JSON file missing “" + name + "”." );
    };

    JSONItem JSONObject::getItem( const std::string & name ) const
    {
        for ( uint_fast16_t i = 0; i < data_->u.object.length; ++i )
        {
            const json_object_entry entry = data_->u.object.values[ i ];
            if ( std::strcmp( name.c_str(), entry.name ) == 0 )
            {
                return { entry.value };
            }
        }
        throw std::runtime_error( "JSON file missing “" + name + "”." );
    };

    bool JSONObject::hasArray( const std::string & name ) const
    {
        return hasType( name, json_array );
    };

    bool JSONObject::hasInt( const std::string & name ) const
    {
        return hasType( name, json_integer );
    };

    bool JSONObject::hasFloat( const std::string & name ) const
    {
        return hasType( name, json_double );
    };

    bool JSONObject::hasBool( const std::string & name ) const
    {
        return hasType( name, json_boolean );
    };

    bool JSONObject::hasString( const std::string & name ) const
    {
        return hasType( name, json_string );
    };

    bool JSONObject::hasObject( const std::string & name ) const
    {
        return hasType( name, json_object );
    };

    bool JSONObject::hasType( const std::string & name, json_type type ) const
    {
        for ( uint_fast16_t i = 0; i < data_->u.object.length; ++i )
        {
            const json_object_entry entry = data_->u.object.values[ i ];
            if ( std::strcmp( name.c_str(), entry.name ) == 0 )
            {
                return entry.value->type == type;
            }
        }
        return false;
    };

    JSON::JSON( std::string&& filename ) : filename_ ( std::move( filename ) )
    {
        char * content = openFile( filename_.c_str() );
        if ( !content )
        {
            throw std::runtime_error( "JSON file " + filename_ + " not found." );
        }
        json_char * json = ( json_char * )( content );
        data_ = json_parse( json, strlen( content ) + 1 );
        free( content );
        if ( !data_ )
        {
            throw std::runtime_error( "JSON file " + filename_ + " not valid JSON." );
        }
        if ( data_->type != json_object || !data_->u.object.length )
        {
            json_value_free( data_ );
            data_ = nullptr;
            throw std::runtime_error( "JSON file " + filename_ + " not valid JSON." );
        }
    };

    JSON::~JSON()
    {
        if ( data_ )
        {
            json_value_free( data_ );
        }
    };

    bool JSON::hasInt( const std::string & name ) const
    {
        JSONObject o { data_ };
        return o.hasInt( name );
    };

    int JSON::getInt( const std::string & name ) const
    {
        JSONObject o { data_ };
        return o.getInt( name );
    };

    bool JSON::hasFloat( const std::string & name ) const
    {
        JSONObject o { data_ };
        return o.hasFloat( name );
    };

    float JSON::getFloat( const std::string & name ) const
    {
        JSONObject o { data_ };
        return o.getFloat( name );
    };

    std::string JSON::getString( const std::string & name ) const
    {
        JSONObject o { data_ };
        return o.getString( name );
    };

    JSONArray JSON::getArray( const std::string & name ) const
    {
        JSONObject o { data_ };
        return o.getArray( name );
    };
}