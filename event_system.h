


// Name
//===================================
//
// EventSystem
//

#ifndef EVENT_SYSTEM_H
#define EVENT_SYSTEM_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include "palette.h"


// CLASS
//===================================

    class EventSystem
    {
        public:
            EventSystem();
            ~EventSystem();

            bool won_ = false;
            bool failed_ = false;
            bool quit_level_ = false;
            bool change_map_ = false;
            bool message_ = false;
            bool message_lock_ = false;
            bool key_ = true;

            void update();
            void reset();
            void changePalette( Palette::PaletteSet new_palette );
            Palette::PaletteSet getPalette();
            bool paletteChanged() const;

            bool switchOn() const;
            bool switchChanged() const;
            void flipSwitch();


       private:
            bool switch_ = false;
            bool switch_changed_ = false;
            Palette::PaletteSet new_palette_ = { Palette::PaletteType::__NULL, 0 };
            bool palette_changed_ = false;
            void resetPalette();

    };

#endif // EVENT_SYSTEM_H
