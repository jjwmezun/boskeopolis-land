


// Name
//===================================
//
// Inventory
//

#ifndef INVENTORY_H
#define INVENTORY_H


// FORWARD DECLARATIONS
//===================================

    class Graphics;


// DEPENDENCIES
//===================================

    #include "counter.h"
    #include "level.h"


// CLASS
//===================================

    class Inventory
    {
        public:

			static constexpr bool DEFAULT_VICTORY    = false;
			static constexpr bool DEFAULT_DIAMOND    = false;
			static constexpr int  DEFAULT_GEM_SCORE  = -1;
			static constexpr int  DEFAULT_TIME_SCORE = -1;
            static constexpr int  PRICE_OF_DEATH = 2500;

			// Methods
            Inventory();
            Inventory( const Inventory& c );

            bool haveDiamond( Level::LevelName level ) const;
            void getDiamond( Level::LevelName level );

			std::string gemScore( Level::LevelName level ) const;
			void setGemScore( Level::LevelName level );

			std::string timeScore( Level::LevelName level ) const;
			void setTimeScore( Level::LevelName level, int time );

            std::vector<bool> victories_;
            bool victory( Level::LevelName level ) const;

			int fundsShown() const;
			int totalFundsShown() const;

			void update();
            void save();
            void load();


			// Members
            Counter funds_;
            Counter total_funds_;

				// Upgrades:
				int heart_upgrades_ = 0;
				bool oxygen_upgrade_ = false;


        private:
            static constexpr int FUNDS_MAX       = 99999;
            static constexpr int TIME_MAX = ( 60 * 9 ) + 59;
            static constexpr int TOTAL_FUNDS_MAX = 999999999;
            static constexpr int TOTAL_FUNDS_MIN = -99999999;
			static constexpr int FUNDS_MAX_DIGITS = 5;
			static constexpr int TOTAL_FUNDS_MAX_DIGITS = 9;
            static constexpr int FUNDS_SPEED = 25;
            static constexpr int TOTAL_FUNDS_SPEED = 100;

            std::vector<bool> diamonds_;
            std::vector<Counter> gem_scores_;
            std::vector<Counter> time_scores_;
            Counter funds_shown_;
            Counter total_funds_shown_;

            void saveJSON();
            void saveBinary();
            void loadJSON();
            void loadBinary();
    };

#endif // INVENTORY_H

