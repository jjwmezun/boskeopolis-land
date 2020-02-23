#include "audio.hpp"
#include <cassert>
#include "collision.hpp"
#include "event_system.hpp"
#include "input.hpp"
#include "level_state.hpp"
#include "random_treasure_chest_sprite.hpp"
#include "sprite_graphics.hpp"

RandomTreasureChestSprite::RandomTreasureChestSprite( int x, int y, EventSystem& events )
:
	TreasureChestSprite( x, y, ItemType::RANDOM_GEMS ),
	state_ ( State::INIT )
{
	id_ = events.addRandomTreasure();
};

RandomTreasureChestSprite::~RandomTreasureChestSprite() {};

void RandomTreasureChestSprite::customUpdate( LevelState& level_state )
{
	switch ( state_ )
	{
		case ( State::INIT ):
		{
			setRandomItemType( level_state.events() );
			state_ = State::DORMANT;
		}
		break;
		case ( State::OPENING ):
		{
			animateChestOpening();
			if ( jump_start_speed_ == 2 )
			{
				state_ = State::ITEM_RISING;
			}
		}
		break;
		case( State::ITEM_RISING ):
		{
			raiseItem();
		}
		break;
	}
};

void RandomTreasureChestSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
	switch ( state_ )
	{
		case ( State::DORMANT ):
		{
			if
			(
				Input::pressed( Input::Action::MOVE_UP ) &&
				them.hasType( SpriteType::HERO )         &&
				their_collision.collideAny()
			)
			{
				state_ = State::OPENING;
			}
		}
		break;
		case( State::ITEM_RISING ):
		{
			if
			(
				item_type_ == ItemType::RANDOM_KEYCANE &&
				isRandomKeyCaneAtTop()                 &&
				them.hasType( SpriteType::HERO )
			)
			{
				const auto collision = them.testCollision( getKeyCanePosition() );
				if ( collision.collideAny() )
				{
					level_state.events().win();
				}
			}
		}
		break;
	}
};

void RandomTreasureChestSprite::setRandomItemType( EventSystem& events )
{
	// Default is already RANDOM_GEMS, so only change if
	// it's the selected 1.
	if ( events.testRandomTreasure( id_ ) )
	{
		item_type_ = ItemType::RANDOM_KEYCANE;
		setKeyCaneGFXFlash( false );
	}
};
