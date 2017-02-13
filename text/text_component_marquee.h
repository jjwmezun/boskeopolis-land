


// Name
//===================================
//
// TextComponentMarquee
//

#ifndef TEXT_COMPONENT_MARQUEE_H
#define TEXT_COMPONENT_MARQUEE_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include "direction.h"
    #include "text_component.h"


// CLASS
//===================================

    class TextComponentMarquee : public TextComponent
    {
        public:
            TextComponentMarquee
            (
                Direction::Horizontal direction = Direction::Horizontal::LEFT,
                int speed = 1
            );
            ~TextComponentMarquee();
            void update( Text& text );

        private:
            const Direction::Horizontal direction_;
            const int speed_;
    };

#endif // TEXT_COMPONENT_MARQUEE_H

