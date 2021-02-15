#pragma once

#include "goal.hpp"

class McGuffinGoal final : public Goal
{
	public:
		static constexpr int DEFAULT_AMOUNT_NEEDED = 3;
		McGuffinGoal( std::u32string message, int amount_needed = DEFAULT_AMOUNT_NEEDED );
		~McGuffinGoal();
		void update( LevelState& level_state ) override;
		void customInit( LevelState& level_state ) override;

	private:
		const int amount_needed_;
		int amount_;
};
