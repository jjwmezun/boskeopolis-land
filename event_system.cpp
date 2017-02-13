



// Name
//===================================
//
// EventSystem
//


// DEPENDENCIES
//===================================

    #include "event_system.h"


// STATIC PROPERTIES
//===================================

// METHODS
//===================================

    EventSystem::EventSystem
    (
    )
    {};

    EventSystem::~EventSystem() {};

    void EventSystem::reset()
    {
        won_ = false;
        failed_ = false;
        quit_level_ = false;
        message_ = false;
        message_lock_ = false;
        key_ = false;
        switch_ = false;

        resetPalette();
    };

    void EventSystem::changePalette( Palette::PaletteSet new_palette )
    {
        new_palette_ = new_palette;
        palette_changed_ = true;
    };

    Palette::PaletteSet EventSystem::getPalette()
    {
        const Palette::PaletteSet temp = new_palette_;
        resetPalette();
        return temp;
    };

    bool EventSystem::paletteChanged() const
    {
        return palette_changed_;
    };

    void EventSystem::resetPalette()
    {
        new_palette_ = { Palette::PaletteType::__NULL, 0 };
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
    };
