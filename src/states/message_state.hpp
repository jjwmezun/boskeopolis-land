#pragma once

#include "game_state.hpp"
#include "text.hpp"

class MessageState : public GameState
{
	public:
		MessageState
		(
			std::string message,
			bool pop = false,
			std::unique_ptr<GameState> next_state = nullptr,
			bool push = false,
			const Palette& palette = { "Grayscale", 1 },
			Text::FontColor font_color = Text::FontColor::BLACK
		);

		static std::unique_ptr<MessageState> error
		(
			std::string message,
			bool pop,
			std::unique_ptr<GameState> next_state,
			bool push
		);

		~MessageState();
		MessageState( const MessageState& c ) = delete;
		const MessageState& operator=( const MessageState& c ) = delete;

		void stateUpdate() override;
		void stateRender() override;

	private:
		const Text message_;
		std::unique_ptr<GameState> next_state_;
		const bool pop_;
		const bool push_;
};