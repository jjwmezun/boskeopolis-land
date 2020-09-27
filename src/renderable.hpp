#pragma once

class LevelState;

class Renderable
{
    public:
        enum class Type
        {
            BLOCK,
            HERO,
            SPRITE,
            MAP_LAYER
        };
        Renderable( Type type );
        virtual ~Renderable();
        virtual void render( const LevelState& level_state ) const = 0;
        int id_;
        Type type_;
};