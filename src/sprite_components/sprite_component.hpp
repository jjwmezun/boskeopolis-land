#pragma once

class LevelState;
class Sprite;

class SpriteComponent
{
    public:
        SpriteComponent();
        virtual ~SpriteComponent();
        virtual void update( Sprite& sprite, LevelState& state ) = 0;
        virtual void reset();
};
