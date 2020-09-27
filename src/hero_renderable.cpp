#include "level_state.hpp"
#include "hero_renderable.hpp"
#include "sprite_system.hpp"

HeroRenderable::HeroRenderable() : Renderable( Type::HERO ) {};
HeroRenderable::~HeroRenderable() {};

void HeroRenderable::render( const LevelState& level_state ) const
{
    level_state.sprites().renderHero( level_state );
};