#include "map_layer.hpp"

MapLayer::MapLayer( int layer_position ) : layer_position_ ( layer_position ) {};

MapLayer::~MapLayer() {};

void MapLayer::update( LevelState& level_state )
{};

void MapLayer::interact( Sprite& sprite, LevelState& level_state )
{};
