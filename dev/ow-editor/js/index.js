import React, { useEffect, useRef, useState } from 'react';
import { render } from 'react-dom';
import styled from 'styled-components';
import { DndContext, useDraggable, useDroppable } from '@dnd-kit/core';
import { saveAs } from 'file-saver';

const highlightColor = `rgba( 128, 128, 255, 0.5 )`;

const tiles = new Image();
tiles.src = './ow.png';

const CanvasStyle = styled.div`
    min-height: 100vh;
`;

const GridStyle = styled.div`
    position: relative;
`;

const GridTableStyle = styled.table`
    border-spacing: 0;
    border-collapse: collapse;
    position: relative;
`;

const GridColumnStyle = styled.td`
    border: 1px solid #000;
    width: 16px;
    height: 16px;
    min-width: 16px;
    cursor: ${ props => props.isDragging ? `inherit` : `pointer` };
    padding: 0;
    margin: 0;
    position: relative;
    box-sizing: border-box;
    overflow: visible;

    &::after {
        content: '';
        position: absolute;
        left: 0;
        right: 0;
        top: 0;
        bottom: 0;
        pointer-events: none;
    }

    &:hover::after {
        background-color: ${ highlightColor };
    }
`;

const ObjectStyle = styled.div`
    position: absolute;
    z-index: 3;
    border: none;
`;

const ObjectBorderCornerStyle = `
    width: 2px;
    height: 2px;
    position: absolute;
    z-index: 4;
`;

const ObjectBorderXSideStyle = `
    width: 2px;
    height: calc( 100% - 4px );
    position: absolute;
    z-index: 4;
    cursor: col-resize;
`;

const ObjectBorderYSideStyle = `
    width: calc( 100% - 4px );
    height: 2px;
    position: absolute;
    z-index: 4;
    cursor: row-resize;
`;

const ObjectBorderRightStyle = styled.span`
    ${ ObjectBorderXSideStyle }
    top: 2px;
    right: 0;
`;

const ObjectBorderBottomStyle = styled.span`
    ${ ObjectBorderYSideStyle }
    bottom: 0;
    left: 2px;
`;

const ObjectBorderLeftStyle = styled.span`
    ${ ObjectBorderXSideStyle }
    top: 2px;
    left: 0;
`;

const ObjectBorderTopStyle = styled.span`
    ${ ObjectBorderYSideStyle }
    top: 0;
    left: 2px;
`;

const ObjectBorderTopLeftStyle = styled.span`
    ${ ObjectBorderCornerStyle }
    top: 0;
    left: 0;
    cursor: nw-resize;
`;

const ObjectBorderTopRightStyle = styled.span`
    ${ ObjectBorderCornerStyle }
    top: 0;
    right: 0;
    cursor: ne-resize;
`;

const ObjectBorderBottomRightStyle = styled.span`
    ${ ObjectBorderCornerStyle }
    bottom: 0;
    right: 0;
    cursor: se-resize;
`;

const ObjectBorderBottomLeftStyle = styled.span`
    ${ ObjectBorderCornerStyle }
    bottom: 0;
    left: 0;
    cursor: sw-resize;
`;

const createRange = end => [ ...Array( end ).keys() ];

const ObjectBorderTop = props => {
    return <ObjectBorderTopStyle onMouseDown={ props.startResize( props.id, `up` ) } />;
};

const ObjectBorderRight = props => {
    return <ObjectBorderRightStyle onMouseDown={ props.startResize( props.id, `right` ) } />;
};

const ObjectBorderBottom = props => {
    return <ObjectBorderBottomStyle onMouseDown={ props.startResize( props.id, `down` ) } />;
};

const ObjectBorderLeft = props => {
    return <ObjectBorderLeftStyle onMouseDown={ props.startResize( props.id, `left` ) } />;
};

const ObjectBorderTopLeft = props => {
    return <ObjectBorderTopLeftStyle onMouseDown={ props.startResize( props.id, `upleft` ) } />;
};

const ObjectBorderTopRight = props => {
    return <ObjectBorderTopRightStyle onMouseDown={ props.startResize( props.id, `upright` ) } />;
};

const ObjectBorderBottomRight = props => {
    return <ObjectBorderBottomRightStyle onMouseDown={ props.startResize( props.id, `downright` ) } />;
};

const ObjectBorderBottomLeft = props => {
    return <ObjectBorderBottomLeftStyle onMouseDown={ props.startResize( props.id, `downleft` ) } />;
};

const ObjectDraggable = props => {
    const {attributes, listeners, setNodeRef, transform} = useDraggable({
        id: `draggable${ props.object.id }`,
    });

    const width = props.object.w * 16 + ( props.object.w - 1 );
    const height = props.object.h * 16 + ( props.object.h - 1 );
    const containerStyle = {
        position: `absolute`,
        width: `${ width }px`,
        height: `${ height }px`,
    };

    const style = {
        width: `${ width }px`,
        height: `${ height }px`,
    };

    if ( transform ) {
        style.transform = `translate3d(${transform.x}px, ${transform.y}px, 0)`;
    }

    const canvasRef = useRef();

    useEffect( () => {
        if ( canvasRef.current ) {
            const context = canvasRef.current.getContext( `2d` );
            objectTypes[ props.object.type ]?.draw( context, props.object );
        }
    }, [ canvasRef.current, props.object.x, props.object.y, props.object.w, props.object.h, props.object.parent ] );

    return <div style={ containerStyle }>
        <ObjectStyle ref={setNodeRef} style={style} {...listeners} {...attributes}>
            <canvas ref={ canvasRef } width={ width } height={ height } />
        </ObjectStyle>
        <ObjectBorderTop id={ props.object.id } startResize={ props.startResize } />
        <ObjectBorderRight id={ props.object.id } startResize={ props.startResize } />
        <ObjectBorderBottom id={ props.object.id } startResize={ props.startResize } />
        <ObjectBorderLeft id={ props.object.id } startResize={ props.startResize } />
        <ObjectBorderTopLeft id={ props.object.id } startResize={ props.startResize } />
        <ObjectBorderTopRight id={ props.object.id } startResize={ props.startResize } />
        <ObjectBorderBottomRight id={ props.object.id } startResize={ props.startResize } />
        <ObjectBorderBottomLeft id={ props.object.id } startResize={ props.startResize } />
    </div>;
};

export function Droppable(props) {
  const {isOver, setNodeRef} = useDroppable({
    id: props.id,
  });
  const style = {
    width: `16px`,
    height: `16px`,
  };
  
  return (
    <div ref={setNodeRef} style={style}>
      {props.children}
    </div>
  );
};

const GridColumn = props => {
    const ref = useRef();

    const onPaste = e => {
        if ( ref.current ) {
            props.addObject( props.id, props.x, props.y );
        }
        e.preventDefault();
    };

    return <GridColumnStyle
        ref={ ref }
        id={ props.id }
        onContextMenu={ onPaste }
        isDragging={ props.isDragging }
    >
        <Droppable id={ props.id }>
            { props.objectsMap[ props.id ].map( ( objectId, i ) => <ObjectDraggable key={ objectId } object={ props.objects[ objectId ] } startResize={ props.startResize } /> ) }
        </Droppable>
    </GridColumnStyle>;
}

const hList = createRange( 200 );
const wList = createRange( 200 );

const GridRow = props => <tr>
    { wList.map( i => <GridColumn
        key={ props.width * props.y + i }
        id={ props.width * props.y + i }
        x={ i }
        y={ props.y }
        addObject={ props.addObject }
        objects={ props.objects }
        objectsMap={ props.objectsMap }
        startResize={ props.startResize }
        isDragging={ props.isDragging }
    >
    </GridColumn> )}
</tr>;

const GridTable = props => <GridTableStyle>
    { hList.map( i => <GridRow
        y={ i }
        width={ props.width }
        key={ i }
        addObject={ props.addObject }
        objects={ props.objects }
        objectsMap={ props.objectsMap }
        startResize={ props.startResize }
        isDragging={ props.isDragging }
    /> ) }
</GridTableStyle>;

const Grid = props => {
    const handleDragEnd = ({ active, over }) => {
        if ( typeof active?.id !== `string` || typeof over?.id !== `number` ) {
            return;
        }
        const objectId = parseInt( active.id.replace( `draggable`, `` ) );
        const newX = over.id % props.width;
        const newY = Math.floor( over.id / props.width );
        if (
                newX + props.objects[ objectId ].w > props.width
                || newY + props.objects[ objectId ].h > props.height
        ) {
            return;
        }
        props.setObjects( prevObjects => {
            const newObjects = prevObjects.map( v => v );
            newObjects[ objectId ].parent = over.id;
            newObjects[ objectId ].x = newX;
            newObjects[ objectId ].y = newY;
            saveData( newObjects );
            return newObjects;
        });
    };

    return <GridStyle>
        <DndContext onDragEnd={handleDragEnd}>
            <GridTable
                width={ props.width }
                height={ props.height }
                addObject={ props.addObject }
                objects={ props.objects }
                objectsMap={ props.objectsMap }
                startResize={ props.startResize }
                isDragging={ props.isDragging }
            />
            { props.objects.map( ( object, i ) => object.parent === null ? <ObjectDraggable key={ i } object={ object } /> : null ) }
        </DndContext>
    </GridStyle>;
}

const generateGraphicTiler = ( sx, sy ) => ( context, object ) => {
    for ( let y = 0; y < object.h; ++y ) {
        for ( let x = 0; x < object.w; ++x ) {
            const xDist = x * 16 + x;
            const yDist = y * 16 + y;
            context.drawImage( tiles, sx, sy, 16, 16, xDist, yDist, 16, 16 );
        }
    }
}

const objectTypes = Object.freeze([
    {
        name: `Grass`,
        draw: generateGraphicTiler( 0, 0 )
    },
    {
        name: `Grass 2`,
        draw: generateGraphicTiler( 16, 0 )
    },
    {
        name: `Brick`,
        draw: generateGraphicTiler( 32, 0 )
    }
]);

const OptionsPanel = props => {
    const onChangeObjectType = e => {
        props.setSelectedType( Number( e.target.value ) );
    };

    const save = () => {
        const list = [];
        const rawlist = Uint16Array.from(list);
        const blob = new Blob([rawlist.buffer], {type: "application/octet-binary"});
        saveAs( blob, 'map.bslmap' );
    };

    const clear = () => {
        if ( confirm( `¿Are you sure you want to clear map?` ) ) {
            props.setObjects([]);
            saveData([]);
        }
    };

    const updateGridWidth = e => {
        props.setGridWidth( Number( e.target.value ) );
    };

    return <div>
        <div>
            <label>Width: <input type="number" value={ props.gridWidth } onChange={ updateGridWidth } /></label>
        </div>
        <select onChange={ onChangeObjectType }>
            { objectTypes.map( ( type, i ) => <option value={ i }>{ type.name }</option> ) }
        </select>
        <button onClick={ save }>Save</button>
        <button onClick={ clear }>Clear</button>
    </div>;
};

const savedData = JSON.parse( localStorage.getItem( `data` ) );

const saveData = objects => localStorage.setItem( `data`, JSON.stringify( objects ) );

const Canvas = () => {
    const [ objects, setObjects ] = useState( [] );
    const [ selectedType, setSelectedType ] = useState( 0 );
    const [ gridWidth, setGridWidth ] = useState( 200 );
    const [ gridHeight, setGridHeight ] = useState( 200 );
    const [ objectsMap, setObjectsMap ] = useState( [ ...Array( gridWidth * gridHeight ).keys() ].map( () => [] ) );

    const addObject = ( parent, x, y ) => {
        const id = objects.length;
        setObjects( prevObjects => {
            const newObjects = prevObjects.map( v => v );
            newObjects.push({
                type: selectedType,
                parent,
                id,
                x,
                y,
                w: 1,
                h: 1
            });
            saveData( newObjects );
            return newObjects;
        });
        setObjectsMap( prevObjectsMap => {
            const newObjectsMap = prevObjectsMap.map( v => v );
            newObjectsMap[ parent ].push( id );
            return newObjectsMap;
        });
    };

    const [ drag, setDrag ] = useState({
        id: null,
        direction: null,
        x: ``,
        y: ``,
    });

    const [ dimensions, setDimensions ] = useState({
        w: 16,
        h: 16
    });

    const stopResize = e => {
        setDrag({ ...drag, id: null });
    };

    const startResize = ( id, direction ) => e => {
        setDrag({
            id,
            direction,
            x: e.clientX,
            y: e.clientY
        });
        setDimensions({ w: objects[ id ].w * 16, h: objects[ id ].h * 16 });
    };

    const resizeFrame = e => {
        const { id, direction, x, y } = drag;
        if ( id !== null ) {

            const moveLeft = () => {
                const xDiff = Math.abs( x - e.clientX );
                const w = x > e.clientX ? dimensions.w + xDiff : dimensions.w - xDiff;
                const objW = Math.min( Math.max( Math.round( w / 16 ), 1 ), gridWidth - objects[ id ].x );
                const wDiff = objW - objects[ id ].w;
                if ( objects[ id ].x - wDiff >= 0 && objW > 0 ) {
                    setObjects( prevObjects => {
                        const newObjects = prevObjects.map( v => v );
                        newObjects[ id ].x -= wDiff;
                        newObjects[ id ].parent -= wDiff;
                        newObjects[ id ].w = objW;
                        saveData( newObjects );
                        return newObjects;
                    });
                }
                setDrag( prevDrag => ({ ...prevDrag, x: e.clientX }) );
                setDimensions( prevDimensions => ({ ...prevDimensions, w }) );
            };

            const moveDown = () => {
                const yDiff = Math.abs( y - e.clientY );
                const h = y > e.clientY ? dimensions.h - yDiff : dimensions.h + yDiff;
                const objH = Math.min( Math.max( Math.round( h / 16 ), 1 ), gridHeight - objects[ id ].y );
                setObjects( prevObjects => {
                    const newObjects = prevObjects.map( v => v );
                    newObjects[ id ].h = objH;
                    saveData( newObjects );
                    return newObjects;
                });
                setDrag( prevDrag => ({ ...prevDrag, y: e.clientY }) );
                setDimensions( prevDimensions => ({ ...prevDimensions, h }) );
            };

            const moveUp = () => {
                const yDiff = Math.abs( y - e.clientY );
                const h = y > e.clientY ? dimensions.h + yDiff : dimensions.h - yDiff;
                const objH = Math.min( Math.max( Math.round( h / 16 ), 1 ), gridWidth - objects[ id ].y );
                const hDiff = objH - objects[ id ].h;
                if ( objects[ id ].y - hDiff >= 0 && objH > 0 ) {
                    setObjects( prevObjects => {
                        const newObjects = prevObjects.map( v => v );                        
                        newObjects[ id ].y -= hDiff;
                        newObjects[ id ].parent -= ( hDiff * gridWidth );
                        newObjects[ id ].h = objH;
                        saveData( newObjects );
                        return newObjects;
                    });
                }
                setDrag( prevDrag => ({ ...prevDrag, y: e.clientY }) );
                setDimensions( prevDimensions => ({ ...prevDimensions, h }) );
            };

            const moveRight = () => {
                const xDiff = Math.abs( x - e.clientX );
                const w = x > e.clientX ? dimensions.w - xDiff : dimensions.w + xDiff;
                const objW = Math.min( Math.max( Math.round( w / 16 ), 1 ), gridWidth - objects[ id ].x );
                setObjects( prevObjects => {
                    const newObjects = prevObjects.map( v => v );
                    newObjects[ id ].w = objW;
                    saveData( newObjects );
                    return newObjects;
                });
                setDrag( prevDrag => ({ ...prevDrag, x: e.clientX }) );
                setDimensions( prevDimensions => ({ ...prevDimensions, w }) );
            };

            switch ( direction ) {
                case ( `up` ): {
                    moveUp();
                }
                break;
                case ( `right` ): {
                    moveRight();
                }
                break;
                case ( `down` ): {
                    moveDown();
                }
                break;
                case ( `left` ): {
                    moveLeft();
                }
                break;
                case ( `upleft` ): {
                    moveLeft();
                    moveUp();
                }
                break;
                case ( `upright` ): {
                    moveRight();
                    moveUp();
                }
                break;
                case ( `downright` ): {
                    moveRight();
                    moveDown();
                }
                break;
                case ( `downleft` ): {
                    moveLeft();
                    moveDown();
                }
                break;
            }
        }
    };

    const style = {};
    switch ( drag.direction ) {
        case ( `downright` ): {
            style.cursor = `se-resize`;
        }
        break;
        case ( `upright` ): {
            style.cursor = `ne-resize`;
        }
        break;
        case ( `downleft` ): {
            style.cursor = `sw-resize`;
        }
        break;
        case ( `upleft` ): {
            style.cursor = `nw-resize`;
        }
        break;
        case ( `right` ):
        case ( `left` ): {
            style.cursor = `col-resize`;
        }
        case ( `up` ):
        case ( `down` ): {
            style.cursor = `row-resize`;
        }
    }

    return <CanvasStyle style={ style } onMouseMove={ resizeFrame } onMouseUp={ stopResize }>
        <h1>Boskeopolis Land OW Editor</h1>
        <div style={{ display: `grid`, gridTemplateColumns: `20rem 1fr` }}>
            <OptionsPanel
                gridWidth={ gridWidth }
                setGridWidth={ setGridWidth }
                objects={ objects }
                setObjects={ setObjects }
                setSelectedType={ setSelectedType }
            />
            <div style={{ overflow: `scroll`, maxHeight: `80vh` }}>
                <Grid
                    width={ gridWidth }
                    height={ gridHeight }
                    startResize={ startResize }
                    objects={ objects }
                    objectsMap={ objectsMap }
                    addObject={ addObject }
                    setObjects={ setObjects }
                    isDragging={ drag.id !== null }
                />
            </div>
        </div>
    </CanvasStyle>;
};

document.addEventListener(
    `DOMContentLoaded`,
    () => {
        const root = document.getElementById( `canvas` );
        render( <Canvas />, root );
    }
);

/*
const createRange = end => [ ...Array( end ).keys() ];

const w = 200;
const h = 200;
const wList = createRange( w );
const hList = createRange( h );

const Canvas = () => {
    return <table>
        { hList.map( i => <tr key={ i }>
            { wList.map( i => <td key={ i } className="tcell">

            </td>)}
        </tr>)}
    </table>;
};

document.addEventListener(
    `DOMContentLoaded`,
    () => {
        const root = document.getElementById( `canvas` );
        render( <Canvas />, root );
    }
);*/

/*
const w = 200;
const h = 200;

const createElement = ( element, atts = {}, content = null ) => {
    const e = document.createElement( element );
    for ( const att in atts ) {
        e.setAttribute( att, atts[ att ] );
    }
    return e;
};

document.addEventListener(
    `DOMContentLoaded`,
    () => {
        const root = document.getElementById( `canvas` );
        const table = createElement( `table`, { 'class': `grid` } );
        for ( let y = 0; y < h; ++y ) {
            const row = createElement( `tr` );
            for ( let x = 0; x < w; ++x ) {
                const cell = createElement( `td`, { 'class': `tcell` } );
                row.appendChild( cell );
            }
            table.appendChild( row );
        }
        root.appendChild( table );
    }
);*/