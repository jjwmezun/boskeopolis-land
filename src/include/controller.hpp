#ifndef CONTROLLER_H
#define CONTROLLER_H

namespace BSL
{
    class Controller
    {
        public:
            virtual ~Controller();
            virtual bool heldRight() = 0;
            virtual bool heldLeft() = 0;
            virtual bool heldJump() = 0;
            virtual bool heldRun() = 0;
    };
}

#endif // CONTROLLER_H