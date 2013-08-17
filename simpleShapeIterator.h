/*
 * Component iterator, control based geometry
 * Used by translate/rotate/scale manipulators to determine where
 * the manipulator should be placed when components are selected.
 * 
 * Deformers use it to deform points around the shape.
 */

#include <maya/MPxGeometryIterator.h>
#include <maya/MPoint.h>

class MVectorArray;

class simpleShapeIterator : public MPxGeometryIterator
{
public:
    simpleShapeIterator( void * userGeometry, MObjectArray & components );
    simpleShapeIterator( void * userGeometry, MObject      & component  );

    /*
     * Overrides for parent (You ain't the boss of me!)
     */
    virtual void   reset();
    virtual MPoint point() const;
    virtual void   setPoint( const MPoint & ) const;
    virtual int    iteratorCount() const;
    virtual bool   hasPoints() const;

public:
    MVectorArray * geometry;
};