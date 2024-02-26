#ifndef BSL_JSON_H
#define BSL_JSON_H

#include <functional>
#include "json/json.h"
#include <string>

namespace BSL
{
    class JSONArray;
    class JSONItem;
    class JSONObject;

    class JSONArray
    {
        public:
            inline JSONArray
            (
                uint_fast16_t length,
                struct _json_value ** values
            ) :
                length_ ( length ),
                values_ ( values )
            {};
            void forEach( const std::function<void( JSONItem )> & callable ) const;
            void forEach( const std::function<void( JSONItem, uint_fast16_t )> & callable ) const;
            uint_fast16_t getLength() const;

        private:
            uint_fast16_t length_;
            struct _json_value ** values_;
    };

    class JSONItem
    {
        public:
            inline JSONItem( const json_value * value ) : value_ ( value ) {};
            int asInt() const;
            std::string asString() const;
            JSONObject asObject() const;

        private:
            const json_value * value_;
    };

    class JSONObject
    {
        public:
            inline JSONObject( const json_value * data ) : data_ ( data ) {};
            int getInt( const std::string & name ) const;
            float getFloat( const std::string & name ) const;
            bool getBool( const std::string & name ) const;
            std::string getString( const std::string & name ) const;
            JSONArray getArray( const std::string & name ) const;
            JSONObject getObject( const std::string & name ) const;
            JSONItem getItem( const std::string & name ) const;
            bool hasArray( const std::string & name ) const;
            bool hasInt( const std::string & name ) const;
            bool hasFloat( const std::string & name ) const;
            bool hasBool( const std::string & name ) const;
            bool hasString( const std::string & name ) const;
            bool hasObject( const std::string & name ) const;
            int getIntOptional( const std::string & name, int fallback ) const;
            bool getBoolOptional( const std::string & name, bool fallback ) const;
            float getFloatOptional( const std::string & name, float fallback ) const;
            std::string getStringOptional( const std::string & name, const std::string & fallback ) const;

        private:
            const json_value * data_;
            bool hasType( const std::string & name, json_type type ) const;
    };

    class JSON
    {
        public:
            JSON( std::string&& filename );
            ~JSON();
            bool hasInt( const std::string & name ) const;
            bool hasFloat( const std::string & name ) const;
            int getInt( const std::string & name ) const;
            float getFloat( const std::string & name ) const;
            std::string getString( const std::string & name ) const;
            JSONArray getArray( const std::string & name ) const;

        private:
            std::string filename_;
            json_value * data_;
    };

    template<typename T>
    std::vector<T> JSONMap( const JSONArray & list, std::function<T( const JSONItem & )> callable )
    {
        std::vector<T> out;
        list.forEach( [&]( const JSONItem & i )
        {
            out.push_back( callable( i ) );
        });
        return out;
    };
}

#endif // BSL_JSON_H