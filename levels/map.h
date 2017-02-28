

// Name
//===================================
//
// Map
//

#ifndef MAP_H
#define MAP_H


// FORWARD DECLARATIONS
//===================================

    class SpriteSystem;
    class WaterEffect;


// DEPENDENCIES
//===================================

    #include "map_layer.h"
    #include "camera.h"
    #include "palette.h"
    #include "sprite_system.h"
    #include <vector>
    #include "warp.h"


// CLASS
//===================================

    class Map
    {
        public:
            static Map mapFromPath
            (
                std::string path,
                Palette::PaletteSet palette = { Palette::PaletteType::CLASSIC_GREEN, 2 },
                std::vector<MapLayer*> backgrounds = {},
                std::vector<Warp> warps = {},
                std::vector<MapLayer*> foregrounds = {}
            );
            ~Map() noexcept;
            Map( Map&& m ) noexcept;
            Map& operator= ( Map&& m ) = delete;
            Map( const Map& c );
            Map& operator= ( const Map& c ) = delete;

            void update();
            void renderBG( Graphics& graphics, Camera& camera );
            void renderFG( Graphics& graphics, Camera& camera );
			
            Palette::PaletteSet palette() const;
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
			
            const bool changed() const;
            void changeBlock( int where, int value );
            void deleteBlock( int where );
			
            bool slippery() const;
            const WaterEffect* effect();
            int topLimit() const;
            int bottomLimit() const;
            int leftLimit() const;
            int rightLimit() const;
            bool scrollLoop() const;
			int scrollLoopWidth() const;
            SpriteSystem::HeroType heroType() const;
            Camera::XPriority cameraXPriority() const;
            Camera::YPriority cameraYPriority() const;
            bool blocksWorkOffscreen() const;


        private:

            Map
            (
                std::vector<int> blocks,
                std::vector<int> sprites,
                int width,
                int height,
                Palette::PaletteSet palette,
                std::vector<MapLayer*> backgrounds,
                std::vector<Warp> warps,
                std::vector<MapLayer*> foregrounds,
                bool slippery,
                int top_limit,
                int bottom_limit,
                int left_limit,
                int right_limit,
                SpriteSystem::HeroType hero_type,
                int water_effect_height_blocks,
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
            const Palette::PaletteSet palette_;
            const int width_;
            const int height_;
            const bool slippery_;
            const int top_limit_;
            const int bottom_limit_;
            const int left_limit_;
            const int right_limit_;
            const SpriteSystem::HeroType hero_type_;
            std::unique_ptr<WaterEffect> water_effect_;
            bool changed_;
            const int scroll_loop_width_;
            const Camera::XPriority camera_x_priority_;
            const Camera::YPriority camera_y_priority_;
            const bool blocks_work_offscreen_;

            bool inBounds( int n ) const;
            WaterEffect* testWaterEffect( int n ) const;
    };

#endif // MAP_H
