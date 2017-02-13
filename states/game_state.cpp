

// Name
//===================================
//
// GameState
//


// DEPENDENCIES
//===================================

    #include "game_state.h"
    #include "graphics.h"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    GameState::GameState
    (
        StateID id,
        Palette::PaletteSet palette
    )
    :
        id_ ( id ),
        palette_ ( palette )
    {};

    GameState::~GameState() {};

    void GameState::render( Graphics& graphics )
    {
        stateRender( graphics );
    };

    void GameState::changePalette( Graphics& graphics )
    {
        graphics.newPalette( palette_ );
    };

    void GameState::newPalette( Graphics& graphics, Palette::PaletteSet palette )
    {
        if ( palette_ != palette )
        {
            palette_ = palette;
            changePalette( graphics );
        }
    };

    GameState::StateID GameState::id() const
    {
        return id_;
    };
