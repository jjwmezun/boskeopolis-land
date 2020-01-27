#pragma once

#include "texture_box.hpp"

class NewsTicker
{
	public:
		NewsTicker( int y );
		~NewsTicker();
		NewsTicker( const NewsTicker& ) = delete;
		NewsTicker( NewsTicker&& ) = delete;
		NewsTicker& operator=( const NewsTicker& ) = delete;
		NewsTicker& operator=( NewsTicker&& ) = delete;
		void update();
		void init();
		void render() const;
		void forceRedraw();

	private:
		void recalculateWidth();

		int y_;
		int w_;
		TextureBox texture_;
		std::u32string selected_message_;
		char scratch_[ 3000 ];
};