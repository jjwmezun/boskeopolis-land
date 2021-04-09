#include "alarm_clock_note_sprite.hpp"
#include "collision.hpp"
#include "event_system.hpp"
#include "level_state.hpp"
#include "map.hpp"
#include "mezun_math.hpp"
#include "sprite_graphics.hpp"

AlarmClockNoteSprite::AlarmClockNoteSprite( int x, int y, Direction::Horizontal direction )
:
	Sprite( std::make_unique<SpriteGraphics> ( "sprites/axe-monster.png", 0, 0, false, false, 0.0, -2, -2, 7, 10 ), x, y, 5, 6, {}, 0, 0, 2000, 6000, direction, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::PERMANENT, true, false )
{
    on_ground_ = true;
    start_speed_ = mezun::randInt( 3000, -3000 );
};

AlarmClockNoteSprite::~AlarmClockNoteSprite() {};

void AlarmClockNoteSprite::customUpdate( LevelState& level_state )
{
    jump();
    hit_box_.x -= start_speed_;
    if ( fellInBottomlessPit( level_state.currentMap() ) )
    {
        kill();
    }
};

void AlarmClockNoteSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if ( them.hasType( SpriteType::HERO ) )
    {
        if ( their_collision.collideAny() )
        {
            level_state.events().setStunned();
        }
    }
};
