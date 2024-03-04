#ifndef LIST_H
#define LIST_H

#include <cstdint>

template <typename A>
struct List
{
    void init( uint_fast16_t init_capacity )
    {
        capacity = init_capacity;
        count = 0;
        data = static_cast<A *> ( calloc( capacity, sizeof( A ) ) );
    };

    void close()
    {
        free( data );
    };

    void push( A item )
    {
        testAllocation();

        // Set next empty space & then update count.
        data[ count++ ] = item;
    };

    void pushEmpty()
    {
        testAllocation();
        ++count;
    };

    A & pushEmptyGet()
    {
        testAllocation();
        return data[ count++ ];
    };

    A pop()
    {
        // Create copy o’ last entry & remove from list.
        A temp = data[ --count ];

        // Make sure we clear last entry.
        memset( &data[ count ], 0, sizeof( A ) );

        return temp;
    };

    void testAllocation()
    {
        // If already full, double capacity for mo’ room.
        if ( count == capacity )
        {
            capacity *= 2;

            // Allocate new space with mo’ room.
            A * new_data = static_cast<A *> ( calloc( capacity, sizeof( A ) ) );

            // Copy ol’ data o’er to new space.
            memcpy( new_data, data, count * sizeof( A ) );

            // Release ol’ space.
            free( data );

            // Set data to use new space.
            data = new_data;
        }
    };

    uint_fast16_t capacity;
    uint_fast16_t count;
    A * data;
};

#endif // LIST_H