#ifndef CONTROLS_H
#define CONTROLS_H

namespace BSL::Controls
{
    void update();
    int heldConfirm();
    int heldCancel();
    int heldUp();
    int heldRight();
    int heldDown();
    int heldLeft();
    int heldJump();
    int heldRun();
    void init();
    void close();
}

#endif // CONTROLS_H