#include "audio.hpp"
#include "collision.hpp"
#include "full_heal_block_sprite.hpp"
#include "health.hpp"
#include "level_state.hpp"
#include "sprite_graphics.hpp"

FullHealBlockSprite::FullHealBlockSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/universal.png", 112, 16 ), x, y, 16, 16, {}, 0, 0, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

FullHealBlockSprite::~FullHealBlockSprite() {};

void FullHealBlockSprite::customUpdate( LevelState& level_state )
{
	hit_box_.x = original_hit_box_.x; // Undo wind.
	graphics_->flip_y_ = isUpsideDown();
	bump_under_block_component.update( *this );
};

void FullHealBlockSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.block_interact_ )
	{
		const Collision collision = them.testBlockCollision( *this );
		const bool hit = bump_under_block_component.testHit( *this, them, collision );
		if ( hit )
		{
			if ( them.hasType( Sprite::SpriteType::HERO ) )
			{
				Health& health = level_state.health();
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
			else
			{
				Audio::playSound( Audio::SoundType::BUMP );
			}
		}
	}
};
