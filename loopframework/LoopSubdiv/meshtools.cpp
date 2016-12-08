#include "meshtools.h"

void subdivideLoop(Mesh* inputMesh, Mesh* subdivMesh) {
  unsigned int numVerts, numHalfEdges, numFaces;
  unsigned int k, m, s, t;
  unsigned int vIndex, hIndex, fIndex;
  unsigned short n;
  HalfEdge* currentEdge;

  qDebug() << ":: Creating new Loop mesh";

  numVerts = inputMesh->Vertices.size();
  numHalfEdges = inputMesh->HalfEdges.size();
  numFaces = inputMesh->Faces.size();

  // Reserve memory
  subdivMesh->Vertices.reserve(numVerts + numHalfEdges/2);
  subdivMesh->HalfEdges.reserve(2*numHalfEdges + 6*numFaces);
  subdivMesh->Faces.reserve(4*numFaces);

  // Create vertex points
  for (k=0; k<numVerts; k++) {
    n = inputMesh->Vertices[k].val;
    // Coords (x,y,z), Out, Valence, Index
    subdivMesh->Vertices.append( Vertex( vertexPoint(2, inputMesh->Vertices[k].out),
                                         nullptr,
                                         n,
                                         k) );
  }

  vIndex = numVerts;
  qDebug() << " * Created vertex points";

  // Create edge points
  for (k=0; k<numHalfEdges; k++) {
    currentEdge = &inputMesh->HalfEdges[k];

    if (k < currentEdge->twin->index) {
      // Coords (x,y,z), Out, Valence, Index
      subdivMesh->Vertices.append( Vertex(edgePoint(2, currentEdge),
                                          nullptr,
                                          6,
                                          vIndex) );
      vIndex++;
    }
  }

  qDebug() << " * Created edge points";

  // Split halfedges
  splitHalfEdges(inputMesh, subdivMesh, numHalfEdges, numVerts, 0);

  qDebug() << " * Split halfedges";

  hIndex = 2*numHalfEdges;
  fIndex = 0;

  // Create faces and remaining halfedges
  for (k=0; k<numFaces; k++) {
    currentEdge = inputMesh->Faces[k].side;

    // Three outer faces

    for (m=0; m<3; m++) {

      s = currentEdge->prev->index;
      t = currentEdge->index;

      // Side, Val, Index
      subdivMesh->Faces.append(Face(nullptr,
                                    3,
                                    fIndex));

      subdivMesh->Faces[fIndex].side = &subdivMesh->HalfEdges[ 2*t ];

      // Target, Next, Prev, Twin, Poly, Index
      subdivMesh->HalfEdges.append(HalfEdge( subdivMesh->HalfEdges[2*s].target,
                                             &subdivMesh->HalfEdges[2*s+1],
                                             &subdivMesh->HalfEdges[ 2*t ],
                                             nullptr,
                                             &subdivMesh->Faces[fIndex],
                                             hIndex ));

      subdivMesh->HalfEdges.append(HalfEdge( subdivMesh->HalfEdges[2*t].target,
                                             nullptr,
                                             nullptr,
                                             &subdivMesh->HalfEdges[hIndex],
                                             nullptr,
                                             hIndex+1 ));

      subdivMesh->HalfEdges[hIndex].twin = &subdivMesh->HalfEdges[hIndex+1];

      subdivMesh->HalfEdges[2*s+1].next = &subdivMesh->HalfEdges[2*t];
      subdivMesh->HalfEdges[2*s+1].prev = &subdivMesh->HalfEdges[hIndex];
      subdivMesh->HalfEdges[2*s+1].polygon = &subdivMesh->Faces[fIndex];

      subdivMesh->HalfEdges[2*t].next = &subdivMesh->HalfEdges[hIndex];
      subdivMesh->HalfEdges[2*t].prev = &subdivMesh->HalfEdges[2*s+1];
      subdivMesh->HalfEdges[2*t].polygon = &subdivMesh->Faces[fIndex];

      // For edge points
      subdivMesh->HalfEdges[2*t].target->out = &subdivMesh->HalfEdges[hIndex];

      hIndex += 2;
      fIndex++;
      currentEdge = currentEdge->next;

    }

    // Inner face

    // Side, Val, Index
    subdivMesh->Faces.append(Face(nullptr,
                                  3,
                                  fIndex));

    subdivMesh->Faces[fIndex].side = &subdivMesh->HalfEdges[ hIndex-1 ];

    for (m=0; m<3; m++) {

      if (m==2) {
        subdivMesh->HalfEdges[hIndex - 1].next = &subdivMesh->HalfEdges[hIndex - 5];
      }
      else {
        subdivMesh->HalfEdges[hIndex - 5 + 2*m].next = &subdivMesh->HalfEdges[hIndex - 5 + 2*(m+1)];
      }

      if (m==0) {
        subdivMesh->HalfEdges[hIndex - 5].prev = &subdivMesh->HalfEdges[hIndex - 1];
      }
      else {
        subdivMesh->HalfEdges[hIndex - 5 + 2*m].prev = &subdivMesh->HalfEdges[hIndex - 5 + 2*(m-1)];
      }

      subdivMesh->HalfEdges[hIndex - 5 + 2*m].polygon = &subdivMesh->Faces[fIndex];

    }

    fIndex++;

  }

  qDebug() << " * Created faces";

  // For vertex points
  for (k=0; k<numVerts; k++) {
    subdivMesh->Vertices[k].out = &subdivMesh->HalfEdges[ 2*inputMesh->Vertices[k].out->index ];
  }

}

// ---

QVector3D vertexPoint(unsigned short subdivType, HalfEdge* firstEdge) {
  unsigned short k, n;
  QVector3D sumStarPts, sumFacePts;
  QVector3D vertexPt;
  float stencilValue;
  HalfEdge* currentEdge;
  Vertex* currentVertex;

  currentVertex = firstEdge->twin->target;
  n = currentVertex->val;

  sumStarPts = QVector3D();
  sumFacePts = QVector3D();
  currentEdge = firstEdge;

  switch (subdivType) {
  case 2:
    // Loop
    for (k=0; k<n; k++) {
      sumStarPts += currentEdge->target->coords;
      currentEdge = currentEdge->prev->twin;
    }

    // Warren's rules
    if (n == 3) {
      stencilValue = 3.0/16.0;
    }
    else {
      stencilValue = 3.0/(8*n);
    }

    vertexPt = (1.0 - n*stencilValue) * currentVertex->coords + stencilValue * sumStarPts;
    break;
  }

  return vertexPt;

}

QVector3D edgePoint(unsigned short subdivType, HalfEdge* firstEdge) {
  QVector3D EdgePt;
  HalfEdge* currentEdge;

  EdgePt = QVector3D();
  currentEdge = firstEdge;

  switch (subdivType) {
  case 2:
    // Loop
    EdgePt = QVector3D();
    currentEdge = firstEdge;
    EdgePt  = 6.0 * currentEdge->target->coords;
    EdgePt += 2.0 * currentEdge->next->target->coords;
    EdgePt += 6.0 * currentEdge->twin->target->coords;
    EdgePt += 2.0 * currentEdge->twin->next->target->coords;
    EdgePt /= 16.0;
    break;
  }

  return EdgePt;

}

void splitHalfEdges(Mesh* inputMesh, Mesh* subdivMesh, unsigned int numHalfEdges, unsigned int numVertPts, unsigned int numFacePts) {
  unsigned int k, m;
  unsigned int vIndex;
  HalfEdge* currentEdge;

  vIndex = numFacePts + numVertPts;

  for (k=0; k<numHalfEdges; k++) {
    currentEdge = &inputMesh->HalfEdges[k];
    m = currentEdge->twin->index;

    // Target, Next, Prev, Twin, Poly, Index
    subdivMesh->HalfEdges.append(HalfEdge(nullptr,
                                          nullptr,
                                          nullptr,
                                          nullptr,
                                          nullptr,
                                          2*k));

    subdivMesh->HalfEdges.append(HalfEdge(nullptr,
                                          nullptr,
                                          nullptr,
                                          nullptr,
                                          nullptr,
                                          2*k+1));

    if (k < m) {
      subdivMesh->HalfEdges[2*k].target = &subdivMesh->Vertices[ vIndex ];
      subdivMesh->HalfEdges[2*k+1].target = &subdivMesh->Vertices[ numFacePts + currentEdge->target->index ];
      vIndex++;
    }
    else {
      subdivMesh->HalfEdges[2*k].target = subdivMesh->HalfEdges[2*m].target;
      subdivMesh->HalfEdges[2*k+1].target = &subdivMesh->Vertices[ numFacePts + currentEdge->target->index ];

      // Assign Twins
      subdivMesh->HalfEdges[2*k].twin = &subdivMesh->HalfEdges[2*m+1];
      subdivMesh->HalfEdges[2*k+1].twin = &subdivMesh->HalfEdges[2*m];
      subdivMesh->HalfEdges[2*m].twin = &subdivMesh->HalfEdges[2*k+1];
      subdivMesh->HalfEdges[2*m+1].twin = &subdivMesh->HalfEdges[2*k];
    }
  }

  // Note that Next, Prev and Poly are not yet assigned at this point.

}
