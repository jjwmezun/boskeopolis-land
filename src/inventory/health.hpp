#pragma once

#include "timers/timer_simple_t.hpp"

class Health
{
	public:
		Health();
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

		static int maxHP();


	private:
		int maxOxygen() const;
		int restorePoint() const;
		int losePoint() const;

		bool heater_;
		int hp_;
		int meter_;
		int lose_meter_amount_;
		TimerSimpleT<48, false> invincibility_timer_;
};
