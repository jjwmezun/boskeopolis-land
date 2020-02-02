#pragma once

#include "game_state.hpp"
#include "shop_item.hpp"
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
		enum class State
		{
			WALKING_IN,
			CHOOSING
		};
		static constexpr int NUMBER_OF_ITEMS = 2;

        ShopPlayer player_;
		State state_;
        CounterFlip<NUMBER_OF_ITEMS-1, -1> item_selection_;
		ShopItem items_[ NUMBER_OF_ITEMS ];
		int input_delay_;
		std::vector<int> cart_;
        char scratch_[3000];
};
