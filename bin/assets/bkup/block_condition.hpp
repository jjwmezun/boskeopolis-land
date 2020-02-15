#pragma once

class Block;
class Collision;
class EventSystem;
class Sprite;

// "BlockCondition" type is a function pointer to function that returns bool & takes in these arguments.
typedef bool ( *BlockCondition )( const Collision& collision, const Sprite& sprite, const Block& block, const EventSystem& events );