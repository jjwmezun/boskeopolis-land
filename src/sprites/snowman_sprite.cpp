#include "jump_and_shoot_sprite.hpp"
#include "simple_projectile_sprite.hpp"
#include "snowman_sprite.hpp"
#include "sprite_graphics.hpp"
#include "sprite_system.hpp"

std::unique_ptr<Sprite> generateSnowmanSprite( int x, int y )
{
	return std::unique_ptr<Sprite>
	(
		new JumpAndShootSprite
		(
			x, y, 15, 22,
			std::make_unique<SpriteGraphics> ( "sprites/snowman.png", 8, 0, false, false, 0, -1, -2, 2, 4 ),
			[]( const Sprite& me, SpriteSystem& sprites )
			{
				sprites.spawn
				(
					std::make_unique<SimpleProjectileSprite>
					(
						me.centerXPixels(), me.yPixels() + 8,
						6, 6,
						me.direction_x_,
						std::make_unique<SpriteGraphics> ( "sprites/snowman.png", 0, 0, false, false, 0, -1, -1, 2, 2 )
					)
				);
			}
		)
	);
};
