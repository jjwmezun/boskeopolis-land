#pragma once

enum class OWState
{
    MOVE_PLAYER,
    MOVE_CAMERA,
    CAMERA_MOVES_AUTOMATICALLY_TO_PLAYER,
    EVENT,
    CAMERA_MOVES_TO_EVENT,
    CAMERA_MOVES_TO_SECRET_EVENT
};