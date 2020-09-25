#pragma once

class LevelState;

class Renderable
{
    public:
        Renderable();
        virtual ~Renderable();
        virtual void render( const LevelState& level_state ) const = 0;
};