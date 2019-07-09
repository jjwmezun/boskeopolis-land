#include "jump_and_shoot_sprite.hpp"
#include "octopig_sprite.hpp"
#include "simple_projectile_sprite.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

std::unique_ptr<Sprite> generateOctopigSprite( int x, int y )
{
	return std::unique_ptr<Sprite>
	(
		new JumpAndShootSprite
		(
			x, y, 14, 14,
			std::make_unique<SpriteGraphics> ( "sprites/octopig.png", 0, 0, false, false, 0, false, -1, -1, 2, 2 ),
			[]( const Sprite& me, SpriteSystem& sprites )
			{
				sprites.spawn
				(
					std::make_unique<SimpleProjectileSprite>
					(
						me.centerXPixels(), me.centerYPixels(),
						2, 2,
						me.direction_x_,
						std::make_unique<SpriteGraphics> ( "sprites/octopig.png", 16, 0, false, false, 0, false, -1, -1, 2, 2 )
					)
				);
			}
		)
	);
};
