#pragma once

#include "game_state.hpp"
#include "text_obj.hpp"

class MessageState : public GameState
{
	public:
		enum class Type
		{
			CHANGE,
			POP,
			PUSH
		};

		MessageState
		(
			std::string message,
			Type type,
			Palette palette,
			std::unique_ptr<GameState> next_state = nullptr,
			Text::FontColor text_color = Text::FontColor::DARK_GRAY,
			Text::FontColor shadow_color = Text::FontColor::__NULL,
			std::string music = "",
			bool loop_music = true,
			bool flash = false
		);

		~MessageState();
		MessageState( const MessageState& c ) = delete;
		const MessageState& operator=( const MessageState& c ) = delete;

		void stateUpdate() override;
		void stateRender() override;

	private:
		TextObj message_;
		std::unique_ptr<GameState> next_state_;
		const Type type_;
};