#pragma once

#include "texture_box.hpp"

class NewsTicker final
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
		void generateNewNewsItem();
		int right() const;
		bool testTickerIsCutOffOnLeftSide() const;
		bool testTickerHasNotReachedScreenYet() const;

		int y_;
		int w_;
		int x_;
		int texture_width_;
		SDL_Texture* texture_;
		std::u32string selected_message_;
		sdl2::SDLRect src_;
		sdl2::SDLRect dest_;
};