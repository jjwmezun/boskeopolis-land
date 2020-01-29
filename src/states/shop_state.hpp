#pragma once

#include "game_state.hpp"
#include "shop_player.hpp"

class ShopState : public GameState
{
	public:
		ShopState( int shop_number );
		~ShopState();
        ShopState( const ShopState& ) = delete;
        ShopState& operator=( const ShopState& ) = delete;
        ShopState( ShopState&& ) = delete;
        ShopState& operator=( ShopState&& ) = delete;
		void stateUpdate() override;
		void stateRender() override;
		void init() override;

	private:
        ShopPlayer player_;
        char scratch_[3000];
};
