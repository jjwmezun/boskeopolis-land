#ifndef CONTROLLER_H
#define CONTROLLER_H

namespace BSL
{
    class Controller
    {
        public:
            virtual ~Controller();
            virtual bool heldRight() const = 0;
            virtual bool heldLeft() const = 0;
            virtual bool heldJump() const = 0;
            virtual bool heldRun() const = 0;
            virtual bool heldMenu() const = 0;
    };
}

#endif // CONTROLLER_H