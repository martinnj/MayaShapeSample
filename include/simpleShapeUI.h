// Include the parent header from Maya and the header containing the shape.
#include <maya\MPxSurfaceShapeUI.h>
#include "simpleShape.h"

// This class will handle all drawing and selection for the shape.
class simpleShapeUI : public MPxSurfaceShapeUI
{
public:
    simpleShapeUI();
    virtual ~simpleShapeUI();

    /*
     * Overrides of the parent class
     */

    //Puts a draw request unto Maya's draw queue
    virtual void getDrawRequests( const MDrawInfo   & info,
                                  bool                objectAndActiveOnly,
                                  MDrawRequestQueue & requests );

    // Main draw function, Maya calls this with draw requests.
    virtual void draw( const MDrawRequest & request,
                             M3dView      & view ) const;

    // Main selection routine
    virtual bool select( MSelectInfo    &selectInfo,
                         MSelectionList &selectionList,
                         MPointArray    &worldSpaceSelectPts ) const;

    /*
     * AUX STUFF
     */
    void drawVertices  ( const MDrawRequest   & request, M3dView & view ) const;

    bool selectVertices(       MSelectInfo    &selectInfo,
                               MSelectionList &selectionList,
                               MPointArray    &worldSpaceSelectPts ) const;

    static void * creator();

private:
    // Draw tokens, what does the viewport/render want?
    enum {
        kDrawVertices, // What we're gonna use here.
        kDrawWireframe,
        kDrawWireframeOnShaded,
        kDrawSmoothShaded,
        kDrawFlatShaded,
        kLastToken
    };
};