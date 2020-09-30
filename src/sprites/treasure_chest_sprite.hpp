#pragma once

class InventoryLevel;

#include "sprite.hpp"

class TreasureChestSprite : public Sprite
{
	public:
		enum class ItemType
		{
			KEY_KEYCANE,
			RANDOM_KEYCANE,
			RANDOM_GEMS
		};
		TreasureChestSprite( int x, int y, ItemType item_type = ItemType::KEY_KEYCANE );
		~TreasureChestSprite();
		virtual void customUpdate( LevelState& level_state ) override;
		virtual void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, LevelState& level_state ) override;
		void render( const Camera& camera ) const override;

	protected:
		const sdl2::SDLRect BOTTOM;
		ItemType item_type_;
		int key_cane_height_;
		int key_cane_y_;
		int key_cane_flash_frame_;
		int timer_;
		int gem_count_;

		void renderKeyCane( const Camera& camera ) const;
		void renderChestBottomHalf( const Camera& camera ) const;
		void renderChestTopHalf( const Camera& camera ) const;
		void animateChestOpening();
		void raiseItem( InventoryLevel& inventory );
		int getAnimationFrame( int speed ) const;
		void setKeyCanePosition();
		void setKeyCaneFlash();
		void setKeyCaneHeight();
		bool isRandomKeyCaneAtTop() const;
		sdl2::SDLRect getKeyCaneGFXPosition() const;
		Object getKeyCanePosition() const;

		void setItemGFXToGem();
		void setKeyCaneGFXFlash( bool flash );
};
