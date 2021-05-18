#pragma once

class Camera;
class EventSystem;
class Health;
class InventoryLevel;
class Level;
class Sprite;

#include "card_suit.hpp"
#include "counter_t.hpp"
#include "frame.hpp"
#include "inventory_level_health.hpp"
#include "mezun_sdl2.hpp"
#include "news_ticker.hpp"
#include "oxygen_meter.hpp"
#include "sprite_graphics.hpp"
#include "texture_box.hpp"
#include "wtext_character.hpp"

class InventoryLevelGraphics final
{
	public:
		static constexpr int HEIGHT = 40;
		static constexpr int Y = Unit::WINDOW_HEIGHT_PIXELS - HEIGHT;
		static constexpr int TOP_ROW_Y_RELATIVE = 10;
		static constexpr int TOP_ROW_Y = Y + TOP_ROW_Y_RELATIVE;
		static constexpr int BOTTOM_ROW_Y = Y + 23;

		enum class Icon
		{
			COWPOKER,
			PRESENT
		};

		InventoryLevelGraphics( int max_hp, bool live_update, int bg_color );
		~InventoryLevelGraphics();
		InventoryLevelGraphics( const InventoryLevelGraphics& ) = delete;
		InventoryLevelGraphics( InventoryLevelGraphics&& ) = delete;
		InventoryLevelGraphics& operator=( const InventoryLevelGraphics& ) = delete;
		InventoryLevelGraphics& operator=( InventoryLevelGraphics&& ) = delete;
		void init( const Level& level, const InventoryLevel& inventory );
		void update( const EventSystem& events, const Health& health, const InventoryLevel& inventory, bool funds_changed, bool time_changed );
		void render( const EventSystem& events, const Sprite& hero, const Camera& camera, const InventoryLevel& inventory );
		void renderBops( const Sprite& hero, const Camera& camera, const InventoryLevel& inventory ) const;
		void setShowCounter( Icon icon, int start_count );
		void changeCounter( int count );
		void forceRerender( const InventoryLevel& inventory );
		void reRenderDiamond();
		void setSuitGotten( CardSuit suit );
		int timeX() const;

	private:
		static constexpr int MAX_MISC_COUNTERS = 3;
		enum class InventoryCounterType
		{
			KEY,
			ON_OFF,
			COUNT
		};
		struct InventoryCounter
		{
			InventoryCounterType type;
			int number;
			sdl2::SDLRect position;
			Icon icon;
		};

		void rerender( const InventoryLevel& inventory );
		void updateHealthGraphics();
		void updatePtsGraphics( const InventoryLevel& inventory );
		void renderPtsGraphics( const InventoryLevel& inventory );
		void updateTimerGraphics( const InventoryLevel& inventory );
		void renderTimerGraphics( const InventoryLevel& inventory );
		void updateCountGraphics( const InventoryCounter& counter );
		void renderCountGraphics( const InventoryCounter& counter );
		void updateKeyGraphics( const InventoryCounter& counter );
		void renderKeyGraphics( const InventoryCounter& counter );
		void updateSwitchGraphics( const InventoryCounter& counter );
		void renderSwitchGraphics( const InventoryCounter& counter );
		void renderDiamond();
		void renderClubSuit();
		void renderDiamondSuit();
		void renderHeartSuit();
		void renderSpadeSuit();
		int getXFromHP() const;
		int colorX() const;
		
		int flashing_timer_;
		int flashing_time_shade_;
		int count_index_;
		int misc_end_x_;
		WTextCharacter::Color text_color_;
		std::vector<InventoryCounter> extra_counters_;
		TextureBox main_texture_;
		InventoryLevelHealth health_gfx_;
		NewsTicker ticker_;
		OxygenMeter oxygen_meter_;
		Frame bg_frame_;
		bool live_;
		int y_;
		int bg_color_;
		char scratch_[2000];
};
