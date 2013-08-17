/* SAMPLE PLUGIN FOR MAYA */

// Include general header files.
#include <math.h>           
#include <maya/MIOStream.h>
#include <maya/MFnPlugin.h>

// Include the headers for the shape.
#include <simpleShape.h>
#include <simpleShapeUI.h>
#include <simpleShapeIterator.h>


/*
 * Implementation of the simpleShape class
 */
MTypeId simpleShape::id ( 0x8009 );
simpleShape::simpleShape() {}
simpleShape::~simpleShape() {}

void* simpleShape::creator() 
// Creates an instance of the MPx node
{
    return new simpleShape();
}

MStatus simpleShape::initialize()
// Static initialization, wohooo
{
    return MS::kSuccess;
}

MPxGeometryIterator* simpleShape::geometryIteratorSetup(MObjectArray& componentList,
                                                        MObject& components,
                                                        bool forReadOnly )
// Creates a geometry iterator for the shape
{
    simpleShapeIterator * result = NULL;
    if (components.isNull() )
    {
        result = new simpleShapeIterator( getControlPoints(), componentList );
    }
    else
    {
        result = new simpleShapeIterator( getControlPoints(), components ); 
    }
    return result;
}

bool simpleShape::acceptsGeometryIterator( bool writeable )
// Lets Maya ask for an iterator that can set points if the bool is true
{
    return true;
}

bool simpleShape::acceptsGeometryIterator( MObject&, bool writeable, bool forReadOnly )
// Lets Maya ask for an iterator that can set points if the 1st bool is true,
// if the second is true, it asks for a readonly iterator.
{
    return true;
}


/*
 * Node registration
 */

// Gets called when the plugin is loaded into Maya.
MStatus initializePlugin( MObject obj ) {
    // Set plugin registration info: Author, plugin-version and Maya version needed.
    MFnPlugin plugin( obj, "Martin Jørgensen", "1.0", "Any" );
    MStatus stat = plugin.registerShape(
        "simpleShape",
        simpleShape::id,
        &simpleShape::creator,
        &simpleShape::initialize,
        &simpleShapeUI::creator
    );

    if ( ! stat ) {
        cerr << "Failed to register: simpleShape" << endl;
    }
    return stat;
}
// Gets called when the plugin is unloaded from Maya.
MStatus uninitializePlugin( MObject obj )
{
    MFnPlugin plugin( obj );
    MStatus stat;

    stat = plugin.deregisterNode( simpleShape::id );
    if ( ! stat ) {
        cerr << "Failed to deregister shape: simpleShape" << endl;
    }

    return stat;
}