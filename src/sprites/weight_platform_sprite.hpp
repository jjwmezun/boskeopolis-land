#pragma once

#include "sprite.hpp"

class WeightPlatformSprite : public Sprite
{
	public:
		WeightPlatformSprite( int x, int y, Direction::Horizontal direction );
		~WeightPlatformSprite();
		void customUpdate( LevelState& level_state ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void render( Camera& camera, bool priority ) override;

	private:
		int break_timer_;
		sdl2::SDLRect* broken_higher_bar_;
		double wheel_rotation_;
		sdl2::SDLRect left_;
		sdl2::SDLRect right_;

		bool isBroken() const;
		void renderPlatforms( const Camera& camera ) const;
		void renderBars( const Camera& camera ) const;
		void renderWheels( const Camera& camera ) const;
		void renderLeftPlatform( const Camera& camera ) const;
		void renderRightPlatform( const Camera& camera ) const;
		void renderPlatform( const Camera& camera, const sdl2::SDLRect& side ) const;
		void renderTopBar( const Camera& camera ) const;
		void renderLeftBar( const Camera& camera ) const;
		void renderRightBar( const Camera& camera ) const;
		void renderSideBar( const Camera& camera, int x_offset, const sdl2::SDLRect& side ) const;
		void renderLeftWheel( const Camera& camera ) const;
		void renderRightWheel( const Camera& camera ) const;
		void renderWheel( const Camera& camera, int x_offset ) const;
		void renderScore( const Camera& camera ) const;

		constexpr int wheelMachineY() const
		{
			return Unit::SubPixelsToPixels( original_hit_box_.y ) - 10;
		}
};
