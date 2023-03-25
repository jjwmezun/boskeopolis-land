#ifndef BLOCK_TYPE_H
#define BLOCK_TYPE_H

namespace BSL
{
    class BlockType
    {
        public:
            BlockType();
            BlockType( unsigned char x, unsigned char y, unsigned char animation );
            bool isNull() const;
            unsigned char getX() const;
            unsigned char getY() const;
            unsigned char getAnimation() const;

        private:
            bool is_null_;
            unsigned char x_;
            unsigned char y_;
            unsigned char animation_;
    };
}

#endif // BLOCK_TYPE_H