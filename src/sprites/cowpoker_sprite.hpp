#pragma once

#include "sprite.hpp"

class CowpokerSprite : public Sprite
{
	public:
		static std::unique_ptr<Sprite> TallCowpokerSprite( int x, int y, int map_id );
		static std::unique_ptr<Sprite> ShortCowpokerSprite( int x, int y, int map_id );
		~CowpokerSprite();
		void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health ) override;
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events ) override;
	
	private:
		enum class CowpokerType
		{
			SHORT,
			TALL,
			STRONGER
		};

		CowpokerSprite( int x, int y, int w, int h, CowpokerType type, std::unique_ptr<SpriteGraphics>&& gfx, int map_id );
		CowpokerType type_;

		int throw_time_;
		int throw_counter_;
		bool is_shooting_;
		int is_shooting_counter_;

		int throwTime() const;
		void handleThrowing( SpriteSystem& sprites );
};