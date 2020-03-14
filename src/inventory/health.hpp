#pragma once

#include "difficulty.hpp"
#include "timers/timer_simple_t.hpp"

class Health final
{
	public:
		static constexpr int START_MAX_HP = 2;

		Health( Difficulty difficulty, int heart_upgrades, bool has_oxygen_upgrade );
		void update();
		bool flickerOff() const;

		int hp() const;
		void hurt();
		void heal();
		void fullHeal();

		void refillOxygen();
		bool losingMeter() const;
		void submerge( int amount = 1 );
		void heatUp();
		bool drowned() const;
		bool overheated() const;
		double oxygenPercent() const;
		bool hasFullHealth() const;
		int getInvincibilityCounter() const;
		int maxHP() const;


	private:
		int maxOxygen() const;
		int restorePoint() const;
		int losePoint() const;

		bool heater_;
		bool has_oxygen_upgrade_;
		int hp_;
		int max_hp_;
		int meter_;
		int lose_meter_amount_;
		TimerSimpleT<48, false> invincibility_timer_;
};
