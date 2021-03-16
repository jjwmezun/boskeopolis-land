#pragma once

#include "counter_t.hpp"
#include "sprite.hpp"
#include "sprite_component.hpp"
#include "timer_repeat_t2.hpp"

class DesertHawkSprite final : public Sprite
{
	public:
		DesertHawkSprite( int x, int y, Direction::Horizontal direction, std::unique_ptr<SpriteComponent>&& component = nullptr, CameraMovement camera_movement = CameraMovement::RESET_OFFSCREEN_AND_AWAY );
		~DesertHawkSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void reset() override;

	private:
		CounterT<3, 0, 0, true> animation_frame_;
		TimerRepeatT2<16> animation_timer_;
		std::unique_ptr<SpriteComponent> component_;

};
