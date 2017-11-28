#pragma once

#include "text_component.hpp"
#include "text_component_marquee.hpp"
#include "text_obj.hpp"

class NewsTicker : public TextComponent
{
	public:
		static TextObj make( int y );
		~NewsTicker();
		void update( TextObj& text ) override;

	private:
		NewsTicker();
		static std::string newMessage();
		void changeMessage( TextObj& text );
		
		TextComponentMarquee marquee_;
};