#pragma once

class Camera;
class EventSystem;
class Health;
class Map;
class Sprite;

#include "clock.hpp"
#include "counter_t.hpp"
#include "difficulty.hpp"
#include "inventory_level_graphics.hpp"

class InventoryLevel final
{
	public:
		InventoryLevel( Difficulty difficulty, int max_hp, bool oxygen_upgrade );
		~InventoryLevel();
		InventoryLevel( const InventoryLevel& ) = delete;
		InventoryLevel( InventoryLevel&& ) = delete;
		InventoryLevel& operator=( const InventoryLevel& ) = delete;
		InventoryLevel& operator=( InventoryLevel&& ) = delete;
		void init( const Map& lvmap );
		void update( EventSystem& events, const Health& health );
		void render( const EventSystem& events, const Sprite& hero, const Camera& camera );
		void renderBops( const Sprite& hero, const Camera& camera ) const;
		void setShowMcGuffins();
		void changeKillCounter( int count );
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
		CounterT<FUNDS_MAX> funds_;
		CounterT<FUNDS_MAX> funds_shown_;
		Clock clock_ = Clock();
		InventoryLevelGraphics graphics_;
};
