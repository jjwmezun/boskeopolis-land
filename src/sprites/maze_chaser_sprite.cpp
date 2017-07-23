



// Name
//===================================
//
// MazeChaserSprite
//


// DEPENDENCIES
//===================================

	#include "block_system.h"
	#include <cassert>
	#include "collision.h"
	#include "map.h"
    #include "maze_chaser_sprite.h"
    #include "maze_chaser_graphics.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    MazeChaserSprite::MazeChaserSprite( int x, int y, Type type )
    :
        Sprite
		(
			std::make_unique<MazeChaserGraphics> ( type ),
			x,
			y,
			14,
			14,
			{ SpriteType::ENEMY },
			2600,
			2600,
			0,
			0,
			Direction::Horizontal::__NULL,
			Direction::Vertical::DOWN,
			nullptr,
			SpriteMovement::Type::FLOATING,
			CameraMovement::PERMANENT,
			false,
			false,
			true,
			true
		),
		type_ ( type ),
		state_timer_ ( STATE_LENGTH, true ),
		current_tile_ ( getChaserPosition() ),
		target_ ( std::make_pair<int, int> ( 0, 0 ) ),
		speed_ ( SPEED_SLOW ),
		direction_prev_ ( Direction::Simple::DOWN ),
		directions_possible_ ( defaultPossibleDirections() ),
		can_turn_ ( true )
    {
		direction_ = direction_prev_;
	};

    MazeChaserSprite::~MazeChaserSprite() {};

    void MazeChaserSprite::customUpdate
	(
		const Input& input,
		Camera& camera,
		Map& lvmap,
		EventSystem& events,
		SpriteSystem& sprites,
		BlockSystem& blocks
	)
    {
		if ( can_turn_ )
		{
			switch( direction_ )
			{
				case ( Direction::Simple::DOWN ):
					hit_box_.y += speed_;
				break;

				case ( Direction::Simple::UP ):
					hit_box_.y -= speed_;
				break;

				case ( Direction::Simple::RIGHT ):
					hit_box_.x += speed_;
				break;

				case ( Direction::Simple::LEFT ):
					hit_box_.x -= speed_;
				break;
			}
		}
		else
		{
			bool reached_middle = false;
			
			// Tile position is based on center o' block for better accuracy & equal treatment o' directions.
			// The problem is, it causes chasers to cling halfway into the wall, 'cause they turn the moment the center reaches a space.
			// This ensures they go all the way on the tile before turning, so that they're always in the middle o' the passageways.
			switch( direction_prev_ )
			{
				case ( Direction::Simple::DOWN ):
					reached_middle = ySubPixels() >= Unit::BlocksToSubPixels( std::get<PAIR_POS_Y> ( current_tile_ ) );
				break;
				case ( Direction::Simple::UP ):
					reached_middle = ySubPixels() <= Unit::BlocksToSubPixels( std::get<PAIR_POS_Y> ( current_tile_ ) );
				break;
				case ( Direction::Simple::RIGHT ):
					reached_middle = xSubPixels() >= Unit::BlocksToSubPixels( std::get<PAIR_POS_X> ( current_tile_ ) );
				break;
				case ( Direction::Simple::LEFT ):
					reached_middle = xSubPixels() <= Unit::BlocksToSubPixels( std::get<PAIR_POS_X> ( current_tile_ ) );
				break;
			}
				
			if ( reached_middle )
			{
				can_turn_ = true;
				direction_prev_ = direction_; // Now it's safe to o'erwrite the discarded direction.
			}
			else
			{
				// Keep going down original direction till all the way in passage.
				switch( direction_prev_ )
				{
					case ( Direction::Simple::DOWN ):
						hit_box_.y += speed_;
					break;

					case ( Direction::Simple::UP ):
						hit_box_.y -= speed_;
					break;

					case ( Direction::Simple::RIGHT ):
						hit_box_.x += speed_;
					break;

					case ( Direction::Simple::LEFT ):
						hit_box_.x -= speed_;
					break;
				}
			}
		}
		
		
		// Keep chasers aligned by only making them move when they hit a new tile.
		if ( evenTile() )
		{
			testDirection( blocks );
		}
		
		
		if ( state_timer_.hit() )
		{
			// Each Chaser cycles through types every N seconds.
			switch( type_ )
			{
				case ( Type::SHADOW ):
					type_ = Type::AMBUSH;
				break;
				case ( Type::AMBUSH ):
					type_ = Type::RANDO;
				break;
				case ( Type::RANDO ):
					type_ = Type::OBFUSCATING;
				break;
				case ( Type::OBFUSCATING ):
					type_ = Type::SHADOW;
				break;
			}
			
			// Change speeds after certain # o' N-second cycles.
			if ( state_timer_.numOHits() == 1 )
			{
				speed_ = SPEED_MEDIUM;
			}
			if ( state_timer_.numOHits() == 4 )
			{
				if ( type_ == Type::SHADOW )
				{
					speed_ = SPEED_FAST;
				}
			}
			
			state_timer_.stop();
		}
		else if ( state_timer_.on() )
		{
			state_timer_.update();
		}
		else
		{
			state_timer_.start();
		}
		
		// If chaser somehow ends up off the map, put it on a random tile.
		if
		(
			rightPixels() < 0 ||
			xPixels() > lvmap.widthPixels() ||
			bottomPixels() < 0 ||
			yPixels() > lvmap.heightPixels()
		)
		{
			std::pair<int, int> rand_tile;
			
			
			// Keep finding random tiles till it finds 1 that's nonsolid.
			do
			{
				rand_tile = randomTile();
			}
			while ( blockInPosition( rand_tile, blocks ) );
			
			hit_box_.x = Unit::BlocksToSubPixels( std::get<PAIR_POS_X> ( rand_tile ) );
			hit_box_.y = Unit::BlocksToSubPixels( std::get<PAIR_POS_Y> ( rand_tile ) );
		}
    };

    void MazeChaserSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap )
	{	
		if ( them.hasType( SpriteType::HERO ) )
		{
			// Default ( Shadow/Blinky )
			target_ = getPlayerPosition( them );
			
			switch( type_ )
			{
				case ( Type::AMBUSH ):
					// Aim for 4 blocks 'head o' you. (Pinky)
					switch( them.direction_ )
					{
						case ( Direction::Simple::UP ):
							std::get<PAIR_POS_Y> ( target_ ) -= 4;
						break;
						case ( Direction::Simple::DOWN ):
							std::get<PAIR_POS_Y> ( target_ ) += 4;
						break;
						case ( Direction::Simple::LEFT ):
							std::get<PAIR_POS_X> ( target_ ) -= 4;
						break;
						case ( Direction::Simple::RIGHT ):
							std::get<PAIR_POS_X> ( target_ ) += 4;
						break;
					}
				break;
				
				case ( Type::RANDO ):
					target_ = randomTile();
				break;
				
				case ( Type::OBFUSCATING ):
					// If close to you, make it aim for down-left corner spot. (Clyde)
					if ( distanceOfCoordinates( target_, getChaserPosition() ) < 8 )
					{
						target_ = std::make_pair<int, int> ( 2, 37 );
					}
				break;
			}
			
			//if ( their_collision.collideAny() )
			if ( current_tile_ == getPlayerPosition( them ) )
			{
				them.hurt();
			}
		}
    };

	const std::pair<int, int> MazeChaserSprite::getChaserPosition() const
	{
		return getPosition( *this );
	};

	const std::pair<int, int> MazeChaserSprite::getNextChaserPosition() const
	{
		return getNextChaserPosition( direction_ );
	};

	const std::pair<int, int> MazeChaserSprite::getNextChaserPosition( Direction::Simple direction ) const
	{
		std::pair<int, int> pos = getPosition( *this );
		
		switch( direction )
		{
			case ( Direction::Simple::DOWN ):
				++std::get<PAIR_POS_Y> ( pos );
			break;
			
			case ( Direction::Simple::UP ):
				--std::get<PAIR_POS_Y> ( pos );
			break;
			
			case ( Direction::Simple::RIGHT ):
				++std::get<PAIR_POS_X> ( pos );
			break;

			default:
				--std::get<PAIR_POS_X> ( pos );
			break;
		}
		
		return pos;
	};
	
	const std::pair<int, int> MazeChaserSprite::getPlayerPosition( const Object& player ) const
	{
		return getPosition( player );
	};
	
	const std::pair<int, int> MazeChaserSprite::getPosition( const Object& obj ) const
	{
		const int x = Unit::SubPixelsToBlocks( obj.centerXSubPixels() );
		const int y = Unit::SubPixelsToBlocks( obj.centerYSubPixels() );
		return std::pair<int, int> ( x, y );
	};

	bool MazeChaserSprite::blockInPosition( const std::pair<int, int>& coordinates, const BlockSystem& blocks ) const
	{
		// Tile shrunk to prevent risk o' chaser registering edges o' adjacent blocks & getting stuck or something.
		const int x = Unit::BlocksToSubPixels( std::get<PAIR_POS_X> ( coordinates ) ) + 4000;
		const int y = Unit::BlocksToSubPixels( std::get<PAIR_POS_Y> ( coordinates ) ) + 4000;
		
		const sdl2::SDLRect r = { x, y, Unit::BlocksToSubPixels( 1 ) - 4000, Unit::BlocksToSubPixels( 1 ) - 4000 };
		
		return blocks.blocksInTheWay( r, BlockComponent::Type::SOLID );
	};
	
	int MazeChaserSprite::distanceOfCoordinates( const std::pair<int, int>& one, const std::pair<int, int>& two ) const
	{
		const int dx = abs( std::get<PAIR_POS_X> ( one ) - std::get<PAIR_POS_X> ( two ) );
		const int dy = abs( std::get<PAIR_POS_Y> ( one ) - std::get<PAIR_POS_Y> ( two ) );
		return sqrt( ( dx * dx ) + ( dy * dy ) );
	};
	
	const bool MazeChaserSprite::evenTile()
	{
		// Incredibly imperfect:
		// Tries to loosen constraints to allow for higher speeds, but chasers still break out o' alignment & fly all o'er solid blocks when speed > 2000.
		
		const std::pair<int, int> TILE_ON_NOW = getChaserPosition();
		
		switch ( Direction::SimpleIsOfWhatType( direction_ ) )
		{		
			case ( Direction::Type::HORIZONTAL ):
				if ( std::get<PAIR_POS_X> ( TILE_ON_NOW ) != std::get<PAIR_POS_X> ( current_tile_ ) )
				{
					std::get<PAIR_POS_X> ( current_tile_ ) = std::get<PAIR_POS_X> ( TILE_ON_NOW );
					return true;
				}
			break;
				
			case ( Direction::Type::VERTICAL ):
				if ( std::get<PAIR_POS_Y> ( TILE_ON_NOW ) != std::get<PAIR_POS_Y> ( current_tile_ ) )
				{
					std::get<PAIR_POS_Y> ( current_tile_ ) = std::get<PAIR_POS_Y> ( TILE_ON_NOW );
					return true;
				}
			break;
		}
		
		return false;
	};
	
	void MazeChaserSprite::testDirection( const BlockSystem& blocks )
	{
		// Directions refer to tiles adjacent to chaser's current tile--i.e. potential next tiles to move to.
		directions_possible_ = defaultPossibleDirections();

		// Chaser can't go backward.
		directions_possible_.at( ( int )Direction::oppositeSimple( direction_ ) ) = false;

		// Chaser can't go through solid blocks.
		for ( int i = 1; i < Direction::SIMPLE_SIZE; ++i )
		{
			if ( directions_possible_.at( i ) )
			{
				directions_possible_.at( i ) = !blockInPosition( getNextChaserPosition( (Direction::Simple)i ), blocks );
			}
		}

		// Cycle through remaining applicable direction tiles
		// & pick the 1 with the shortest diagonal distance from it to the target tile.
		Direction::Simple nearest_dir = Direction::Simple::__NULL;
		int lowest_distance = 999999999; // Arbitrarily huge #.

		for ( int j = 1; j < Direction::SIMPLE_SIZE; ++j )
		{
			if ( directions_possible_.at( j ) )
			{	
				const int distance = distanceOfCoordinates( getNextChaserPosition( (Direction::Simple)j ), target_ );

				if ( distance < lowest_distance )
				{
					lowest_distance = distance;
					nearest_dir = ( Direction::Simple )j;
				}
			}
		}
		
		assert( nearest_dir != Direction::Simple::__NULL );

		direction_ = nearest_dir;
		
		if ( direction_ != direction_prev_ )
		{
			can_turn_ = false; // Warn update that it needs to check for turning.
		}
	};

	const std::pair<int, int> MazeChaserSprite::randomTile() const
	{
		// Note: Impractical to get map width, so it's hard-coded in as the "40."
		return std::make_pair<int, int> ( mezun::randInt( 40, 0 ), mezun::randInt( 40, 0 ) );
	};

	std::array<bool, Direction::SIMPLE_SIZE> MazeChaserSprite::defaultPossibleDirections() const
	{
		// 1st, __NULL, is ne'er valid.
		// Whittle down other 4 as they're proven to be inapplicable.
		return { { false, true, true, true, true } };
	};