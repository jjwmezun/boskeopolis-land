#include "audio.hpp"
#include "health.hpp"
#include "sprite_graphics.hpp"
#include "full_heal_block_sprite.hpp"

FullHealBlockSprite::FullHealBlockSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/universal.png", 112, 16 ), x, y, 16, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

FullHealBlockSprite::~FullHealBlockSprite() {};

void FullHealBlockSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	hit_box_.x = original_hit_box_.x; // Undo wind.
	bump_under_block_component.update( *this );
};

void FullHealBlockSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	const bool hit = bump_under_block_component.testHit( *this, them, their_collision );
	if ( hit )
	{
		if ( health.hasFullHealth() )
		{
			Audio::playSound( Audio::SoundType::BUMP );
		}
		else
		{
			health.fullHeal();
			Audio::playSound( Audio::SoundType::HEAL );
		}
	}
};
