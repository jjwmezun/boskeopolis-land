#pragma once

class Block;

#include "sprite.hpp"

class MoonSprite : public Sprite
{
	public:
		MoonSprite( int x, int y );
		~MoonSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
		void render( Camera& camera, bool priority = false ) override;

	private:
		friend class Pebble;
		class Pebble
		{
			public:
				enum class Type
				{
					__NULL,
					SMALL,
					BIG,
					LARGE
				};

				void update();
				void render( const MoonSprite& moon ) const;

			private:
				sdl2::SDLRect dest = { -1, -1, -1, -1 };
				double rotation = 0.0;
				Type type = Type::__NULL;
				int speed = 0;

				void regenerate();
		};

		enum class MoonState
		{
			UNUSED,
			BEFORE_FREEZE,
			FREEZE,
			AFTER_FREEZE,
			RUNNING
		};
		static constexpr int MAX_PEBBLES = 5;

		Pebble pebbles_ [ MAX_PEBBLES ];
		const sdl2::SDLRect small_pebble_src_;
		const sdl2::SDLRect big_pebble_src_;
		const sdl2::SDLRect large_pebble_src_;
		MoonState moon_state_;
		int timer_; // For both moon animation before grabbed & last second if clock hits 0 @ end.
		int animation_counter_;
};