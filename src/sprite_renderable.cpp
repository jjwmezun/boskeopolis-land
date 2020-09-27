#include "level_state.hpp"
#include "sprite_renderable.hpp"
#include "sprite_system.hpp"

SpriteRenderable::SpriteRenderable( int id ) : Renderable( Type::SPRITE ), id_ ( id ) {};
SpriteRenderable::~SpriteRenderable() {};

void SpriteRenderable::render( const LevelState& level_state ) const
{
    level_state.sprites().renderSprite( id_, level_state );
};