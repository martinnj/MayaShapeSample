// Implementation of the class from simpleShapeUI.h
// This class will do all the UI and renderwork, so it is somewhat
// more massive than the rest. A real workhorse! :)

// We will need this to produce console debugging
#include <maya/MIOStream.h> 

// And this to get the class definition of course.
#include <simpleShapeUI.h>

// And aaallllll of this for drawing stuff.
#include <maya/MMaterial.h>
#include <maya/MSelectionList.h>
#include <maya/MSelectionMask.h>
#include <maya/MDrawData.h>
#include <maya/MMatrix.h>
#include <maya/MFnSingleIndexedComponent.h>
#include <maya/MObjectArray.h>
#include <maya/MDagPath.h>

// Some practical constants we will want:
// Object and component color defines
#define LEAD_COLOR				18	// green
#define ACTIVE_COLOR			15	// white
#define ACTIVE_AFFECTED_COLOR	8	// purple
#define DORMANT_COLOR			4	// blue
#define HILITE_COLOR			17	// pale blue
#define DORMANT_VERTEX_COLOR	8	// purple
#define ACTIVE_VERTEX_COLOR		16	// yellow

// Vertex point size
#define POINT_SIZE				2.0

/*
 * Actual UI implementation
 */
simpleShapeUI::simpleShapeUI() {}
simpleShapeUI::~simpleShapeUI() {}

void* simpleShapeUI::creator()
{
    return new simpleShapeUI();
}

/*
 * Overrides for the parent functions
 */
void simpleShapeUI::getDrawRequests( const MDrawInfo & info,
                                     bool  objectAndActiveOnly,
                                     MDrawRequestQueue & queue)
// Add draw requests to the draw queue
{
    // Get data needed for drawing the shape
    MDrawData data;
    simpleShape* shape   = (simpleShape*) surfaceShape();
    MVectorArray* geomPtr = shape->getControlPoints();

    // This will create a prototype draw request that we can fill in and add to the draw queue.
    MDrawRequest request = info.getPrototype( *this );

    // Put our data into the draw request so it can be used.
    getDrawData( geomPtr, data );
    request.setDrawData( data );

    // Decode the draw info to determine what needs to be drawn. (wireframe, shades, etc)
    M3dView::DisplayStyle  appearance = info.displayStyle();
    M3dView::DisplayStatus displayStatus = info.displayStatus();

    switch ( appearance )
    {
        case M3dView::kWireFrame :
        {
            request.setToken( kDrawWireframe );
            
            M3dView::ColorTable activeColorTable = M3dView::kActiveColors;
            M3dView::ColorTable dormantColorTable = M3dView::kDormantColors;

            switch ( displayStatus )
            {
            case M3dView::kLead :
                request.setColor( LEAD_COLOR, activeColorTable );
                break;
            case M3dView::kActive :
                request.setColor( ACTIVE_COLOR, activeColorTable );
                break;
            case M3dView::kActiveAffected :
                request.setColor( ACTIVE_AFFECTED_COLOR, activeColorTable );
                break;
            case M3dView::kDormant :
                request.setColor( DORMANT_COLOR, dormantColorTable );
                break;
            case M3dView::kHilite :
                request.setColor( HILITE_COLOR, activeColorTable );
                break;
            default:
                break;
            }

            queue.add( request );
            break;
        }

        case M3dView::kGouraudShaded :
        {
            // Create the smooth shaded draw request
            request.setToken( kDrawSmoothShaded );

            // We also need material info
            MDagPath path = info.multiPath(); // Path to our DAG object.
            M3dView  view = info.view();      // View to draw into
            MMaterial material = MPxSurfaceShapeUI::material( path );

            // Evaluate material and if need be: texture
            if ( ! material.evaluateMaterial( view, path ) )
            {
                cerr << "simpleShapeUI: Could not evaluate material" << endl;
            }

            bool drawTexture = true;
            if ( drawTexture && material.materialIsTextured() )
            {
                material.evaluateTexture( data );
            }

            request.setMaterial( material );

            bool materialTransparent = false;
            material.getHasTransparency( materialTransparent );
            if ( materialTransparent )
            {
                request.setIsTransparent( true );
            }

            queue.add( request );

            // Create a draw request for wireframe on shaded if needed
            if ((displayStatus == M3dView::kActive) ||
                (displayStatus == M3dView::kLead) ||
                (displayStatus == M3dView::kHilite))
            {
                MDrawRequest wireRequest = info.getPrototype( *this );
                wireRequest.setDrawData( data );
                wireRequest.setToken( kDrawWireframeOnShaded );
                wireRequest.setDisplayStyle( M3dView::kWireFrame );

                M3dView::ColorTable activeColorTable = M3dView::kActiveColors;

                switch ( displayStatus )
                {
                    case M3dView::kLead :
                        wireRequest.setColor( LEAD_COLOR, activeColorTable );
                        break;
                    case M3dView::kActive :
                        wireRequest.setColor( ACTIVE_COLOR, activeColorTable );
                        break;
                    case M3dView::kHilite :
                        wireRequest.setColor( HILITE_COLOR, activeColorTable );
                        break;
                    default :
                        break;
                }
                queue.add ( wireRequest );
            }
            break;
        }
        case M3dView::kFlatShaded :
            request.setToken ( kDrawFlatShaded );
            break;
        default:
            break;
    }

    // Add draw requests for components 221!!
}