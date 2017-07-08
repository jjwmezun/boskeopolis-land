

// Name
//===================================
//
// TextComponent
//

#ifndef TEXT_COMPONENT_H
#define TEXT_COMPONENT_H


// FORWARD DECLARATIONS
//===================================

    class Text;


// DEPENDENCIES
//===================================


// CLASS
//===================================

    class TextComponent
    {
        public:
            TextComponent();
            virtual ~TextComponent();
            virtual void update( Text& text ) = 0;
    };

#endif // TEXT_COMPONENT_H

