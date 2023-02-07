#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>

namespace BSL
{
    class Map
    {
        public:
            enum class CollisionType
            {
                NONE = 0,
                SOLID = 1,
                SOLID_ABOVE = 2
            };

            Map( std::string && slug );
            unsigned int getWidthPixels() const;
            unsigned int getHeightPixels() const;
            bool testCollision( unsigned int x, unsigned int y, std::vector<CollisionType> types = { CollisionType::SOLID } ) const;
            constexpr int getIFromXAndY( unsigned int x, unsigned int y ) const
            {
                return y * width_ + x;
            };

        private:
            unsigned int width_;
            unsigned int height_;
            std::vector<std::vector<int>> collision_;
    };
}

#endif // MAP_H