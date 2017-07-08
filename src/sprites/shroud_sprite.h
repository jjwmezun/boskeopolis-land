#ifndef SHROUD_SPRITE_H
#define SHROUD_SPRITE_H

class Block;
class Input;

#include "counter.h"
#include "sprite.h"

class ShroudSprite : public Sprite
{
	public:
		ShroudSprite( int x, int y );
		~ShroudSprite();
		void customUpdate( const Input& input, Camera& camera, Map& lvmap, Game& game, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks );
		void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap );
		void render( Graphics& graphics, Camera& camera, bool priority = false );
		
	private:
		enum class ShroudState
		{
			VISIBLE,
			HIDING,
			HIDDEN,
			RETURNING
		};
		
		static constexpr int COLOR = 5;
		static constexpr int TRANS_SPEED = 10;
	
		const sdl2::SDLRect top_;
		const sdl2::SDLRect bottom_;
		ShroudState state_;
		Counter alpha_;
		bool activated_;
};


#endif // SHROUD_SPRITE_H
