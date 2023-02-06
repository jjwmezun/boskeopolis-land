#ifndef MAP_H
#define MAP_H

#include <vector>

namespace BSL
{
    class Map
    {
        public:
            Map();
            bool testCollision( unsigned int x, unsigned int y ) const;
            constexpr int getIFromXAndY( unsigned int x, unsigned int y ) const
            {
                return y * width_ + x;
            };

        private:
            unsigned int width_;
            unsigned int height_;
            std::vector<bool> collision_;
    };
}

#endif // MAP_H