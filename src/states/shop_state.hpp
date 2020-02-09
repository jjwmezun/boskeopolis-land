#pragma once

#include "game_state.hpp"
#include "shop_item.hpp"
#include "shop_player.hpp"
#include "wtext_gradual.hpp"

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
			CHOOSING,
			CONFIRM,
			WAITING_FOR_CHECKOUT_THANKS_TO_FINISH,
			WAITING_FOR_LEAVING_TEXT_TO_FINISH,
			LEAVING
		};
		static constexpr int NUMBER_OF_ITEMS = 2;

		bool testCartIsEmpty() const;
		bool testSelectingCheckout() const;
		bool testSelectingExit() const;
		int calculateCartTotal() const;
		void changeDialogue( const std::u32string& text );
		void renderDialogueBox();
		void setItemDescriptionText();

        ShopPlayer player_;
		State state_;
        CounterFlip<NUMBER_OF_ITEMS-1, -2> item_selection_;
		ShopItem items_[ NUMBER_OF_ITEMS ];
		int input_delay_;
		bool cart_[ NUMBER_OF_ITEMS ];
		WTextGradual<true> dialogue_;
		int cart_total_shown_;
		CounterFlip<1> confirm_selection_;
		WTextGradual<false> item_description_;
        char scratch_[3000];
};
