#pragma once

#include "game_state.hpp"
#include <memory>
#include "texture_box.hpp"
#include "wtext_obj.hpp"

class WMessageState final : public GameState
{
	public:
		enum class Type
		{
			CHANGE,
			POP,
			PUSH
		};

		WMessageState
		(
			WTextObj::MessageData message,
			Type type,
			Palette palette,
			std::unique_ptr<GameState> next_state = nullptr,
			std::string music = "",
			bool loop_music = true,
			bool flash = false
		);

		~WMessageState();
		WMessageState( WMessageState&& c ) = delete;
		const WMessageState& operator=( WMessageState&& c ) = delete;
		WMessageState( const WMessageState& c ) = delete;
		const WMessageState& operator=( const WMessageState& c ) = delete;

		static std::unique_ptr<WMessageState> generateErrorMessage( std::u32string message, Type type, std::unique_ptr<GameState> next_state );

        void init() override;
		void stateUpdate() override;
		void stateRender() override;

	private:
        WTextObj text_;
		TextureBox text_gfx_;
		std::unique_ptr<GameState> next_state_;
		const Type type_;
};
