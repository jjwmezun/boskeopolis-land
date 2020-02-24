#include "animated_graphics.hpp"
#include "collision.hpp"
#include "downhill_gem_sprite.hpp"
#include "inventory_level.hpp"
#include "level_state.hpp"

DownhillGemSprite::DownhillGemSprite( int x, int y )
:
	Sprite( std::make_unique<AnimatedGraphics> ( "tilesets/universal.png", std::vector<std::pair<int, int>>( { std::make_pair( 3, 64 ), std::make_pair( 19, 64 ), std::make_pair( 35, 64 ), std::make_pair( 51, 64 ), std::make_pair( 67, 64 ), std::make_pair( 83, 64 ) } ) ), x + 3, y, 10, 16, {}, 2000, 2000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::DOWN, nullptr, SpriteMovement::Type::GROUNDED )
{};

DownhillGemSprite::~DownhillGemSprite() {};

void DownhillGemSprite::customUpdate( LevelState& level_state )
{
	moveLeft();
	graphics_->rotation_ -= 3;
};

void DownhillGemSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) && their_collision.collideAny() )
	{
		level_state.inventory().addFunds( 100 );
	}
};