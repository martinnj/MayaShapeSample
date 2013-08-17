/* SAMPLE PLUGIN FOR MAYA */

// Include general header files.
#include <math.h>           
#include <maya/MIOStream.h>
#include <maya/MFnPlugin.h>

// Include the headers for the shape.
#include "simpleShape.h"
#include "simpleShapeUI.h"
#include "simpleShapeIterator.h"


// Gets called when the plugin is loaded into Maya.
MStatus initializePlugin( MObject obj ) {
    // Set plugin registration info: Author, plugin-version and Maya version needed.
    MFnPlugin plugin( obj, "Martin Jørgensen", "1.0", "Any" );
    MStatus stat = plugin.registerShape(
        "simpleShape", simpleShape::id,
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