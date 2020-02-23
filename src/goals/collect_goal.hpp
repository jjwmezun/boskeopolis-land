#pragma once

#include "goal.hpp"

class CollectGoal : public Goal
{
	public:
		static constexpr int DEFAULT_AMOUNT = 10000;
		CollectGoal( std::u32string message, int amount_needed = DEFAULT_AMOUNT );
		~CollectGoal();
		void update( LevelState& level_state ) override;

	private:
		const int amount_needed_;
};
