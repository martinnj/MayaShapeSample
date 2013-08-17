// Include the needed headers.
#include <stdio.h>
#include <maya/MString.h>
#include <maya/MArgList.h>
#include <maya/MFnPlugin.h>
#include <maya/MPxCommand.h>
#include <maya/MIOStream.h>

// Class to represent our command.
class commandExample : public MPxCommand
{
    public:
        commandExample();
        virtual ~commandExample();
        MStatus doIt( const MArgList& );
        MStatus redoIt();
        MStatus undoIt();
        bool isUndoable() const;
        static void* creator();
};