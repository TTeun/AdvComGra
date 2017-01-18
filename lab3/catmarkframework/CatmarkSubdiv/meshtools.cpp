#include "meshtools.h"

float max(double a, double b){
    return a > b ? a : b;
}

void subdivideCatmullClark(Mesh* inputMesh, Mesh* subdivMesh) {
  unsigned int numVerts, numHalfEdges, numFaces, sumFaceValences;
  unsigned int k, m, s, t;
  unsigned int vIndex, hIndex, fIndex;
  unsigned short n;
  HalfEdge* currentEdge;

  qDebug() << ":: Creating new Catmull-Clark mesh";

  numVerts = inputMesh->Vertices.size();
  numHalfEdges = inputMesh->HalfEdges.size();
  numFaces = inputMesh->Faces.size();

  // Reserve memory
  subdivMesh->Vertices.reserve(numFaces + numVerts + numHalfEdges/2);

  sumFaceValences = 0;
  for (k=0; k<numFaces; k++) {
    sumFaceValences += inputMesh->Faces[k].val;
  }

  subdivMesh->HalfEdges.reserve(2*numHalfEdges + 2*sumFaceValences);
  subdivMesh->Faces.reserve(sumFaceValences);

  // Create face points
  for (k=0; k<numFaces; k++) {
    n = inputMesh->Faces[k].val;
    // Coords (x,y,z), Out, Valence, Index
    subdivMesh->Vertices.append( Vertex(facePoint(inputMesh->Faces[k].side),
                                        nullptr,
                                        n,
                                        k) );
  }

  qDebug() << " * Created face points";

  vIndex = numFaces;

  // Create vertex points
  float sharp;
  for (k=0; k<numVerts; k++) {
    n = inputMesh->Vertices[k].val;
    // Coords (x,y,z), Out, Valence, Index
<<<<<<< HEAD
    float sharp=0;
    //Check if a vertex has a sharpness assigned to it
    if(inputMesh->Vertices[k].out->twin->target->sharpness>0){
        sharp = std::max(inputMesh->Vertices[k].out->twin->target->sharpness-1,0);
    }
=======
    sharp = 0.0;
    if(inputMesh->Vertices[k].sharpness)
        sharp = max(inputMesh->Vertices[k].sharpness-1,0); // Create new vertex that is less sharp
>>>>>>> 425e72cd0736a0fad6cf7188bdccef8cde57260c

    subdivMesh->Vertices.append( Vertex(vertexPoint(inputMesh->Vertices[k].out, subdivMesh),
                                        nullptr,
                                        n,
                                        vIndex,
                                        sharp) );
    vIndex++;
  }

  qDebug() << " * Created vertex points";

  // Create edge points
  for (k=0; k<numHalfEdges; k++) {
    currentEdge = &inputMesh->HalfEdges[k];

    if (k < currentEdge->twin->index) {
      m = (!currentEdge->polygon || !currentEdge->twin->polygon) ? 3 : 4;
      // Coords (x,y,z), Out, Valence, Index
      subdivMesh->Vertices.append( Vertex(edgePoint(currentEdge, subdivMesh),
                                          nullptr,
                                          m,
                                          vIndex) );
      vIndex++;
    }
  }

  qDebug() << " * Created edge points";

  // Split halfedges
  splitHalfEdges(inputMesh, subdivMesh, numHalfEdges, numVerts, numFaces);

  qDebug() << " * Split halfedges";

  hIndex = 2*numHalfEdges;
  fIndex = 0;

  // Create faces and remaining halfedges
  for (k=0; k<numFaces; k++) {
    currentEdge = inputMesh->Faces[k].side;
    n = inputMesh->Faces[k].val;

    for (m=0; m<n; m++) {

      s = currentEdge->prev->index;
      t = currentEdge->index;

      // Side, Val, Index
      subdivMesh->Faces.append(Face(nullptr,
                                    4,
                                    fIndex));

      subdivMesh->Faces[fIndex].side = &subdivMesh->HalfEdges[ 2*t ];

      // Target, Next, Prev, Twin, Poly, Index
      subdivMesh->HalfEdges.append(HalfEdge( &subdivMesh->Vertices[k],
                                             nullptr,
                                             &subdivMesh->HalfEdges[ 2*t ],
                                   nullptr,
                                   &subdivMesh->Faces[fIndex],
                                   hIndex ));

      subdivMesh->HalfEdges.append(HalfEdge( nullptr,
                                             &subdivMesh->HalfEdges[2*s+1],
                                   &subdivMesh->HalfEdges[hIndex],
                                   nullptr,
                                   &subdivMesh->Faces[fIndex],
                                   hIndex+1 ));

      subdivMesh->HalfEdges[hIndex].next = &subdivMesh->HalfEdges[hIndex+1];
      subdivMesh->HalfEdges[hIndex+1].target = subdivMesh->HalfEdges[hIndex+1].next->twin->target;

      subdivMesh->HalfEdges[2*s+1].next = &subdivMesh->HalfEdges[2*t];
      subdivMesh->HalfEdges[2*s+1].prev = &subdivMesh->HalfEdges[hIndex+1];
      subdivMesh->HalfEdges[2*s+1].polygon = &subdivMesh->Faces[fIndex];

      subdivMesh->HalfEdges[2*t].next = &subdivMesh->HalfEdges[hIndex];
      subdivMesh->HalfEdges[2*t].prev = &subdivMesh->HalfEdges[2*s+1];
      subdivMesh->HalfEdges[2*t].polygon = &subdivMesh->Faces[fIndex];

      if (m > 0) {
        // Twins
        subdivMesh->HalfEdges[hIndex+1].twin = &subdivMesh->HalfEdges[hIndex-2];
        subdivMesh->HalfEdges[hIndex-2].twin = &subdivMesh->HalfEdges[hIndex+1];
      }

      // For edge points
      subdivMesh->HalfEdges[2*t].target->out = &subdivMesh->HalfEdges[hIndex];

      hIndex += 2;
      fIndex++;
      currentEdge = currentEdge->next;
    }

    subdivMesh->HalfEdges[hIndex-2*n+1].twin = &subdivMesh->HalfEdges[hIndex-2];
    subdivMesh->HalfEdges[hIndex-2].twin = &subdivMesh->HalfEdges[hIndex-2*n+1];

    // For face points
    subdivMesh->Vertices[k].out = &subdivMesh->HalfEdges[hIndex-1];

  }

//  qDebug() << " * Created faces and remaining halfedges";

  // For vertex points
  for (k=0; k<numVerts; k++) {
    subdivMesh->Vertices[numFaces + k].out = &subdivMesh->HalfEdges[ 2*inputMesh->Vertices[k].out->index ];
  }

  qDebug() << " * Completed!";
  qDebug() << "   # Vertices:" << subdivMesh->Vertices.size();
  qDebug() << "   # HalfEdges:" << subdivMesh->HalfEdges.size();
  qDebug() << "   # Faces:" << subdivMesh->Faces.size();

  // Here we have a loop that does the sharpness smoothing
  float sharpness;
  for (int i = 0; i < inputMesh->HalfEdges.size(); ++i){
      currentEdge = &inputMesh->HalfEdges[i];
      if (currentEdge->sharpness){
          sharpness = chaiSharpness(currentEdge);
          qDebug() << "sharpness" << sharpness;
          subdivMesh->HalfEdges[2 * i + 1].sharpness = sharpness;
          subdivMesh->HalfEdges[2 * i + 1].twin->sharpness = sharpness;
        }
    }

}

float chaiSharpness(HalfEdge *currentEdge){
  // Here we loop over all other edges incident upon currentEdge->target to find out
  // how sharp the newly created edges should be

  size_t n = currentEdge->target->val;
  size_t counter = 1;   // How many sharp edges meet at currentEdge->target? One so far
  float sharpness = currentEdge->sharpness;
  float otherSharpness; // Store sharpness of other edges, only used if there are exactly
                        // two sharp incident edges

  for (size_t i = 1; i < n; ++i){
      currentEdge = currentEdge->next->twin;
      if (currentEdge->sharpness){
        ++counter;
        otherSharpness = currentEdge->sharpness;
        }
      if (counter > 2) // currentEdge points to a corner
        return max(0.0, sharpness - 1);
    }
  if (counter == 1)    // // currentEdge points to a dart
    return max(0.0, sharpness - 1);

  // currentEdge is part of a crease
  return max(0.0, 0.25 * (3.0 * sharpness + otherSharpness) - 1.0);
}

QVector3D ccVertexPoint(HalfEdge* firstEdge, Mesh* subdivMesh) {
  // The standard CC new vertex position function
  unsigned short k, n;
  QVector3D sumStarPts, sumFacePts;
  QVector3D vertexPt;

  HalfEdge* currentEdge;
  Vertex* currentVertex;

  currentVertex = firstEdge->twin->target;
  n = currentVertex->val;

  sumStarPts = QVector3D();
  sumFacePts = QVector3D();
  currentEdge = firstEdge;

  // Catmull-Clark (also supporting initial meshes containing n-gons)
  if (HalfEdge* boundaryEdge = vertOnBoundary(currentVertex)) {
    if (boundaryEdge->twin->target->val == 2) {
      // Interpolate corners
      return boundaryEdge->twin->target->coords;
    }
    else {
      vertexPt  = 1.0 * boundaryEdge->target->coords;
      vertexPt += 6.0 * boundaryEdge->twin->target->coords;
      vertexPt += 1.0 * boundaryEdge->prev->twin->target->coords;
      vertexPt /= 8.0;
    }
  }
  else {
    for (k=0; k<n; k++) {
      sumStarPts += currentEdge->target->coords;
      sumFacePts += subdivMesh->Vertices[currentEdge->polygon->index].coords;
      currentEdge = currentEdge->prev->twin;
    }

    vertexPt = ((n-2)*currentVertex->coords + sumStarPts/n + sumFacePts/n)/n;
  }

  return vertexPt;

}

QVector3D vertexPoint(HalfEdge* firstEdge, Mesh* subdivMesh) {

  Vertex* currentVertex;
  currentVertex = firstEdge->twin->target;

  if(currentVertex->sharpness) //  Vertex is tagged as sharp
    return currentVertex->coords;

  QVector3D vertexPt = QVector3D(0.0, 0.0, 0.0);
  HalfEdge* currentEdge;

  unsigned short n = currentVertex->val;

  float sharpness = 0.0;
  size_t incidentCreases = 0;
  currentEdge = firstEdge;

<<<<<<< HEAD
  if(currentVertex->sharpness>0){
      //Sharp vertex case
      if (currentVertex->sharpness >= 3) // Vertex is a corner
          return currentVertex->coords;
  }else{
    //Normal vertex case
=======

>>>>>>> 425e72cd0736a0fad6cf7188bdccef8cde57260c
  for (size_t i = 0; i < n; ++i){
      if (currentEdge->sharpness > 0.0){
          sharpness += currentEdge->sharpness;
          ++incidentCreases;
          vertexPt += ccVertexPoint(currentEdge, subdivMesh);
      }
      currentEdge = currentEdge->prev->twin;
  }


  // At this point, vertexPt is the sum of the vertices connected to currentVertex by sharp edges.
  if (incidentCreases >= 3) // Vertex is a corner
      return currentVertex->coords;

  if (incidentCreases == 2){
      vertexPt += 6.0 * currentVertex->coords;
      vertexPt /= 8.0;
      if (sharpness < 2.0){
          sharpness /= 2; // Average edge sharpness of incident edges
          vertexPt *= sharpness;
          vertexPt += (1-sharpness) * ccVertexPoint(currentEdge, subdivMesh);
      }
      return vertexPt;
  }

  // return smooth vertex position as incidentCreases < 2
  return ccVertexPoint(currentEdge, subdivMesh);
}

QVector3D ccEdgePoint(HalfEdge *currentEdge, Mesh *subdivMesh){
  // smooth CC edge point position
    QVector3D point;
    point  = currentEdge->target->coords;
    point += currentEdge->twin->target->coords;
    point += subdivMesh->Vertices[currentEdge->polygon->index].coords;
    point += subdivMesh->Vertices[currentEdge->twin->polygon->index].coords;
    return point / 4.0;
}

QVector3D avEdgePoint(HalfEdge *currentEdge){
  // boundary of sharp rule for edge point
    QVector3D point;
    point = currentEdge->target->coords;
    point += currentEdge->twin->target->coords;
    return point / 2.0;
}

QVector3D edgePoint(HalfEdge* edge, Mesh* subdivMesh) {
  HalfEdge* currentEdge;
  currentEdge = edge;

  float sharpness = currentEdge->sharpness;
  if (!currentEdge->polygon || !currentEdge->twin->polygon || sharpness > 1.0) // Edge is boundary or sharpness > 1.0
    return avEdgePoint(currentEdge);

  if (sharpness > 0.0){ // sharpness  > 0.0, lerp between smooth and sharp
      return sharpness * avEdgePoint(currentEdge) + (1-sharpness) * ccEdgePoint(currentEdge, subdivMesh);
  }

  // Edge is smmooth
  return ccEdgePoint(currentEdge, subdivMesh);
}

QVector3D facePoint(HalfEdge* firstEdge)
{
  unsigned short k, n;
  QVector<float> stencil;
  QVector3D facePt;
  HalfEdge* currentEdge;

  n = firstEdge->polygon->val;

  // Bilinear, Catmull-Clark, Dual
  stencil.clear();
  stencil.fill(1.0/n, n);

  facePt = QVector3D();
  currentEdge = firstEdge;

  for (k=0; k<n; k++) {
    // General approach
    facePt += stencil[k] * currentEdge->target->coords;
    currentEdge = currentEdge->next;
  }

  return facePt;

}

HalfEdge* vertOnBoundary(Vertex* currentVertex) {

  unsigned short n = currentVertex->val;
  int k;
  HalfEdge* currentEdge = currentVertex->out;

  for (k=0; k<n; k++) {
    if (!currentEdge->polygon) {
      return currentEdge;
    }
    currentEdge = currentEdge->prev->twin;
  }

  return nullptr;
}

// For Bilinear, Catmull-Clark and Loop
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

      // Boundary edges are added last when importing a mesh, so their index will always be higher than their twins.
      if (!currentEdge->polygon) {
        subdivMesh->HalfEdges[2*k].next = &subdivMesh->HalfEdges[2*k+1];
        subdivMesh->HalfEdges[2*k+1].prev = &subdivMesh->HalfEdges[2*k];

        if (currentEdge > currentEdge->next) {
          m = currentEdge->next->index;
          subdivMesh->HalfEdges[2*k+1].next = &subdivMesh->HalfEdges[2*m];
          subdivMesh->HalfEdges[2*m].prev = &subdivMesh->HalfEdges[2*k+1];
        }

        if (currentEdge > currentEdge->prev) {
          m = currentEdge->prev->index;
          subdivMesh->HalfEdges[2*k].prev = &subdivMesh->HalfEdges[2*m+1];
          subdivMesh->HalfEdges[2*m+1].next = &subdivMesh->HalfEdges[2*k];
        }
      }
    }
  }

  // Note that Next, Prev and Poly are not yet assigned at this point.

}

QVector3D faceAverage(Face* inputFace){
    int n = inputFace->val;
    HalfEdge* currentEdge = inputFace->side;
    QVector3D average = QVector3D(0.0, 0.0, 0.0);
    for (int k = 0; k < n; ++k){
        average += currentEdge->target->coords;
        currentEdge = currentEdge->next;
    }
    return average / n;
}
