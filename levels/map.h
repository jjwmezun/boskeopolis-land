

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
            //Map& operator= ( Map&& m ) = delete;
            Map( const Map& c );
            //Map& operator= ( const Map& c ) = delete;

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
            bool slippery() const;
            const bool changed() const;
            void changeBlock( int where, int value );
            void deleteBlock( int where );
            void update();
            Palette::PaletteSet palette() const;
            void renderBG( Graphics& graphics, Camera& camera );
            void renderFG( Graphics& graphics, Camera& camera );
            WaterEffect* effect();
            const Warp* getWarp( int x_sub_pixels, int y_sub_pixels ) const;
            int topLimit() const;
            int bottomLimit() const;
            int leftLimit() const;
            int rightLimit() const;
            SpriteSystem::HeroType heroType() const;
            bool scrollLoop() const;
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
                Palette::PaletteSet palette = { Palette::PaletteType::CLASSIC_GREEN, 2 },
                std::vector<MapLayer*> backgrounds = {},
                std::vector<Warp> warps = {},
                std::vector<MapLayer*> foregrounds = {},
                bool slippery = false,
                int top_limit = -1,
                int bottom_limit = -1,
                int left_limit = -1,
                int right_limit = -1,
                SpriteSystem::HeroType hero_type = SpriteSystem::HeroType::NORMAL,
                int water_effect_height_blocks = -1,
                bool scroll_loop = false,
                Camera::XPriority camera_x_priority = Camera::XPriority::__NULL,
                Camera::YPriority camera_y_priority = Camera::YPriority::__NULL,
                bool blocks_work_offscreen = false
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
            const bool scroll_loop_;
            const Camera::XPriority camera_x_priority_;
            const Camera::YPriority camera_y_priority_;
            const bool blocks_work_offscreen_;

            bool inBounds( int n ) const;
            WaterEffect* testWaterEffect( int n ) const;
    };

#endif // MAP_H
