


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

            bool won_ = false;
            bool failed_ = false;
            bool quit_level_ = false;
            bool change_map_ = false;
            bool message_ = false;
            bool message_lock_ = false;
            bool key_ = true;

            void update();
            void reset();
            void changePalette( const Palette& new_palette );
            Palette getPalette();
            bool paletteChanged() const;

            bool switchOn() const;
            bool switchChanged() const;
            void flipSwitch();
			
			int move_water_ = -1;
			bool waterShouldMove() const;


       private:
	   		static constexpr int WATER_NULL = -1;
	   
            bool switch_;
            bool switch_changed_;
            Palette new_palette_;
            bool palette_changed_;
			
            void resetPalette();

    };

#endif // EVENT_SYSTEM_H
