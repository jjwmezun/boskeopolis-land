#pragma once

#include "wtext_character.hpp"
#include <filesystem>
#include "input.hpp"
#include "rapidjson/document.h"
#include <string>
#include <vector>
#include <unordered_map>

class LocalizationLanguage
{
    public:
        LocalizationLanguage( const std::filesystem::directory_entry& file );

        static constexpr int NUMBER_OF_OVERWORLD_MENU_OPTIONS = 5;

        const std::u32string& getLanguageName() const;
        const std::u32string& getIntroText() const;
        const std::u32string& getTitleCreatedBy() const;
        const std::u32string& getScreenOptionFullscreen() const;
        const std::u32string& getScreenOptionWindow() const;
        const std::u32string& getInputQuitting() const;
        const std::u32string& getOptionsTitle() const;
        const std::u32string& getScreenOptionsTitle() const;
        const std::u32string& getLanguageOptionsTitle() const;
        const std::u32string& getLevelSelectTitle() const;
        const std::u32string& getLevelSelectCycleName() const;
        const std::u32string& getLevelSelectPercentSymbol() const;
        const std::vector<WTextCharacter> getCharacterFrames( char32_t character ) const;
        const std::vector<std::u32string>& getTitleOptions() const;
        const std::vector<std::u32string>& getOptionsOptions() const;
        int getOrder() const;
        const std::u32string& getControlsOptionsTitle() const;
        const std::u32string* getControlsActionNames() const;
        const std::string& getCharsetImageSrc() const;
        int getCharsetHeight() const;
        const std::string& getPathName() const;
        std::u32string getLevelName( const std::string& code_name ) const;
        std::u32string getLevelMessage( const std::string& code_name ) const;
        std::u32string getLevelGoalMessage( const std::string& code_name ) const;
        const std::u32string* getOverworldMenuNames() const;
        const std::u32string& getPressAnyKey() const;
        const std::u32string& getRandomNewsTickerMessage() const;
        int getMaxNewsTickerMessageWidth() const;
        const std::u32string& getPauseContinue() const;
        const std::u32string& getPauseOptions() const;
        const std::u32string& getPauseQuitUnbeaten() const;
        const std::u32string& getPauseQuitBeaten() const;
        const std::u32string& getLevelTilePlay() const;
        const std::u32string& getLevelTileHardMode() const;
        const std::u32string& getLevelTileCancel() const;

        std::u32string getShopItemHPUpgradeName() const;
        std::u32string getShopItemHPUpgradeDescription() const;
        std::u32string getShopItemOxygenUpgradeName() const;
        std::u32string getShopItemOxygenUpgradeDescription() const;
        std::u32string getShopCheckout() const;
        std::u32string getShopExit() const;
        std::u32string getNoItemsInCartMessage() const;
        std::u32string getNotEnoughFundsForCheckoutMessage() const;
        std::u32string getShopGreeting() const;
        std::u32string getCheckoutConfirmPrompt() const;
        std::u32string getCheckoutThanks() const;
        std::u32string getShopNotAvailableText() const;
        std::u32string getShopOutOfStockText() const;
        std::u32string getShopLeavingMessage() const;
        std::u32string getShopkeeperNotAvailableMessage() const;
        std::u32string getShopkeeperOutOfStockMessage() const;
        std::u32string getOverworldShopTitle() const;

        std::u32string getCollectGoalMessage() const;
        std::u32string getMcGuffinGoalMessage() const;
        std::u32string getSurviveTimeGoalMessage() const;
        std::u32string getPastRightEdgeGoalMessage() const;
        std::u32string getWarpGoalMessage() const;
        std::u32string getAvoidMoneyGoalMessage() const;
        std::u32string getStarvingGoalMessage() const;
        std::u32string getHeatGoalMessage() const;
        std::u32string getDoNothingGoalMessage() const;
        std::u32string getKillAllGoalMessage() const;
        std::u32string getStopOnOffGoalMessage() const;
        std::u32string getGenericGoalMessage() const;

        std::u32string getFailureMessage() const;
        std::u32string getSuccessMessage() const;

    private:
        void loadCharset( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data, const std::string& path );
        void loadOrder( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data, const std::string& path );
        void loadIntroMessage( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data, const std::string& path );
        void loadLanguageName( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data, const std::string& path );
        void loadInputText( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data, const std::string& path );
        void loadScreenOptions( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data, const std::string& path );
        void loadTitleText( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data, const std::string& path );
        void loadOptionsText( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data, const std::string& path );
        void loadLevelSelectText( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data, const std::string& path );
        void loadLevelText( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data, const std::string& path );
        void loadOverworldText( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data, const std::string& path );
        void loadNewsTickerText( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data, const std::string& path );
        void loadPauseText( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data, const std::string& path );
        void loadLevelTileMenuText( const rapidjson::GenericObject<false, rapidjson::GenericValue<rapidjson::UTF8<> > >& data, const std::string& path );

        int order_;
        int charset_height_;
        int max_news_ticker_message_width_;
        std::vector<WTextCharacter> default_character_;
        std::vector<std::u32string> title_options_;
        std::vector<std::u32string> options_options_;
        std::u32string language_;
        std::u32string intro_text_;
        std::u32string title_created_by_;
        std::u32string input_quitting_;
        std::u32string screen_option_fullscreen_;
        std::u32string screen_option_window_;
        std::u32string options_title_;
        std::u32string screen_options_title_;
        std::u32string language_options_title_;
        std::u32string controls_options_title_;
        std::u32string level_select_title_;
        std::u32string level_select_cycle_name_;
        std::u32string level_select_percent_symbol_;
        std::u32string press_any_key_;
        std::u32string pause_continue_;
        std::u32string pause_options_;
        std::u32string pause_quit_unbeaten_;
        std::u32string pause_quit_beaten_;
        std::u32string level_tile_play_;
        std::u32string level_tile_hard_mode_;
        std::u32string level_tile_cancel_;
        std::string path_name_;
        std::string charset_image_src_;
        std::unordered_map<char32_t, std::vector<WTextCharacter>> charset_;
        std::unordered_map<std::string, std::u32string> level_names_;
        std::unordered_map<std::string, std::u32string> level_messages_;
        std::unordered_map<std::string, std::u32string> level_goal_messages_;
        std::u32string controls_actions_names_[ Input::NUM_O_ACTIONS ];
        std::u32string overworld_menu_names_[ NUMBER_OF_OVERWORLD_MENU_OPTIONS ];
        std::vector<std::u32string> news_ticker_messages_;
};
