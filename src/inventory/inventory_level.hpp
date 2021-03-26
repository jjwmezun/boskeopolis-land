#pragma once

class Camera;
class EventSystem;
class Health;
class Level;
class Sprite;

#include "card_suit.hpp"
#include "clock.hpp"
#include "counter_t.hpp"
#include "difficulty.hpp"
#include "inventory_level_graphics.hpp"

class InventoryLevel final
{
	public:
		InventoryLevel( Difficulty difficulty, int max_hp, bool oxygen_upgrade, bool live_update_gfx );
		~InventoryLevel();
		InventoryLevel( const InventoryLevel& ) = delete;
		InventoryLevel( InventoryLevel&& ) = delete;
		InventoryLevel& operator=( const InventoryLevel& ) = delete;
		InventoryLevel& operator=( InventoryLevel&& ) = delete;
		void init( const Level& level );
		void update( EventSystem& events, const Health& health );
		void render( const EventSystem& events, const Sprite& hero, const Camera& camera );
		void renderBops( const Sprite& hero, const Camera& camera ) const;
		void setShowCounter( InventoryLevelGraphics::Icon icon, int start_count );
		void changeCounter( int count );
		void forceRerender();

		int funds() const;
		const Clock& clock() const;
		int mcguffins() const;
		std::u32string fundsString() const;
		int howManyBops() const;
		bool bopsMultiplier() const;
		int howManyGhostKills() const;
		bool multipleGhostKills() const;
		bool isHardMode() const;
		bool haveSuit( CardSuit suit ) const;
	
		void addFunds( int n );
		void loseFunds( int n );
		void setFunds( int n );
		void addMcGuffin();
		void bop();
		void clearBops();
		void addGhostKill();
		void clearGhostKills();
		Clock& clock();
		void setDiamondGotten();
		void getSuit( CardSuit suit );

		static constexpr int  FUNDS_MAX              = 99999;
		static constexpr int  TIME_MAX               = ( 60 * 9 ) + 59;
		static constexpr int  FUNDS_MAX_DIGITS       = 5;
		static constexpr int  FUNDS_SPEED            = 25;
		static constexpr int  MAX_HEART_UPGRADES     = 3;
		static constexpr int  MAX_BOPS               = 8;

	private:
		bool updateFunds();
		void addFundsForMultiplier( int value );

		bool oxygen_upgrade_;
		Difficulty difficulty_;
		int mcguffins_;
		int bops_;
		int ghost_kills_;
		bool suits_[ NUMBER_OF_CARD_SUITS ];
		CounterT<FUNDS_MAX> funds_;
		CounterT<FUNDS_MAX> funds_shown_;
		Clock clock_ = Clock();
		InventoryLevelGraphics graphics_;
		char scratch_[3000];
};
