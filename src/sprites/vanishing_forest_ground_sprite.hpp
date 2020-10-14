#pragma once

#include "sprite.hpp"
#include "texture_box.hpp"

class VanishingForestGroundSprite final : public Sprite
{
	public:
		VanishingForestGroundSprite( int x, int y, int length );
		~VanishingForestGroundSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
        void render( const Camera& camera ) const override;
		void reset() override;

    private:
		enum class GroundState
		{
			NORMAL,
			FLASHING,
			GONE
		};
		bool texture_created_;
		TextureBox texture_;
		Uint8 alpha_;
		GroundState ground_state_;
		int timer_;
        char scatch_[ 3000 ];

		bool doHeroCollision( Sprite& them ) const;
};
