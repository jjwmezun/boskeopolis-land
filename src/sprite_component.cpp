#include <cmath>
#include "sprite_component.hpp"
#include "sprite.hpp"

namespace BSL
{
    SpriteComponent SpriteComponent::create( Type type, const ArgList & args )
    {
        SpriteComponent c;
        c.type_ = type;
        switch ( type )
        {
            case ( Type::MOVEX ):
            {
                c.data_.movex.dir = GetArg( "dir", args, Dir::X::RIGHT );
                c.data_.movex.width = GetArg( "width", args, 0.0f );
                c.data_.movex.acc = GetArg( "acc", args, 0.0f );
                c.data_.movex.maxspeed = GetArg( "maxspeed", args, 0.0f );
            }
            break;
            case ( Type::MOVEY ):
            {
                c.data_.movey.dir = GetArg( "dir", args, Dir::Y::DOWN );
                c.data_.movey.height = GetArg( "height", args, 0.0f );
                c.data_.movey.acc = GetArg( "acc", args, 0.0f );
                c.data_.movey.maxspeed = GetArg( "maxspeed", args, 0.0f );
            }
            break;
            case ( Type::CIRCLE ):
            {
                c.data_.circle.startx = GetArg( "x", args, 0.0f );
                c.data_.circle.starty = GetArg( "y", args, 0.0f );
                c.data_.circle.speed = GetArg( "speed", args, 0.0f );
                c.data_.circle.radius = GetArg( "radius", args, 0.0f );
                c.data_.circle.angle = GetArg( "angle", args, 0.0f );
            }
            break;
        }
        return c;
    };

    void SpriteComponent::update
    (
        Sprite & sprite,
        float dt,
        Level & level,
        Game & game,
        std::vector<Sprite> & sprites
    )
    {
        switch ( type_ )
        {
            case ( Type::__NULL ):
            {
                // Do nothing.
            }
            break;
            case ( Type::MOVEX ):
            {
                if ( data_.movex.dir == Dir::X::LEFT )
                {
                    if ( sprite.dir_x_ == Dir::X::LEFT )
                    {
                        if ( sprite.pos_.x < sprite.startx_ - data_.movex.width )
                        {
                            sprite.dir_x_ = Dir::X::RIGHT;
                        }
                        else
                        {
                            sprite.accx_ = -data_.movex.acc;
                        }
                    }
                    else
                    {
                        if ( sprite.pos_.x > sprite.startx_ )
                        {
                            sprite.dir_x_ = Dir::X::LEFT;
                        }
                        else
                        {
                            sprite.accx_ = data_.movex.acc;
                        }
                    }
                }
                else
                {
                    if ( sprite.dir_x_ == Dir::X::LEFT )
                    {
                        if ( sprite.pos_.x < sprite.startx_ )
                        {
                            sprite.dir_x_ = Dir::X::RIGHT;
                        }
                        else
                        {
                            sprite.accx_ = -data_.movex.acc;
                        }
                    }
                    else
                    {
                        if ( sprite.pos_.x > sprite.startx_ + data_.movex.width )
                        {
                            sprite.dir_x_ = Dir::X::LEFT;
                        }
                        else
                        {
                            sprite.accx_ = data_.movex.acc;
                        }
                    }
                }

                sprite.vx_ += sprite.accx_ * dt;
                if ( sprite.vx_ > data_.movex.maxspeed )
                {
                    sprite.vx_ = data_.movex.maxspeed;
                }
                else if ( sprite.vx_ < -data_.movex.maxspeed )
                {
                    sprite.vx_ = -data_.movex.maxspeed;
                }
                if ( sprite.accx_ == 0.0f )
                {
                    sprite.vx_ /= ( 1.0f + 0.5f * dt );
                }
                sprite.pos_.x += sprite.vx_ * dt;
            }
            break;
            case ( Type::MOVEY ):
            {
                if ( data_.movey.dir == Dir::Y::UP )
                {
                    if ( sprite.dir_y_ == Dir::Y::UP )
                    {
                        if ( sprite.pos_.y < sprite.starty_ - data_.movey.height )
                        {
                            sprite.dir_y_ = Dir::Y::DOWN;
                        }
                        else
                        {
                            sprite.accy_ = -data_.movey.acc;
                        }
                    }
                    else
                    {
                        if ( sprite.pos_.y > sprite.starty_ )
                        {
                            sprite.dir_y_ = Dir::Y::UP;
                        }
                        else
                        {
                            sprite.accy_ = data_.movey.acc;
                        }
                    }
                }
                else
                {
                    if ( sprite.dir_y_ == Dir::Y::UP )
                    {
                        if ( sprite.pos_.y < sprite.starty_ )
                        {
                            sprite.dir_y_ = Dir::Y::DOWN;
                        }
                        else
                        {
                            sprite.accy_ = -data_.movey.acc;
                        }
                    }
                    else
                    {
                        if ( sprite.pos_.y > sprite.starty_ + data_.movey.height )
                        {
                            sprite.dir_y_ = Dir::Y::UP;
                        }
                        else
                        {
                            sprite.accy_ = data_.movey.acc;
                        }
                    }
                }

                sprite.vy_ += sprite.accy_ * dt;
                if ( sprite.vy_ > data_.movey.maxspeed )
                {
                    sprite.vy_ = data_.movey.maxspeed;
                }
                else if ( sprite.vy_ < -data_.movey.maxspeed )
                {
                    sprite.vy_ = -data_.movey.maxspeed;
                }
                if ( sprite.accy_ == 0.0f )
                {
                    sprite.vy_ /= ( 1.0f + 0.5f * dt );
                }
                sprite.pos_.y += sprite.vy_ * dt;
            }
            break;
            case ( Type::CIRCLE ):
            {
                sprite.pos_.x = data_.circle.startx + sprite.pos_.halfWidth() + std::cos( data_.circle.angle ) * data_.circle.radius;
                sprite.pos_.y = data_.circle.starty + sprite.pos_.halfHeight() + std::sin( data_.circle.angle ) * data_.circle.radius;
                data_.circle.angle += data_.circle.speed;
            }
            break;
        }
    };
}