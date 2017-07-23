

// Name
//===================================
//
// NewsTicker
//

#ifndef NEWS_TICKER_H
#define NEWS_TICKER_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include "text.hpp"


// CLASS
//===================================

    class NewsTicker : public Text
    {
		public:
			NewsTicker( int y );
			void updateTicker();
	
		private:
            const std::vector<std::string> strings_ =
            {
                "Kitties Want Answers In Possible Kitty Kibble Shortage",
                "Thank you for reading this message. This message loves you.",
                "Clean up on aisle Delfino.",
                "Experts declare, SimCity 3000 Marquee + platformer = bad Main:: 9/10.",
                "NEWSFLASH: Crazy woman running round grabbing floating gems everywhere she goes. Details @ 11.",
                "¡AHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH!",
                "WARNING: Playing games for too long can be dangerous. Take 15-minute breaks 'tween each session or we'll come to smash your hand with a hammer-& yes, Jacob, we'll find you @ 2456 Acorn Avenue. We know where you live.",
                "NEWSFLASH: Despite all o' my rage, I am still just a rat in a cage. Details @ 11.",
                "Thou shalt not waste thine time reading these dumb newsflashes 'stead o' playing the game-¡Look out for that Bad Apple! ¡You fool!",
                "If you don't know 'bout the tavern, then it is time."
            };
			
			std::string newMessage() const;
			void changeMessage();
    };

#endif // NEWS_TICKER_H

