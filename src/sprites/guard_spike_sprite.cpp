#include "guard_spike_sprite.hpp"
#include "sprite_graphics.hpp"

static constexpr int FAST_MOVEMENT = 2000;
static constexpr int SLOW_MOVEMENT = 800;

GuardSpikeSprite::GuardSpikeSprite( int x, int y )
:
	Sprite( std::make_unique<SpriteGraphics> ( "tilesets/universal.png", 16, 16 ), x, y, 16, 16, { SpriteType::ENEMY }, SLOW_MOVEMENT, SLOW_MOVEMENT, 0, 0, Direction::Horizontal::LEFT, Direction::Vertical::__NULL, nullptr, SpriteMovement::Type::GROUNDED, CameraMovement::RESET_OFFSCREEN_AND_AWAY )
{};

GuardSpikeSprite::~GuardSpikeSprite() {};

void GuardSpikeSprite::customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health )
{
	turnOnEdge( blocks );
	turnOnCollide();
	moveInDirectionX();
};

void GuardSpikeSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events )
{
    if ( them.hasType( SpriteType::HERO ) )
    {
        start_speed_ = top_speed_ = ( them.on_ground_ && them.bottomSubPixels() == bottomSubPixels() )
            ? FAST_MOVEMENT
            : SLOW_MOVEMENT;
    }
};
