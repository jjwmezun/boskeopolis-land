#pragma once

#include "wtext_character.hpp"
#include <filesystem>
#include "input.hpp"
#include <string>
#include <vector>
#include <unordered_map>

class LocalizationLanguage
{
    public:
        LocalizationLanguage( const std::filesystem::directory_entry& file );
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

    private:
        void loadCharset( const auto& data, const std::string& path );
        void loadOrder( const auto& data, const std::string& path );
        void loadIntroMessage( const auto& data, const std::string& path );
        void loadLanguageName( const auto& data, const std::string& path );
        void loadInputText( const auto& data, const std::string& path );
        void loadScreenOptions( const auto& data, const std::string& path );
        void loadTitleText( const auto& data, const std::string& path );
        void loadOptionsText( const auto& data, const std::string& path );
        void loadLevelSelectText( const auto& data, const std::string& path );
        void loadLevelText( const auto& data, const std::string& path );

        std::unordered_map<std::string, std::u32string> level_names_;
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
        std::u32string controls_actions_names_[ Input::NUM_O_ACTIONS ];
        std::unordered_map<char32_t, std::vector<WTextCharacter>> charset_;
        std::vector<std::u32string> title_options_;
        std::vector<std::u32string> options_options_;
        int order_;
        std::string charset_image_src_;
        int charset_height_;
        std::vector<WTextCharacter> default_character_;
        std::string path_name_;
};