


// Name
//===================================
//
// HydrantSprite
//


// DEPENDENCIES
//===================================

    #include "collision.h"
    #include "hydrant_sprite.h"
    #include "hydrant_graphics.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    HydrantSprite::HydrantSprite( int x, int y )
    :
        Sprite( std::make_unique<HydrantGraphics> (), x, y, 14, 14, {}, 100, 1500, 3000, 3000 )
    {
        jump_lock_ = false;
    };

    HydrantSprite::~HydrantSprite() {};

    void HydrantSprite::customUpdate( const Input& input, Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks )
    {
        if ( isDead() )
            block_interact_ = false;

        if ( awake_ && delay_.done() )
        {
            if ( onGround() )
            {
                jump();
            }
        }
        else if ( delay_.counter() > 24 )
        {
            graphics_->current_frame_x_ = 32;
        }
        else if ( delay_.counter() > 8 )
        {
            graphics_->current_frame_x_ = 16;
        }

        if ( delay_.counter() % 4 == 1 )
        {
            graphics_->current_frame_y_ = 16;
        }
        else
        {
            graphics_->current_frame_y_ = 0;
        }
    };

    void HydrantSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap )
    {
        if ( them.hasType( Sprite::SpriteType::HERO ) )
        {
            if ( awake_ )
            {
                if ( delay_.done() )
                {
                    if ( them.xSubPixels() > rightSubPixels() )
                        moveRight();
                    else
                        moveLeft();

                    if ( their_collision.collideAny() && them.bottomSubPixels() < ySubPixels() + 12000 )
                    {
                        kill();
                        them.bounce();
                    }
                    else if ( their_collision.collideAny() && !isDead() )
                    {
                        them.hurt();
                    }
                }
                else
                {
                    delay_.update();
                }
            }
            else
            {
                if ( them.rightSubPixels() > xSubPixels() - 40000 && them.xSubPixels() < rightSubPixels() + 40000 )
                {
                    awake_ = true;
                    delay_.start();
                }
            }

            if ( !delay_.done() )
            {
                them.collideStopAny( their_collision );
            }

        }
        else if ( them.hasType( Sprite::SpriteType::ENEMY ) )
        {
            collideStopAny( my_collision );
        }
    };
