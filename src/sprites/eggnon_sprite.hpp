


// Name
//===================================
//
// EggnonSprite
//

#ifndef EGGNON_SPRITE_H
#define EGGNON_SPRITE_H


// FORWARD DECLARATIONS
//===================================

    class Block;
    


// DEPENDENCIES
//===================================

    #include "sprite.hpp"


// CLASS
//===================================

    class EggnonSprite : public Sprite
    {
        public:
            EggnonSprite( int x, int y );
            ~EggnonSprite();
            void customUpdate( Camera& camera, Map& lvmap, EventSystem& events, SpriteSystem& sprites, BlockSystem& blocks, Health& health );
            void customInteract( Collision& my_collision, Collision& their_collision, Sprite& them, BlockSystem& blocks, SpriteSystem& sprites, Map& lvmap, Health& health, EventSystem& events );

        private:
            friend class State;
            friend class MovingState;
            friend class DroppingState;

            class State
            {
                public:
                    enum class Type
                    {
                        MOVING,
                        DROPPING
                    };

                    State( int time, Type type );
                    void run( EggnonSprite& e );
                    virtual void init( EggnonSprite& e, SpriteSystem& sprites ) = 0;
                    bool done() const;
                    Type type() const;

                
                    virtual void update( EggnonSprite& e ) = 0;
                    TimerSimple timer_;
                    Type type_;
            };

            class MovingState : public State
            {
                public:
                    MovingState();
                    void init( EggnonSprite& e, SpriteSystem& sprites );

                
                    void update( EggnonSprite& e );
            };

            class DroppingState : public State
            {
                public:
                    DroppingState();
                    void init( EggnonSprite& e, SpriteSystem& sprites );

                
                    void update( EggnonSprite& e );
            };

            std::unique_ptr<State> state_;
    };


#endif // EGGNON_SPRITE_H
