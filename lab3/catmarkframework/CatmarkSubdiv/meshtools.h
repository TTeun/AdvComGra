#ifndef MESHTOOLS_H
#define MESHTOOLS_H

#include "mesh.h"
#include <QVector3D>

void subdivideCatmullClark(Mesh* inputMesh, Mesh *subdivMesh);
void toLimitMesh(Mesh* inputMesh, Mesh *limitMesh);

QVector3D vertexPoint(HalfEdge* firstEdge, Mesh *newMesh);
QVector3D edgePoint(HalfEdge* firstEdge, Mesh* newMesh);
QVector3D facePoint(HalfEdge* firstEdge);

HalfEdge* vertOnBoundary(Vertex* currentVertex);

void splitHalfEdges(Mesh* inputMesh, Mesh* subdivMesh, unsigned int numHalfEdges, unsigned int numVertPts, unsigned int numFacePts);

struct VertexInfo {
    HalfEdge *bdryEdge;
    QVector<HalfEdge*> incidentCreases;
    float sharpness = 0.0;
    unsigned int creaseCount = 0;
};

#endif // MESHTOOLS_H
