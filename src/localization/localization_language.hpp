#pragma once

#include "char_frame.hpp"
#include <string>
#include <vector>
#include <unordered_map>

class LocalizationLanguage
{
    public:
        LocalizationLanguage( const std::string& path );
        const std::u32string& getLanguageName() const;
        const std::u32string& getTitleCreatedBy() const;
        const std::u32string& getScreenOptionFullscreen() const;
        const std::u32string& getScreenOptionWindow() const;
        const std::u32string& getInputQuitting() const;
        const std::u32string& getOptionsTitle() const;
        const std::u32string& getScreenOptionsTitle() const;
        const std::u32string& getLanguageOptionsTitle() const;
        const std::vector<CharFrame> getCharacterFrames( char32_t character ) const;
        const std::vector<std::u32string>& getTitleOptions() const;
        const std::vector<std::u32string>& getOptionsOptions() const;

    private:
        void loadCharset( const auto& data, const std::string& path );
        void loadLanguageName( const auto& data, const std::string& path );
        void loadInputText( const auto& data, const std::string& path );
        void loadScreenOptions( const auto& data, const std::string& path );
        void loadTitleText( const auto& data, const std::string& path );
        void loadOptionsText( const auto& data, const std::string& path );

        std::u32string language_;
        std::u32string title_created_by_;
        std::u32string input_quitting_;
        std::u32string screen_option_fullscreen_;
        std::u32string screen_option_window_;
        std::u32string options_title_;
        std::u32string screen_options_title_;
        std::u32string language_options_title_;
        std::unordered_map<char32_t, std::vector<CharFrame>> charset_;
        std::vector<std::u32string> title_options_;
        std::vector<std::u32string> options_options_;
};