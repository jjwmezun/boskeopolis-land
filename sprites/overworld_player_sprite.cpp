




// Name
//===================================
//
// OverworldPlayerSprite
//


// DEPENDENCIES
//===================================

	#include "camera.h"
    #include "input.h"
    #include "overworld_player_sprite.h"
    #include "overworld_player_graphics.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    OverworldPlayerSprite::OverworldPlayerSprite( int x, int y )
    :
        Sprite( std::unique_ptr<SpriteGraphics> ( new OverworldPlayerGraphics() ), x, y, 12, 14, { SpriteType::HERO }, 800, 3000, 1000, 6000, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, false, true, true, false, 0, 2, 2 ),
		death_spins_ ( 0 )
    {
	};

    OverworldPlayerSprite::~OverworldPlayerSprite() {};

    void OverworldPlayerSprite::customUpdate( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
    {
        if ( input.held( Input::Action::MOVE_LEFT ) )
        {
            moveLeft();
        }
        else if ( input.held( Input::Action::MOVE_RIGHT ) )
        {
            moveRight();
        }
        else
        {
            stopX();
        }

        if ( input.held( Input::Action::MOVE_UP ) )
        {
            moveUp();
        }
        else if ( input.held( Input::Action::MOVE_DOWN ) )
        {
            moveDown();
        }
        else
        {
            stopY();
        }
		
		camera.adjustCart( *this, lvmap );
    };

    void OverworldPlayerSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites )
    {
    };

	void OverworldPlayerSprite::deathAction( Camera& camera )
	{	
		block_interact_ = false;
		
        if ( camera.offscreen( hit_box_ ) )
        {
            death_finished_ = true;
        }
		
		if ( death_spins_ > 3 )
		{
			changeMovement( SpriteMovement::Type::GROUNDED );
		}
		else
		{
			fullStopX();
			fullStopY();

			graphics_->rotation_ += DEATH_SPIN_SPEED;

			if ( ( int )graphics_->rotation_ % 360 == 0 )
			{
				++death_spins_;
			}
		}
	};
