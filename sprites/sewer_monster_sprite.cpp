

// Name
//===================================
//
// SewerMonsterSprite
//


// DEPENDENCIES
//===================================

    #include "collision.h"
    #include "sewer_monster_graphics.h"
    #include "sewer_monster_sprite.h"
    #include "sprite_graphics.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    SewerMonsterSprite::SewerMonsterSprite( int x, int y )
    :
        Sprite( std::unique_ptr<SpriteGraphics> ( new SewerMonsterGraphics() ), x+16, y+16, 48, 48, {}, 100, 1500, 3000, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false, false, true, true ),
        awake_ ( false ),
        attacking_ ( false ),
        after_attack_ ( false ),
        delay_ ( { 64, false } ),
        attack_length_ ( { 32, false } ),
        after_attack_delay_ ( { 32, false } )
    {};

    SewerMonsterSprite::~SewerMonsterSprite() {};

    void SewerMonsterSprite::customUpdate( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites )
    {
        if ( !attacking_ )
        {
            if ( delay_.done() )
            {
                attacking_ = true;
            }
            else if ( delay_.on() )
            {
                delay_.update();
            }
            else if ( awake_ )
            {
                graphics_->current_frame_x_ = 80;

                if ( !delay_.on() )
                {
                    delay_.start();
                }
            }
            else
            {
                graphics_->current_frame_x_ = 0;

                if ( after_attack_ )
                {
                    if ( after_attack_delay_.done() )
                    {
                        after_attack_ = false;
                        after_attack_delay_.stop();
                    }
                    else if ( after_attack_delay_.on() )
                    {
                        after_attack_delay_.update();
                    }
                    else
                    {
                        after_attack_delay_.start();
                    }
                }
            }
        }
        else
        {
            if ( attack_length_.done() )
            {
                attacking_ = false;
                awake_ = false;

                attack_length_.stop();
                delay_.stop();

                after_attack_ = true;
            }
            else if ( attack_length_.on() )
            {
                attack_length_.update();
            }
            else
            {
                attack_length_.start();
                graphics_->current_frame_x_ = 160;
            }
        }
    };

    void SewerMonsterSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites )
    {
        if ( them.hasType( SpriteType::HERO ) )
        {
            if ( attacking_ )
            {
                if ( their_collision.collideAny() )
                {
                    them.hurt();
                }
            }
            else if ( !after_attack_ )
            {
                if
                (
                    them.rightSubPixels() > leftSubPixels() - Unit::BlocksToSubPixels( 2 ) &&
                    them.leftSubPixels() < rightSubPixels() + Unit::BlocksToSubPixels( 2 ) &&
                    !awake_
                )
                {
                    awake_ = true;
                }
            }
        }
    };

