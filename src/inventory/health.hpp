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

		bool inWater() const;
		bool submerged() const;
		void goInWater();
		void outOfWater();
		void submerge();
		bool drowned() const;
		double oxygenPercent() const;

		static int maxHP();
		static int maxOxygen();
		static int restorePoint();


	private:
		static constexpr int NORMAL_OXYGEN_LIMIT = 720;
		static constexpr int STRONGER_OXYGEN_LIMIT = 720 + 360;
		static constexpr int START_MAX_HP = 2;

		int hp_;
		int oxygen_meter_;
		bool invincible_;
		bool in_water_;
		bool submerged_in_water_;
		TimerSimple invincibility_timer_;
};