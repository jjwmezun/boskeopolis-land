#include "camera.hpp"
#include "input_component_mirror.hpp"
#include "level_state.hpp"
#include "ghost_player_sprite.hpp"
#include "sprite_graphics.hpp"

GhostPlayerSprite::GhostPlayerSprite()
:
	PlayerSprite
	(
		-99,
		-99,
		PlayerSprite::DEFAULT_JUMP_START_SPEED,
		PlayerSprite::DEFAULT_JUMP_TOP_SPEED,
		std::unique_ptr<InputComponent> ( new InputComponentMirror( &player_sequence_ ) ),
		std::make_unique<SpriteGraphics> ( "sprites/autumn-skirt-ghost.png", 0, 0, false, false, 0, -1, -2, 2, 4 )
	),
    timer_ ( 0 )
{
    camera_movement_ = CameraMovement::PERMANENT;
    despawn_when_dead_ = false;
    types_ = {};
    changeMovement( SpriteMovement::Type::FLOATING );
};

GhostPlayerSprite::~GhostPlayerSprite() {};

void GhostPlayerSprite::customUpdate( LevelState& level_state )
{
    if ( timer_ == 60 )
    {
        changeMovement( SpriteMovement::Type::GROUNDED );
        types_.emplace_back( SpriteType::ENEMY );
        graphics_->visible_ = true;
    }

    if ( timer_ >= 60 )
    {
        actions( level_state.blocks(), level_state.events() );
        player_gfx_.update( *this, graphics_.get(), level_state.health() );
        if ( fellInBottomlessPit( level_state.currentMap() ) )
        {
            timer_ = 1;
            types_ = {};
            changeMovement( SpriteMovement::Type::FLOATING );
            fullStopY();
            fullStopX();
            //player_sequence_.clear();
            //input_.reset();
        }
    }
    else
    {
        const int frame = timer_ % 8;
        graphics_->visible_ = frame >= 4;
        ++timer_;
    }
    if ( timer_ != 1 )
    {
        player_sequence_.emplace_back
        (
            std::array<bool, Input::NUM_O_ACTIONS>
            {
                false, false, false,
                Input::held( Input::Action::JUMP ),
                Input::held( Input::Action::RUN ),
                Input::held( Input::Action::MOVE_UP ),
                Input::held( Input::Action::MOVE_RIGHT ),
                Input::held( Input::Action::MOVE_DOWN ),
                Input::held( Input::Action::MOVE_LEFT ),
                false, false, false, false
            }
        );
    }
};

void GhostPlayerSprite::customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state )
{
    if ( timer_ == 1 )
    {
        if ( them.hasType( SpriteType::HERO ) )
        {
            hit_box_.x = them.hit_box_.x;
            hit_box_.y = them.hit_box_.y;
            player_sequence_.emplace_back
            (
                std::array<bool, Input::NUM_O_ACTIONS>
                {
                    false, false, false,
                    Input::held( Input::Action::JUMP ),
                    Input::held( Input::Action::RUN ),
                    Input::held( Input::Action::MOVE_UP ),
                    Input::held( Input::Action::MOVE_RIGHT ),
                    Input::held( Input::Action::MOVE_DOWN ),
                    Input::held( Input::Action::MOVE_LEFT ),
                    false, false, false, false
                }
            );
        }
    }
};
