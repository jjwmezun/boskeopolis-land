const { assert } = require("console");
const fs = require( `fs` );
const eventsDir = `dev/events`;
const tilesetLength = 2048;

const isObject = ( o ) => typeof o === `object`;
const hasObject = ( parent, childName ) => childName in parent && isObject( [ childName ] );
const fswitch = ( value, options, fallback ) => {
    for ( const key of Object.keys( options ) ) {
        if ( value == key ) {
            return options[ key ];
        }
    };
    return fallback;
};

fs.readdir( eventsDir, ( err, files ) => {
    if ( err ) {
        throw err;
    }

    files.forEach( ( file ) => {
        const fullFilename = `${ eventsDir }/${ file }`;
        fs.readFile( fullFilename, ( err, buffer  ) => {
            if ( err ) {
                throw err;
            }

            const inData = JSON.parse( buffer.toString() );
            const outData = { changes: [] };

            if ( !( `width` in inData ) ) {
                return;
            }

            const xFromI = ( i ) => i % inData.width;
            const yFromI = ( i ) => Math.floor( i / inData.width );

            if ( hasObject( inData, `properties` ) ) {
                inData.properties.forEach( ( property ) => {
                    if ( isObject( property ) && `name` in property && `value` in property ) {
                        switch ( property.name ) {
                            case ( `camera_x` ): {
                                outData.camera_target_x = property.value;
                            }
                            break;
                            case ( `camera_y` ): {
                                outData.camera_target_y = property.value;
                            }
                            break;
                            case ( `speed` ): {
                                outData.speed = property.value;
                            }
                            break;
                        }
                    }
                });
            }

            const validChanges = [];

            if ( hasObject( inData, `layers` ) ) {
                inData.layers.forEach( ( change ) => {
                    if ( isObject( change ) && hasObject( change, `data` ) ) {
                        validChanges.push( change );
                    }
                });
            }

            outData.changes = validChanges.map(( change, changeIndex ) => {
                const list = [];
                change.data.forEach( ( tileValue, tileIndex ) => {
                    --tileValue;
                    const layer = fswitch( Math.floor( tileValue / tilesetLength ), [ `BG1`, `BG2`, `FG1`, `FG2` ], `BG1` );
                    const tile = tileValue % tilesetLength;
                    if ( tile === tilesetLength - 1 ) {
                        list.push({
                            layer: layer,
                            x: xFromI( tileIndex ),
                            y: yFromI( tileIndex ),
                            tile: -1
                        })
                    }
                    else if ( tile >= 0 ) {
                        list.push({
                            layer: layer,
                            x: xFromI( tileIndex ),
                            y: yFromI( tileIndex ),
                            tile: tile
                        })
                    }
                });
                return list;
            });

            fs.writeFile( `bin/assets/events/${ file }`, JSON.stringify( outData, null, `\t` ), ( err ) => {
                if ( err ) {
                    throw err;
                }
            } );
        })
    });
} );