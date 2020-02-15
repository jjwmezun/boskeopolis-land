#pragma once

#include "level_graphics.hpp"
#include <unordered_map>
#include <vector>

class Camera;

class LevelGraphicsSystem
{
    public:
        LevelGraphicsSystem();
        void render( const Camera* camera ) const;

    private:
		std::vector<LevelGraphics> graphics_;
        std::unordered_map<int, LevelGraphics*> graphics_lookup_;
        int latest_index_;
};