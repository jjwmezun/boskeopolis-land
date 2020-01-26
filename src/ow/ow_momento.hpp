#pragma once

#include "point.hpp"

class OWMomento
{
    public:
        static OWMomento startOverworld( int previous_level );
        static OWMomento resumeGame();

        constexpr bool showMenu() const { return menu_item_ != -1; };
        constexpr int getMenuItem() const { return menu_item_; };
        constexpr Point getHeroPosition() const { return hero_position_; };
        constexpr int getPreviousLevel() const { return previous_level_; };

    private:
        constexpr OWMomento( int menu_item, int previous_level, Point hero_position )
        :
            menu_item_ ( menu_item ),
            previous_level_ ( previous_level ),
            hero_position_ ( hero_position )
        {};

        int menu_item_;
        int previous_level_;
        Point hero_position_;
}