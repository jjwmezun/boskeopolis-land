#pragma once

#include "timers/timer_simple.hpp"

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

		static int maxHP();


	private:
		static constexpr int NORMAL_OXYGEN_LIMIT = 720;
		static constexpr int STRONGER_OXYGEN_LIMIT = 720 + 360;
		static constexpr int HEAT_LIMIT = 500;
		static constexpr int START_MAX_HP = 2;

		int hp_;
		bool heater_;
		int meter_;
		bool invincible_;
		int lose_meter_amount_;
		TimerSimple invincibility_timer_;

		int maxOxygen() const;
		int restorePoint() const;
		int losePoint() const;
};
