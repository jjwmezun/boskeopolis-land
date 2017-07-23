



// Name
//===================================
//
// SwitchGraphics
//


// DEPENDENCIES
//===================================

    #include "switch_graphics.hpp"


// STATIC PROPERTIES
//===================================


// METHODS
//===================================

    SwitchGraphics::SwitchGraphics
    (
        std::string&& texture,
        std::pair<int, int> frames_on,
        std::pair<int, int> frames_off,
        bool flip_x,
        bool flip_y,
        double rotation,
        bool priority,
        int x_adjustment,
        int y_adjustment,
        int w_adjustment,
        int h_adjustment
    )
    :
        SpriteGraphics( std::forward<std::string> ( texture ), 0, 0, flip_x, flip_y, rotation, priority, x_adjustment, y_adjustment, w_adjustment, h_adjustment )
    {};

    SwitchGraphics::~SwitchGraphics() {};

    void SwitchGraphics::update()
    {
    };

    void SwitchGraphics::setOn()
    {
        current_frame_x_ = std::get< X_INDEX >( frames_on_ );
        current_frame_y_ = std::get< Y_INDEX >( frames_on_ );
    };

    void SwitchGraphics::setOff()
    {
        current_frame_x_ = std::get< X_INDEX >( frames_off_ );
        current_frame_y_ = std::get< Y_INDEX >( frames_off_ );
    };
