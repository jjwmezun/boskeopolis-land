#include "map_layer.hpp"

MapLayer::MapLayer( Unit::Layer layer_position ) : layer_position_ ( layer_position ) {};

MapLayer::~MapLayer() {};

void MapLayer::update( LevelState& level_state )
{};

void MapLayer::interact( Sprite& sprite, LevelState& level_state )
{};
