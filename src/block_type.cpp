#include "block_type.hpp"

namespace BSL
{
    BlockType::BlockType()
        : is_null_ ( true ), x_ ( 0 ), y_ ( 0 ), animation_ ( 0 ) {};

    BlockType::BlockType( unsigned char x, unsigned char y, unsigned char animation )
        : is_null_ ( false ), x_ ( x ), y_ ( y ), animation_ ( animation ) {};

    bool BlockType::isNull() const
    {
        return is_null_;
    };

    unsigned char BlockType::getX() const { return x_; };
    unsigned char BlockType::getY() const { return y_; };
    unsigned char BlockType::getAnimation() const { return animation_; };
}