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

    // Add draw requests for components
    if ( !objectAndActiveOnly ) 
    {
        // Inactive components
        if ( (appearance == M3dView::kPoints) ||
             (appearance == M3dView::kHilite) )
        {
            MDrawRequest vertexRequest = info.getPrototype( *this );
            vertexRequest.setDrawData ( data );
            vertexRequest.setToken( kDrawVertices );
            vertexRequest.setColor( DORMANT_VERTEX_COLOR, M3dView::kActiveColors);
            queue.add( vertexRequest );
        }

        // Active components
        if ( surfaceShape()->hasActiveComponents() )
        {
            MDrawRequest activeVertexRequest = info.getPrototype( *this );
            activeVertexRequest.setDrawData ( data );
            activeVertexRequest.setToken( kDrawVertices );
            activeVertexRequest.setColor( ACTIVE_VERTEX_COLOR, M3dView::kActiveColors);

            MObjectArray clist = surfaceShape()->activeComponents();
            MObject vertexComponent = clist[0];
            activeVertexRequest.setComponent( vertexComponent );
            queue.add( activeVertexRequest );
        }
    }
}

void simpleShapeUI::draw( const MDrawRequest & request, M3dView & view ) const
// Main (openGL) draw routine
{
    // Extract token from request, this shows what should be drawn
    int token = request.token();
    switch ( token )
    {
    case kDrawWireframe:
    case kDrawWireframeOnShaded:
    case kDrawVertices:
        drawVertices( request, view );
        break;
    case kDrawSmoothShaded:
        break; // Not implemented, can be done as exercise.
    case kDrawFlatShaded:
        break; // Not implemented, can be done as exercise.
    default:
        break;
    }
}

bool simpleShapeUI::select( MSelectInfo &selectInfo, MSelectionList &selectionList,
                            MPointArray &worldSpaceSelectPts ) const
// Main selection routine
{
    bool selected = false;
	bool componentSelected = false;
	bool hilited = false;

	hilited = (selectInfo.displayStatus() == M3dView::kHilite);
	if ( hilited )
	{
		componentSelected = selectVertices( selectInfo, selectionList, worldSpaceSelectPts );
		selected = selected || componentSelected;
	}

	if ( !selected )
	{
		// NOTE: If the geometry has an intersect routine it should
		// be called here with the selection ray to determine if the
		// the object was selected.

		selected = true;
		MSelectionMask priorityMask( MSelectionMask::kSelectNurbsSurfaces );
		MSelectionList item;
		item.add( selectInfo.selectPath() );
		MPoint xformedPt;
		if ( selectInfo.singleSelection() )
		{
			MPoint center = surfaceShape()->boundingBox().center();
			xformedPt = center;
			xformedPt *= selectInfo.selectPath().inclusiveMatrix();
		}

		selectInfo.addSelection( item, xformedPt, selectionList, worldSpaceSelectPts, priorityMask, false );

	}

	return selected;
}


/*
 * Helper routines
 */

void simpleShapeUI::drawVertices( const MDrawRequest & request, M3dView & view ) const
// Component ( vertex ) drawing routine.
{
	MDrawData data = request.drawData();
	MVectorArray * geom = (MVectorArray*)data.geometry();

	view.beginGL();

	// Query current state so it can be restored
	bool lightingWasOn = glIsEnabled( GL_LIGHTING ) ? true : false;
	if ( lightingWasOn )
	{
		glDisable( GL_LIGHTING );
	}
	float lastPointSize;
	glGetFloatv ( GL_POINT_SIZE, &lastPointSize );

	// Set the point size of the vertices.
	glPointSize( POINT_SIZE );

	// If there is a component specified by the draw request
	// then loop over comp (using an MFnComponent class) and draw the
	// active vertices, otherwise draw all vertices.
	MObject comp = request.component();
	if ( ! comp.isNull() )
	{
		MFnSingleIndexedComponent fnComponent ( comp );
		for ( int i=0; i < fnComponent.elementCount(); i++ )
		{
			int index = fnComponent.element( i );
			glBegin( GL_POINTS );
			MVector& point = (*geom)[index];
			glVertex3f( (float) point[0],
				        (float) point[1],
						(float) point[2] );
			glEnd();

			char annotation[32];
			sprintf( annotation, "%d", index );
			view.drawText( annotation, point );
		}
	}
	else {
		for ( unsigned int i=0; i<geom->length(); i++)
		{
			glBegin( GL_POINTS );
			MVector point = (*geom)[i];
			glVertex3f( (float) point[0],
				        (float) point[1],
						(float) point[2] );
			glEnd();
		}
	}

	// Restore the state we saved earlier.
	if ( lightingWasOn ) {
		glEnable( GL_LIGHTING );
	}
	glPointSize( lastPointSize );

	view.endGL();
}

bool simpleShapeUI::selectVertices( MSelectInfo &selectInfo,
								    MSelectionList &selectionList,
									MPointArray &worldSpaceSelectPts ) const
// Vertex selection
{
	bool selected = false;
	M3dView view = selectInfo.view();

	MPoint xformedPoint;
	MPoint currentPoint;
	MPoint selectionPoint;
	double z,previousZ = 0.0;
	int    closestPointVertexIndex = -1;

	const MDagPath & path = selectInfo.multiPath();

	// Create a component that will store the selected vertices.
	MFnSingleIndexedComponent fnComponent;
	MObject surfaceComponent = fnComponent.create( MFn::kMeshVertComponent );
	int vertexIndex;

	// if the user did a single mouse click and we find > 1 selection
	// we will use the alignmentMatrix to find out which is the closest
	
	MMatrix alignmentMatrix;
	MPoint singlePoint;
	bool singleSelection = selectInfo.singleSelection();
	if ( singleSelection )
	{
		alignmentMatrix = selectInfo.getAlignmentMatrix();
	}

	// Get the geometry information.
	simpleShape* shape = (simpleShape*) surfaceShape();
	MVectorArray* geomPtr = shape->getControlPoints();
	MVectorArray& geom = *geomPtr;

	// Loop through all vertices of the mesh to check if they are within
	// the selection area.
	int numVertices = geom.length();
	for ( vertexIndex=0; vertexIndex<numVertices; vertexIndex++ )
	{
		const MVector& point = geom[vertexIndex];
		
		// Set openGL's rendermode to select and store selected items in a pick buffer.
		view.beginSelect();
		glBegin( GL_POINTS );
		glVertex3f( (float)point[0],
			        (float)point[1],
					(float)point[2]);
		glEnd();

		if ( view.endSelect() > 0)
		{
			selected = true;

			if ( singleSelection )
			{
				xformedPoint = currentPoint;
				xformedPoint.homogenize();
				xformedPoint*=alignmentMatrix;
				z = xformedPoint.z;

				if ( closestPointVertexIndex < 0 || z > previousZ )
				{
					closestPointVertexIndex = vertexIndex;
					singlePoint = currentPoint;
					previousZ = z;
				}
			}
			else
			{
				// Multible selection, store all elements
				fnComponent.addElement( vertexIndex );
			}
		}
	}

	// If single selection, insert closest point into the array
	if ( selected && selectInfo.singleSelection() )
	{
		fnComponent.addElement( closestPointVertexIndex );
		// Need to get world space coordinates for the vertex
		selectionPoint = singlePoint;
		selectionPoint *= path.inclusiveMatrix();
	}

	// Add the selected component to the selection list.
	if ( selected )
	{
		MSelectionList selectionItem;
		selectionItem.add( path, surfaceComponent );

		MSelectionMask mask( MSelectionMask::kSelectComponentsMask );
		selectInfo.addSelection(
			selectionItem,
			selectionPoint,
			selectionList,
			worldSpaceSelectPts,
			mask,
			true );
	}

	return selected;
}
