#include "dict.hpp"
#include "json.hpp"
#include "object_factory.hpp"

#include <cstdio>

namespace BSL::ObjectFactory
{
    static constexpr uint_fast16_t MAX_OBJECTS = 64;

    static ObjectTemplate objects[ MAX_OBJECTS ];

    static uint_fast8_t getOptionalPropValue( const Dict<JSONItem> & props, std::string && type );

    std::optional<ObjectTemplate> get( uint_fast16_t n )
    {
        return ( n > MAX_OBJECTS || objects[ n ].type == ObjectType::__NULL )
            ? std::nullopt
            : std::optional<ObjectTemplate>{ objects[ n ] };
    };

    void init()
    {
        JSON json { "assets/tilesets/objects.json" };
        JSONArray list = json.getArray( "tiles" );
        list.forEach
        (
            [&] ( const JSONItem item )
            {
                const JSONObject obj = item.asObject();
                const uint_fast16_t id = obj.getInt( "id" );
                if ( id >= MAX_OBJECTS )
                {
                    return;
                }

                // Generate hash map from props.
                Dict<JSONItem> props;
                const JSONArray propslist = obj.getArray( "properties" );
                propslist.forEach
                (
                    [&] ( JSONItem propitem )
                    {
                        const JSONObject propobj = propitem.asObject();
                        const std::string key = propobj.getString( "name" );
                        const JSONItem value = propobj.getItem( "value" );
                        props.add( key, value );
                    }
                );

                // Gather tile data.
                objects[ id ].tile.x = getOptionalPropValue( props, "x" );
                objects[ id ].tile.y = getOptionalPropValue( props, "y" );
                objects[ id ].tile.animation = getOptionalPropValue( props, "animation" );

                // Gather type-specific data.
                const std::string type = props.get( "type" ).asString();
                if ( type == "money" )
                {
                    objects[ id ].type = ObjectType::MONEY;
                    objects[ id ].data.money.amount = props.get( "amount" ).asInt();
                }
            }
        );
    };

    static uint_fast8_t getOptionalPropValue( const Dict<JSONItem> & props, std::string && type )
    {
        uint_fast8_t v = 0;
        auto vi = props.getOptional( type );
        if ( vi.has_value() )
        {
            v = vi.value().asInt();
        }
        return v;
    };
}