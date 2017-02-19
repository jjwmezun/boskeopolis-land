




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
    #include "player_graphics.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    OverworldPlayerSprite::OverworldPlayerSprite( int x, int y )
    :
        Sprite( std::unique_ptr<SpriteGraphics> ( new PlayerGraphics() ), x, y, 14, 15, { SpriteType::HERO }, 160, 2000, 1000, 6000, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::PERMANENT, false, true )
    {};

    OverworldPlayerSprite::~OverworldPlayerSprite() {};

    void OverworldPlayerSprite::customUpdate( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites )
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
		
		camera.adjust( *this, lvmap );
    };

    void OverworldPlayerSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites )
    {
    };
