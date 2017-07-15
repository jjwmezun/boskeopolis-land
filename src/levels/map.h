#pragma once

class BlockSystem;
class Camera;
class SpriteSystem;

#include "map_layer.h"
#include "camera.h"
#include "palette.h"
#include "sprite_system.h"
#include <vector>
#include "warp.h"

class Map
{
	public:
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

		void update( EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, const Camera& camera );
		void renderBG( Graphics& graphics, Camera& camera );
		void renderFG( Graphics& graphics, Camera& camera );

		Palette palette() const;
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
		const bool changed() const;
		void setChanged();
		void changeBlock( int where, int value );
		void deleteBlock( int where );

		int topLimit() const;
		int bottomLimit() const;
		int leftLimit() const;
		int rightLimit() const;

		bool slippery() const;		
		SpriteSystem::HeroType heroType() const;
		Camera::XPriority cameraXPriority() const;
		Camera::YPriority cameraYPriority() const;
		bool blocksWorkOffscreen() const;
		
		void interact( Sprite& sprite, Camera& camera );
		
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
			Camera::XPriority camera_x_priority,
			Camera::YPriority camera_y_priority,
			bool blocks_work_offscreen
		);
		std::vector<int> blocks_;
		std::vector<int> sprites_;
		const std::vector<Warp> warps_;
		std::vector<std::shared_ptr<MapLayer>> backgrounds_;
		std::vector<std::shared_ptr<MapLayer>> foregrounds_;
		const Palette palette_;
		const int width_;
		const int height_;
		const std::string tileset_;
		const bool slippery_;
		const int top_limit_;
		const int bottom_limit_;
		const int left_limit_;
		const int right_limit_;
		const SpriteSystem::HeroType hero_type_;
		bool changed_;
		const Camera::XPriority camera_x_priority_;
		const Camera::YPriority camera_y_priority_;
		const bool blocks_work_offscreen_;
		
		static constexpr int LOOP_CHANGE = 3;
		const int scroll_loop_width_;
		int current_loop_;
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