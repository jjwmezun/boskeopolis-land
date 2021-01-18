#pragma once

#include "level.hpp"
#include <vector>
#include <string>

class LevelData;

class LevelList final
{
    public:
		static constexpr int MAX_LEVELS = 256;
        static void init();
        static Level getLevel( int id );
		static const std::vector<LevelData>& getLevelDataList();
		static int getNumberOfLevels();
        static int gemChallenge( unsigned int level );
        static int timeChallenge( unsigned int level );
        static std::u32string gemChallengeText( unsigned int level );
        static std::u32string timeChallengeText( unsigned int level );
        static bool hasSecretGoal( unsigned int level );
		static int getIDFromCodeName( std::string code_name );
		static const std::string& getCodeNameFromID( unsigned int level );
		static int getIDbyCycleAndTheme( unsigned int cycle, unsigned int theme );
		static const std::string& getCodeNameByCycleAndTheme( unsigned int cycle, unsigned int theme );
		static int getSpecialLevelID( unsigned int number );
		static int getCycleFromLevelID( unsigned int level );
		static int getThemeFromLevelID( unsigned int level );
		static std::u32string getLevelHeader( unsigned int level );
		static std::u32string getThemeCodeFromLevelID( unsigned int level );
		static int getNextLevel( unsigned int level );
		static bool hasGemScore( unsigned int level );
		static bool hasTimeScore( unsigned int level );
		static bool hasCard( unsigned int level );
		static bool hasHardMode( unsigned int level );
};