#pragma once

class Camera;
class EventSystem;
class Health;
class Map;
class Sprite;

#include "inventory_level_health.hpp"
#include "mezun_sdl2.hpp"
#include "news_ticker.hpp"
#include "oxygen_meter.hpp"
#include "sprite_graphics.hpp"
#include "text_obj.hpp"
#include "texture_box.hpp"

class InventoryLevel
{
	public:
		static constexpr int HEIGHT = 40;
		static constexpr int Y = Unit::WINDOW_HEIGHT_PIXELS - HEIGHT;
		static constexpr int TOP_ROW_Y_RELATIVE = 10;
		static constexpr int TOP_ROW_Y = Y + TOP_ROW_Y_RELATIVE;
		static constexpr int BOTTOM_ROW_Y = Y + 23;
		static constexpr int CLOCK_ICON_X = Unit::MiniBlocksToPixels( 17 );
		static constexpr int CLOCK_X = 146;

		InventoryLevel();
		~InventoryLevel();
		InventoryLevel( const InventoryLevel& ) = delete;
		InventoryLevel( InventoryLevel&& ) = delete;
		InventoryLevel& operator=( const InventoryLevel& ) = delete;
		InventoryLevel& operator=( InventoryLevel&& ) = delete;
		void init( const Map& lvmap );
		void update( EventSystem& events, const Health& health );
		void render( const EventSystem& events, const Sprite& hero, const Camera& camera );
		void setShowMcGuffins();
		void changeKillCounter( int count );
		void forceRerender();

	private:
		void updateHealthGraphics();
		void updatePtsGraphics();
		void renderPtsGraphics();
		void updateTimerGraphics();
		void renderTimerGraphics();
		void updateKillCountGraphics();
		void renderKillCountGraphics();
		void updateKeyGraphics();
		void renderKeyGraphics();
		void updateSwitchGraphics();
		void renderSwitchGraphics();
		void updateMcGuffinGraphics();
		void renderMcGuffinGraphics();
		
		bool showing_key_;
		bool show_on_off_;
		bool on_off_state_;
		int flashing_timer_;
		int flashing_time_shade_;
		int kill_count_;
		int mcguffins_to_render_;
		TextureBox main_texture_;
		InventoryLevelHealth health_gfx_;
		NewsTicker ticker_;
		OxygenMeter oxygen_meter_;
};
