#pragma once

class Block;

#include "sprite.hpp"
#include "timer_simple_t.hpp"
#include "timer_switch.hpp"

class HeatBeamSprite : public Sprite
{
    public:
		enum class Type
		{
			ODD,
			EVEN
		};

        HeatBeamSprite( int x, int y, Type type );
        ~HeatBeamSprite();
        void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
        void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events );

        static constexpr int STATE_DURATION = 64;
        static constexpr int TRANS_DURATION = 32;

	private:
        enum class State
        {
            OFF,
            ON
        };
        static constexpr int DELAY_DURATION = STATE_DURATION + TRANS_DURATION;
        static constexpr int SECONDS_TILL_SPEED_UP = 20;
        static constexpr int FRAMES_TILL_SPEED_UP = SECONDS_TILL_SPEED_UP * 60;

		Type type_;
		State state_;
		TimerSwitch state_timer_;
		TimerSimpleT<DELAY_DURATION> timer_start_delay_;
		TimerSimpleT<FRAMES_TILL_SPEED_UP> timer_speed_up_;
};
