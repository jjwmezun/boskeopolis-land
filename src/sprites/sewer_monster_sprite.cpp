#include "collision.hpp"
#include "level_state.hpp"
#include "sewer_monster_sprite.hpp"
#include "sprite_graphics.hpp"

constexpr int SewerMonsterSprite::WAKING_FRAMES[];
constexpr int SewerMonsterSprite::FALLING_ASLEEP_FRAMES[];

SewerMonsterSprite::SewerMonsterSprite( int x, int y )
:
	Sprite
	(
		std::make_unique<SpriteGraphics> ( "sprites/sewer_monster.png", 0, 0, false, false, 0, -16, -16, 32, 32 ),
		x+16,
		y+16,
		48,
		48,
		{},
		100,
		1500,
		3000,
		0,
		Direction::Horizontal::__NULL,
		Direction::Vertical::__NULL,
		nullptr,
		SpriteMovement::Type::FLOATING,
		CameraMovement::RESET_OFFSCREEN_AND_AWAY,
		false,
		false,
		true,
		true
	),
	state_ ( MonsterState::SLEEPING ),
	waking_timer_ (),
	attack_timer_ (),
	falling_asleep_timer_ ()
{};

SewerMonsterSprite::~SewerMonsterSprite() {};

void SewerMonsterSprite::stateGraphics( LevelState& level_state )
{
	switch( state_ )
	{
		case ( MonsterState::SLEEPING ):
			graphics_->current_frame_x_ = 0;
			level_state.changeRenderableLayer( renderable_id_, Unit::Layer::SPRITES_1 );
		break;

		case ( MonsterState::WAKING ):
			graphics_->current_frame_x_ = getXImg( WAKING_FRAMES[ getFrame( waking_timer_.counter() ) ] );
			level_state.changeRenderableLayer( renderable_id_, Unit::Layer::SPRITES_2 );
		break;

		case ( MonsterState::ATTACK ):
			graphics_->current_frame_x_ = 10*80;
			level_state.changeRenderableLayer( renderable_id_, Unit::Layer::SPRITES_2 );
		break;

		case ( MonsterState::FALLING_ASLEEP ):
			graphics_->current_frame_x_ = getXImg( FALLING_ASLEEP_FRAMES[ getFrame( falling_asleep_timer_.counter() ) ] );
			level_state.changeRenderableLayer( renderable_id_, Unit::Layer::SPRITES_2 );
		break;
	}
};

void SewerMonsterSprite::customUpdate( LevelState& level_state )
{

	switch( state_ )
	{
		case ( MonsterState::SLEEPING ):
		break;

		case ( MonsterState::WAKING ):
			if ( waking_timer_.done() )
			{
				state_ = MonsterState::ATTACK;
				waking_timer_.stop(); // BUG ( also applies to other 2 cases ).
				// I shouldn't have to do this. Either this isn't stopping the timer for some reason, e'en though the code clearly says it does,
				// or for some reason this case is running after stopping, which causes it to start 'gain, e'en though state_ is changed.
			}
			else if ( waking_timer_.on() )
			{
				waking_timer_.update();
			}
			else
			{
				waking_timer_.start();
			}
		break;

		case ( MonsterState::ATTACK ):
			if ( attack_timer_.done() )
			{
				state_ = MonsterState::FALLING_ASLEEP;
				attack_timer_.stop();
			}
			else if ( attack_timer_.on() )
			{
				attack_timer_.update();
			}
			else
			{
				attack_timer_.start();
			}
		break;

		case ( MonsterState::FALLING_ASLEEP ):
			if ( falling_asleep_timer_.done() )
			{
				state_ = MonsterState::SLEEPING;
				falling_asleep_timer_.stop();
			}
			else if ( falling_asleep_timer_.on() )
			{
				falling_asleep_timer_.update();
			}
			else
			{
				falling_asleep_timer_.start();
			}
		break;
	}

	stateGraphics( level_state );
};

void SewerMonsterSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	if ( them.hasType( SpriteType::HERO ) )
	{
		if ( their_collision.collideAny() )
		{
			switch( state_ )
			{
				case ( MonsterState::SLEEPING ):
					state_ = MonsterState::WAKING;
				break;

				case ( MonsterState::ATTACK ):
					them.killNoAnimation();
					them.changeX( centerXSubPixels() - them.halfWidthSubPixels() );
					them.changeY( centerYSubPixels() - them.halfHeightSubPixels() );
				break;
			}
		}

		if ( them.isDead() )
		{
			attack_timer_.stop(); // Don't reopen mouth.
		}
	}
};

int SewerMonsterSprite::getFrame( int tick ) const
{
	return floor( tick / FRAME_SPEED );
};

int SewerMonsterSprite::getXImg( int frame ) const
{
	return frame * GRAPHIC_WIDTH;
};
