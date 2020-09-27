#include "level_state.hpp"
#include "map.hpp"
#include "map_layer_renderable.hpp"

MapLayerRenderable::MapLayerRenderable( int id ) : Renderable( Type::SPRITE ), id_ ( id ) {};
MapLayerRenderable::~MapLayerRenderable() {};

void MapLayerRenderable::render( const LevelState& level_state ) const
{
    level_state.currentMap().renderLayer( id_, level_state );
};