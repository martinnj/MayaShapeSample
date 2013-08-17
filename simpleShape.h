// Include-guards
#ifndef _simpleShape
#define _simpleShape

#include <maya/MTypeId.h> 
#include <maya/MPxComponentShape.h>

class MPxGeometryIterator;

class simpleShape : public MPxComponentShape
{
public:
    simpleShape();
    virtual ~simpleShape();

    // Associate a userdefined iterator with the shape,
    // this should allow us to have components in the shape.
    virtual MPxGeometryIterator* geometryIteratorSetup( MObjectArray&, MObject&, bool forReadOnly=false );
    virtual bool                 acceptsGeometryIterator( bool writeable=true );
    virtual bool                 acceptsGeometryIterator( MObject&, bool writeable=true, bool forReadOnly=false );

    static void *  creator();
    static MStatus initialize();

    static MTypeId id;
};

#endif /* _simpleShape */