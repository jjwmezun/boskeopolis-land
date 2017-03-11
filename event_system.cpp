



// Name
//===================================
//
// EventSystem
//


// DEPENDENCIES
//===================================

    #include "event_system.h"
	#include "mezun_helpers.h"


// STATIC PROPERTIES
//===================================

// METHODS
//===================================

    EventSystem::EventSystem()
	:
		switch_ (false ),
		switch_changed_ (false ),
		new_palette_ ( mezun::emptyString(), 0 ),
		palette_changed_ ( false ),
		move_water_ ( -1 )
    {};

    void EventSystem::reset()
    {
        won_ = false;
        failed_ = false;
        quit_level_ = false;
        message_ = false;
        message_lock_ = false;
        key_ = false;
        switch_ = false;
		move_water_ = -1;

        resetPalette();
    };

    void EventSystem::changePalette( const Palette& new_palette )
    {
        new_palette_ = new_palette;
        palette_changed_ = true;
    };

    Palette EventSystem::getPalette()
    {
        const auto temp = new_palette_;
        resetPalette();
        return temp;
    };

    bool EventSystem::paletteChanged() const
    {
        return palette_changed_;
    };

    void EventSystem::resetPalette()
    {
        new_palette_ = { mezun::emptyString(), 0 };
        palette_changed_ = false;
    };

    bool EventSystem::switchOn() const
    {
        return switch_;
    };

    bool EventSystem::switchChanged() const
    {
        return switch_changed_;
    };

    void EventSystem::flipSwitch()
    {
        switch_ = !switch_;
        switch_changed_ = true;
    };

    void EventSystem::update()
    {
        switch_changed_ = false;
		
		if ( waterShouldStop() )
		{
			move_water_ = WATER_NULL;
		}
    };

	bool EventSystem::waterShouldMove() const
	{
		return move_water_ != WATER_NULL;
	};

	bool EventSystem::waterShouldStop() const
	{
		return move_water_ == current_water_;
	};