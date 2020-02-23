#pragma once

class LevelState;

#include <string>

class Goal
{
	public:
		Goal( std::u32string message );
		virtual ~Goal();
		virtual void update( LevelState& level_state );
		void init( LevelState& level_state );
		virtual void customInit( LevelState& level_state );

	protected:
		static std::u32string formatMessage( std::u32string message, int amount_needed );

	private:
		const std::u32string message_;
};