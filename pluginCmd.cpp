/* SAMPLE PLUGIN FROM MAYA
 * INFO:
 * The plugin is a more "documented" version of the sample command from
 * Maya 2013's API documentation[0] and I recommend reading the API if
 * youre looking for help on Maya plugins.
 * It registers a MEL command that is then callable from within Maya,
 * and contains both undo and redo functions.
 *
 * [0] : (http://docs.autodesk.com/MAYAUL/2013/ENU/Maya-API-Documentation/files/Command_plugins_MPxCommand.htm)
 */

// Include the header for the file.
#include "pluginCmd.h"

// Constructor for the command object.
commandExample::commandExample() {
    cout << "In commandExample::commandExample()" << endl;
}
// Destructor for the command object.
commandExample::~commandExample() {
    cout << "In commandExample::~commandExample()" << endl;
}
// The actual command/work to be performed.
MStatus commandExample::doIt( const MArgList& ) {
    cout << "In commandExample::doIt()" << endl;
    return MS::kSuccess;
}
// Function that is called when Maya asks for a "redo".
MStatus commandExample::redoIt() {
    cout << "In commandExample::redoIt()" << endl;
    return MS::kSuccess;
}
// Function that is called when Maya asks for a "undo"
MStatus commandExample::undoIt() {
    cout << "In commandExample::undoIt()" << endl;
    return MS::kSuccess;
}
// Function that tells Maya if the command can be undone/redone,
// if it returns false, the destructor will be called right after this.
bool commandExample::isUndoable() const {
    cout << "In commandExample::isUndoable()" << endl;
    return true;
}

// The creator is called when the command is invoked and sets up the command object.
void* commandExample::creator() {
    cout << "In commandExample::creator()" << endl;
    return new commandExample();
}

// Gets called when the plugin is loaded into Maya.
MStatus initializePlugin( MObject obj ) {
    // Set plugin registration info: Author, plugin-version and Maya version needed.
    MFnPlugin plugin( obj, "Martin Jørgensen", "1.0", "Any" );
    plugin.registerCommand( "commandExample", commandExample::creator );

    // Print to show plugin command was registered.
    cout << "In initializePlugin()" << endl;

    return MS::kSuccess;
}
// Gets called when the plugin is unloaded from Maya.
MStatus uninitializePlugin( MObject obj )
{
    MFnPlugin plugin( obj );
    plugin.deregisterCommand( "commandExample" );

    // Print to show the plugin was unloaded.
    cout << "In uninitializePlugin()" << endl;
    return MS::kSuccess;
}