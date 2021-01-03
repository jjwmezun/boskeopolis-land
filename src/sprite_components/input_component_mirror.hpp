#pragma once

#include <array>
#include "input.hpp"
#include "input_component.hpp"
#include <vector>

class InputComponentMirror final : public InputComponent
{
	public:
        InputComponentMirror( const std::vector<std::array<bool, Input::NUM_O_ACTIONS>>* list );
		~InputComponentMirror();
		void update() override;
		void reset() override;
		bool action1() override;
		bool action2() override;
		bool action3() override;
		bool up() override;
		bool down() override;
		bool left() override;
		bool right() override;
		bool cUp() override;
		bool cDown() override;
		bool cLeft() override;
		bool cRight() override;

    private:
        int current_;
        const std::vector<std::array<bool, Input::NUM_O_ACTIONS>>* list_;
        char scratch_[3000];

        bool testFrame( Input::Action action ) const;
};