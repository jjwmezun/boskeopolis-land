




// Name
//===================================
//
// HeatBeamSprite
//

#ifndef HEAT_BEAM_SPRITE_H
#define HEAT_BEAM_SPRITE_H


// FORWARD DECLARATIONS
//===================================

    class Block;
    


// DEPENDENCIES
//===================================

    #include "sprite.hpp"
	#include "timer_switch.hpp"


// CLASS
//===================================

    class HeatBeamSprite : public Sprite
    {
        public:
			enum class Type
			{
				ODD,
				EVEN
			};
			
			enum class State
			{
				OFF,
				ON
			};
		
            HeatBeamSprite( int x, int y, Type type );
            ~HeatBeamSprite();
            void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );
			
			
		private:
			static constexpr int STATE_DURATION = 64;
			static constexpr int TRANS_DURATION = 32;
			static constexpr int DELAY_DURATION = STATE_DURATION + TRANS_DURATION;
			static constexpr int SECONDS_TILL_SPEED_UP = 20;
			static constexpr int FRAMES_TILL_SPEED_UP = SECONDS_TILL_SPEED_UP * 60;
		
			Type type_;
			State state_;
			TimerSwitch state_timer_;
			TimerSimple timer_start_delay_;
			TimerSimple timer_speed_up_;
			
			TimerSwitch timerType( const Type& type ) const;
    };


#endif // HEAT_BEAM_SPRITE_H
