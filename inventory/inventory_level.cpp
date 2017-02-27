


// Name
//===================================
//
// InventoryLevel
//


// DEPENDENCIES
//===================================

    #include "event_system.h"
    #include "game.h"
    #include "inventory_level.h"


// STATIC PROPERTIES
//===================================

	constexpr sdl2::SDLRect InventoryLevel::BG_DEST;
	constexpr sdl2::SDLRect InventoryLevel::GEM_ICON_DEST;
	constexpr sdl2::SDLRect InventoryLevel::CLOCK_ICON_DEST;
	constexpr sdl2::SDLRect InventoryLevel::KEY_ICON_DEST;
	constexpr sdl2::SDLRect InventoryLevel::DIAMOND_ICON_DEST;


// METHODS
//===================================

    InventoryLevel::InventoryLevel()
    :
		inventory_ ( {} ),
		health_ ( VERTICAL_POS ),
        clock_ ( {} ),
		ticker_ ( VERTICAL_POS + 16 )
    {};

    InventoryLevel::InventoryLevel( const InventoryLevel& c )
    :
		health_ ( VERTICAL_POS ),
		inventory_ ( c.inventory_ ),
        clock_ ( {} ),
		ticker_ ( VERTICAL_POS + 16 )
    {};

    InventoryLevel::InventoryLevel( const Inventory c )
    :
		health_ ( VERTICAL_POS ),
		inventory_ ( c ),
        clock_ ( {} ),
		ticker_ ( VERTICAL_POS + 16 )
	{};

    void InventoryLevel::init()
    {
        clock_.reset();
        inventory_.funds_ = 0;
    };

    void InventoryLevel::update( EventSystem& events, Sprite& hero )
    {
        adjustOxygen( hero );

		inventory_.update();
		health_.update( hero );
        clock_.update();
		ticker_.updateTicker();
    };

    void InventoryLevel::render( Graphics& graphics, Level::LevelName level, EventSystem& events )
    {
		// BG
        graphics.renderRect( BG_DEST, 1 );

		// GEMS
        gem_icon_gfx_.render( graphics, GEM_ICON_DEST, nullptr );
        Text::renderNumber( graphics, inventory_.fundsShown(), FUNDS_X, VERTICAL_POS, 5, Text::FontShade::DARK_GRAY );

		// HEALTH
		health_.render( graphics );

		// TIME
        clock_.render( graphics, CLOCK_X, VERTICAL_POS, nullptr, Text::FontShade::DARK_GRAY );
        clock_gfx_.render( graphics, CLOCK_ICON_DEST, nullptr );

		// KEY
        if ( events.key_ )
        {
            key_gfx_.render( graphics, KEY_ICON_DEST, nullptr );
        }

		// SWITCH
        if ( events.switchOn() )
		{
			switch_on_.render( graphics );
		}
		else
		{
			switch_off_.render( graphics );
		}
		
		// TICKER
		ticker_.render( graphics );

		// DIAMOND
        if ( inventory_.haveDiamond( level ) )
        {
            diamond_gfx_.render( graphics, DIAMOND_ICON_DEST, nullptr );
        }

		// OXYGEN
        if ( show_oxygen_ )
        {
            graphics.renderRect( OXYGEN_METER_DEST, oxygen_color_ );
        }
    };

    void InventoryLevel::addFunds( int n )
    {
        inventory_.funds_ += n;
    };

    void InventoryLevel::loseFunds( int n )
    {
        inventory_.funds_ -= n;
    };

    int InventoryLevel::funds() const
    {
        return inventory_.funds_();
    };

    int InventoryLevel::clockTime() const
    {
        return clock_.totalSeconds();
    };

    void InventoryLevel::adjustOxygen( Sprite& hero )
    {
        if ( hero.in_water_ )
        {
            show_oxygen_ = true;
            OXYGEN_METER_DEST.w = round( OXYGEN_METER_WIDTH_PIXELS * hero.status().oxygenPercent() );

            oxygen_color_ = 5;

            if ( hero.status().oxygenPercent() < .25 )
            {
                if ( Game::nextFrame( 2 ) )
                {
                    oxygen_color_ = 2;
                }
            }
        }
        else
        {
            show_oxygen_ = false;
        }
    };

    bool InventoryLevel::haveDiamond( Level::LevelName level ) const
    {
        return inventory_.haveDiamond( level );
    };

    void InventoryLevel::getDiamond( Level::LevelName level )
    {
        inventory_.getDiamond( level );
    };

    Clock& InventoryLevel::clock()
    {
        return clock_;
    };

    void InventoryLevel::setFunds( int n )
    {
        inventory_.funds_ = n;
    };

    const Inventory& InventoryLevel::inventory() const
    {
        return inventory_;
    };

    void InventoryLevel::won( Level::LevelName level )
    {
        inventory_.victories_.at( level ) = true;
        inventory_.setGemScore( level );
        inventory_.setTimeScore( level, clockTime() );

        inventory_.total_funds_ += inventory_.funds_;
        inventory_.funds_ = 0;
        inventory_.save();
    };

    void InventoryLevel::failed()
    {
        inventory_.total_funds_ -= inventory_.PRICE_OF_DEATH;
        inventory_.save();
    };

    void InventoryLevel::quit( Level::LevelName level )
    {
        if ( !inventory_.victories_.at( level ) )
        {
            failed();
        }
    };
