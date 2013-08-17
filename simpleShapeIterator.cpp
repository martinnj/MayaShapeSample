// Implementation of the iterator class defined in include/simpleShapeIterator.h

// We need the vector array functionality for "points"
#include <maya/MVectorArray.h>

// And of course the definition for the iterator class.
#include <simpleShapeIterator.h>

simpleShapeIterator::simpleShapeIterator( void * geom, MObjectArray & comps ) : MPxGeometryIterator(geom, comps), geometry( (MVectorArray*)geom)
{
    reset();
}

simpleShapeIterator::simpleShapeIterator( void * geom, MObject & comp ) : MPxGeometryIterator(geom, comp), geometry( (MVectorArray*)geom)
{
    reset();
}


/*
 * Overrides
 */
void simpleShapeIterator::reset()
// Sets the iterator at the start of the components, so another pass may be made
{
    MPxGeometryIterator::reset();
    setCurrentPoint( 0 );
    if ( NULL != geometry )
    {
        int maxVertex = geometry->length();
        setMaxPoints( maxVertex );
    }
}

MPoint simpleShapeIterator::point() const
// return the index of the current element in the iteration.
{
    MPoint pnt;
    if ( NULL != geometry )
    {
        pnt = (*geometry)[index()];
    }
    return pnt;
}

void simpleShapeIterator::setPoint( const MPoint & pnt ) const
// Set the point for the current element in the iteration
{
    if ( NULL != geometry)
    {
        (*geometry)[index()] = pnt;
    }
}

int simpleShapeIterator::iteratorCount() const
// returns number of vertices in the iteration
{
    return geometry->length();
}

bool simpleShapeIterator::hasPoints() const
// returns true since the shape data has points
// Could be made to do an actual check, if length of geometry == 0, no points?
{
    return true;
}