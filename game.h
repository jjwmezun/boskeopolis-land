

// Name
//===================================
//
// Game
//


#ifndef GAME_H
#define GAME_H


// FORWARD DECLARATIONS
//===================================


// DEPENDENCIES
//===================================

    #include "game_state.h"
    #include "graphics.h"
    #include "input.h"
    #include <memory>
    #include <vector>


// CLASS
//===================================

    class Game
    {
        public:
            const char* TITLE = "Boskeopolis Land";

            Game( std::vector<std::string>& args );
            ~Game();
            Game( const Game& ) = delete;
            Game& operator=( const Game& ) = delete;
            bool running() const;
            void execute();
            void quit();

            void changeState( std::unique_ptr<GameState> state );
            void pushState( std::unique_ptr<GameState> state );
            void popState();

            static bool nextFrame( int interval = 8, int duration = 1 );
            static int frame();
            static std::string resourcePath();
            static std::string pathDivider();
            static std::string savePath();
			
            static bool savingAllowed();


        private:
            Input input_;
            bool running_ = true;
            bool state_change_ = false;
			bool state_is_new_ = false;
			bool state_pop_    = false;
			
            int ticks_ = 0;
            int graphics_ticks_ = 0;
            static int frames_;
            static std::string resource_path_;
            static std::string path_divider_;
            static bool SAVING_ALLOWED;

			std::unique_ptr<GameState> new_state_;
            std::vector< std::unique_ptr<GameState> > states_ = {}; // Polymorphism pointers.
            std::unique_ptr<Graphics> graphics_; // Must be allocated @ runtime, thus must be a pointer.

            int fpsMilliseconds() const;
            bool initSDL();
            void setResourcePath();
            void render();
            void firstState();

            void changeStateSafe();
            void popStateSafe();
    };

#endif // GAME_H
