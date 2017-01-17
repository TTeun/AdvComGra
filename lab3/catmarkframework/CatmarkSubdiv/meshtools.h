#ifndef MESHTOOLS_H
#define MESHTOOLS_H

#include "mesh.h"
#include <QVector3D>

void subdivideCatmullClark(Mesh* inputMesh, Mesh *subdivMesh);
void toLimitMesh(Mesh* inputMesh, Mesh *limitMesh); // Project mesh to its limit

QVector3D vertexPoint(HalfEdge* firstEdge, Mesh *newMesh);
QVector3D edgePoint(HalfEdge* firstEdge, Mesh* newMesh);
QVector3D facePoint(HalfEdge* firstEdge);
float chaiSharpness(HalfEdge *currentEdge);

HalfEdge* vertOnBoundary(Vertex* currentVertex);

void splitHalfEdges(Mesh* inputMesh, Mesh* subdivMesh, unsigned int numHalfEdges, unsigned int numVertPts, unsigned int numFacePts);

#endif // MESHTOOLS_H
