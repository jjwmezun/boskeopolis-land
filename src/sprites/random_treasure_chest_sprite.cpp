#include "audio.hpp"
#include <cassert>
#include "collision.hpp"
#include "event_system.hpp"
#include "input.hpp"
#include "random_treasure_chest_sprite.hpp"
#include "sprite_graphics.hpp"
#include <iostream>

static constexpr int NUMBER_OF_RANDOM_CHESTS = 5;
static int count_ = 0;
static int keycane_chest_ = 0;

void setRandomChestIfNotSetYet();

RandomTreasureChestSprite::RandomTreasureChestSprite( int x, int y )
:
	TreasureChestSprite( x, y, ItemType::RANDOM_GEMS ),
	state_ ( State::INIT )
{
	// Since these are static variables,
	// we need to make sure we reset them
	// if we're returning to this level.
	// Since keycane_chest_ starts @ 0 &
	// is set to 0 here till after all sprites
	// are initialized, this only runs once @ the
	// start o' the level 'pon replaying it.
	// ( Note: if this were to be used on a level
	// with multiple maps, the chest with the keycane
	// would change on each map transition back to the map
	// with these sprites ).
	if ( keycane_chest_ != 0 )
	{
		count_ = 0;
		keycane_chest_ = 0;
	}

	id_ = ++count_;
};

RandomTreasureChestSprite::~RandomTreasureChestSprite() {};

void RandomTreasureChestSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	switch ( state_ )
	{
		case ( State::INIT ):
			setRandomChestIfNotSetYet();
			setRandomItemType();
			state_ = State::DORMANT;
		break;
		case ( State::OPENING ):
			animateChestOpening();
			if ( jump_start_speed_ == 2 )
			{
				state_ = State::ITEM_RISING;
			}
		break;
		case( State::ITEM_RISING ):
			raiseItem();
		break;
	}
};

void RandomTreasureChestSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
	switch ( state_ )
	{
		case ( State::DORMANT ):
			if
			(
				Input::pressed( Input::Action::MOVE_UP ) &&
				them.hasType( SpriteType::HERO )         &&
				their_collision.collideAny()
			)
			{
				state_ = State::OPENING;
			}
		break;
		case( State::ITEM_RISING ):
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
					events.won_ = true;
				}
			}
		break;
	}
};

void RandomTreasureChestSprite::debugChestTypeRandomness()
{
	std::cout<<"=========================="<<std::endl;
	std::cout<<"ID   : "<<id_<<std::endl;
	std::cout<<"COUNT: "<<count_<<std::endl;
	std::cout<<"RAND : "<<keycane_chest_<<std::endl;
	std::cout<<"==========================\n\n"<<std::endl;
};

void testRandomness()
{
	const int JRAND_MX = 5;
	int rands[ JRAND_MX ] = { 0, 0, 0, 0, 0 };
	for ( int i = 0; i < 1000; ++i )
	{
		const auto n = mezun::randInt( JRAND_MX - 1, 0 );
		++rands[ n ];
	}

	for ( int j = 0; j < JRAND_MX; ++j )
	{
		std::cout<<j<<": "<<rands[ j ]<<std::endl;
	}
};

void setRandomChestIfNotSetYet()
{
	if ( keycane_chest_ == 0 )
	{
		keycane_chest_ = mezun::randInt( count_, 1 );
	}
};

void RandomTreasureChestSprite::setRandomItemType()
{
	// Default is already RANDOM_GEMS, so only change if
	// it's the selected 1.
	if ( id_ == keycane_chest_ )
	{
		item_type_ = ItemType::RANDOM_KEYCANE;
		setKeyCaneGFXFlash( false );
	}
};
