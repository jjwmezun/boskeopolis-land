#pragma once

class Block;    
class Sprite;

#include "input_component_player.hpp"
#include "sprite.hpp"

class PlayerSprite : public Sprite
{
	public:
		PlayerSprite
		(
			int x,
			int y,
			std::unique_ptr<InputComponent> input
				= std::unique_ptr<InputComponentPlayer>
				(),
			std::string&& texture
				= "sprites/autumn.png",
			SpriteType type = SpriteType::HERO,
			int start_speed = 160,
			int top_speed = 2000,
			int jump_start_speed = 1000,
			int jump_top_speed = 6000,
			bool permanent = true
		);
		~PlayerSprite();

		virtual void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
		virtual void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health );
		void duck();
		void unduck();

	protected:
		void deathAction( Camera& camera );
		void actions();
		std::unique_ptr<InputComponent> input_;

	private:
		void testVX();
		void testFrameRate();
		bool door_lock_;
};