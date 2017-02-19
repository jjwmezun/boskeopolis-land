






// Name
//===================================
//
// PufferbeeSprite
//


// DEPENDENCIES
//===================================

    #include "collision.h"
    #include "pufferbee_sprite.h"
    #include "pufferbee_graphics.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    PufferbeeSprite::PufferbeeSprite( int x, int y, std::unique_ptr<SpriteComponent> component )
    :
        Sprite( std::unique_ptr<SpriteGraphics> ( new PufferbeeGraphics() ), x, y, 20, 20, { SpriteType::ENEMY }, 600, 2000, 0, 0, Direction::Horizontal::__NULL, Direction::Vertical::__NULL, std::move( component ), SpriteMovement::Type::FLOATING, CameraMovement::RESET_OFFSCREEN_AND_AWAY, false, false )
    {};

    PufferbeeSprite::~PufferbeeSprite() {};

    void PufferbeeSprite::customUpdate( Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites )
    {
        if ( component_ == nullptr )
        {
            switch ( randomDirection() )
            {
                case ( Direction::Rotation::UP ):
                    hit_box_.y = original_hit_box_.y - 1000;
                    hit_box_.x = original_hit_box_.x;
                break;
                case ( Direction::Rotation::UP_RIGHT ):
                    hit_box_.y = original_hit_box_.y - 1000;
                    hit_box_.x = original_hit_box_.x + 1000;
                break;
                case ( Direction::Rotation::RIGHT ):
                    hit_box_.x = original_hit_box_.x + 1000;
                    hit_box_.y = original_hit_box_.y;
                break;
                case ( Direction::Rotation::DOWN_RIGHT ):
                    hit_box_.y = original_hit_box_.y + 1000;
                    hit_box_.x = original_hit_box_.x + 1000;
                break;
                case ( Direction::Rotation::DOWN ):
                    hit_box_.y = original_hit_box_.y + 1000;
                    hit_box_.x = original_hit_box_.x;
                break;
                case ( Direction::Rotation::DOWN_LEFT ):
                    hit_box_.y = original_hit_box_.y + 1000;
                    hit_box_.x = original_hit_box_.x - 1000;
                break;
                case ( Direction::Rotation::LEFT ):
                    hit_box_.x = original_hit_box_.x - 1000;
                    hit_box_.y = original_hit_box_.y;
                break;
                case ( Direction::Rotation::UP_LEFT ):
                    hit_box_.y = original_hit_box_.y - 1000;
                    hit_box_.x = original_hit_box_.x - 1000;
                break;
                default:
                    hit_box_.y = original_hit_box_.y;
                    hit_box_.x = original_hit_box_.x;
            }
        }
    };

    void PufferbeeSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites )
    {
    };

    Direction::Rotation PufferbeeSprite::randomDirection() const
    {
        return (Direction::Rotation)( rand() % 9 );
    };
