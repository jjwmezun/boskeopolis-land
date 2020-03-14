#pragma once

class BlockSystem;
class Camera;
class Health;
class SpriteSystem;

#include "map_layer.hpp"
#include "camera.hpp"
#include "palette.hpp"
#include "sprite_system.hpp"
#include <vector>
#include "warp.hpp"

class Map final
{
	public:
		enum class LayerType
		{
			__NULL,
			BLOCKS,
			SPRITES,
			BACKGROUND,
			FOREGROUND,
			FADE_FOREGROUND,
			BACKGROUND_IMAGE
		};

		struct LayerInfo
		{
			LayerType type;
			int n;
		};

		const std::vector<Warp> warps_;
		std::vector<std::shared_ptr<MapLayer>> backgrounds_;
		std::vector<std::shared_ptr<MapLayer>> foregrounds_;
		std::vector<int> blocks_;
		std::vector<int> sprites_;
		const std::string tileset_;
		std::string music_;
		const Palette palette_;
		const SpriteSystem::HeroType hero_type_;
		const Camera::Type camera_type_;
		const int width_;
		const int height_;
		const int top_limit_;
		const int bottom_limit_;
		const int left_limit_;
		const int right_limit_;
		const int wind_strength_;
		const int scroll_loop_width_;
		const int lightning_flash_color_;
		const int ui_bg_color_;
		int current_loop_;
		int current_bg_;
		const bool blocks_work_offscreen_;
		const bool loop_sides_;
		const bool slippery_;
		const bool moon_gravity_;
		const bool show_on_off_;
		const bool warp_on_fall_;
		const bool watery_;
		const bool oxygen_;
		const bool hide_;
		bool changed_;

		static Map mapFromPath
		(
			std::string path,
			std::vector<std::unique_ptr<MapLayer>> backgrounds = {},
			std::vector<Warp> warps = {},
			std::vector<std::unique_ptr<MapLayer>> foregrounds = {}
		);
		~Map() noexcept;
		Map( Map&& m ) noexcept;
		Map& operator= ( Map&& m ) = delete;
		Map( const Map& c );
		Map& operator= ( const Map& c ) = delete;

		void update( LevelState& level_state );
		void renderBG( const Camera& camera );
		void renderFG( const Camera& camera );

		const Warp* getWarp( int x_sub_pixels, int y_sub_pixels ) const;

		int widthBlocks() const;
		int heightBlocks() const;
		int widthPixels() const;
		int heightPixels() const;

		int blocksSize() const;
		int spritesSize() const;
		int block( int n ) const;
		int sprite( int n ) const;

		int mapX( int n ) const;
		int mapY( int n ) const;
		int indexFromXAndY( int x, int y ) const;

		const std::string& tileset() const;
		void changeBlock( int where, int value );
		void deleteBlock( int where );
		void deleteSprite( int where );

		void interact( Sprite& sprite, LevelState& level_state );

		bool scrollLoop() const;
		int scrollLoopWidthPixels() const;


	private:
		Map
		(
			std::vector<int> blocks,
			std::vector<int> sprites,
			int width,
			int height,
			std::string tileset,
			Palette palette,
			std::vector<std::unique_ptr<MapLayer>> backgrounds,
			std::vector<Warp> warps,
			std::vector<std::unique_ptr<MapLayer>> foregrounds,
			bool slippery,
			int top_limit,
			int bottom_limit,
			int left_limit,
			int right_limit,
			SpriteSystem::HeroType hero_type,
			int scroll_loop_width,
			Camera::Type camera_type,
			bool blocks_work_offscreen,
			bool loop_sides,
			int wind_strength,
			bool moon_gravity,
			bool show_on_off,
			int lightning_flash_color,
			std::string music,
			bool warp_on_fall,
			int ui_bg_color,
			bool watery,
			bool oxygen,
			bool hide
		);
		static LayerInfo getLayerInfo( const std::string& layer_name );
		void updateLayers( LevelState& level_state );
		void updateLoop( const SpriteSystem& sprites );
		void updateBGColor();
		void renderBGColor() const;
		int scrollLoopWidthBlocks() const;
		int scrollLoopWidthBlocks( int loop ) const;
		int scrollLoopWidthPixels( int loop ) const;
		int leftEdgeOfLoop( int loop ) const;
		int leftEdgeOfLoop() const;
		int rightEdgeOfLoop( int loop ) const;
		int rightEdgeOfLoop() const;
		int getLoopBlocks( int x ) const;
		int getLoopPixels( int x ) const;
		int getXIndexForLoop( int x ) const;
		int spriteLoopPosition( int x ) const;
		bool inBounds( int n ) const;

};
