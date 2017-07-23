#ifndef MESSAGE_STATE_H
#define MESSAGE_STATE_H

#include "game_state.h"
#include "text.h"

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
			Text::FontShade font_color = Text::FontShade::BLACK
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

		void update( Game& game, const Input& input );
		void stateRender();
		void init( Game& game );
		void backFromPop( Game& game );

	private:
		const Text message_;
		const bool pop_;
		std::unique_ptr<GameState> next_state_;
		const bool push_;
		const Text::FontShade font_color_;
};

#endif // MESSAGE_STATE_H
