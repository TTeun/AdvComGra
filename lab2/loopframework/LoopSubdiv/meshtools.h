#ifndef MESHTOOLS_H
#define MESHTOOLS_H

#include "mesh.h"
#include <QVector3D>

void subdivideLoop(Mesh* inputMesh, Mesh *subdivMesh);

QVector3D vertexPoint(unsigned short subdivType, HalfEdge* firstEdge);
QVector3D edgePoint(unsigned short subdivType, HalfEdge* firstEdge);

void splitHalfEdges(Mesh* inputMesh, Mesh* subdivMesh, unsigned int numHalfEdges, unsigned int numVertPts, unsigned int numFacePts);

#endif // MESHTOOLS_H
