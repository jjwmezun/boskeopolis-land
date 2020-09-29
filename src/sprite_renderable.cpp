#include "level_state.hpp"
#include "sprite_renderable.hpp"
#include "sprite_system.hpp"

SpriteRenderable::SpriteRenderable( int sprite_id ) : Renderable( Type::SPRITE ), sprite_id_ ( sprite_id ) {};
SpriteRenderable::~SpriteRenderable() {};

void SpriteRenderable::render( const LevelState& level_state ) const
{
    level_state.sprites().renderSprite( sprite_id_, level_state );
};