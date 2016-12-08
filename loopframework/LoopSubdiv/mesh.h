#ifndef MESH_H
#define MESH_H

#include <QVector>

#include "vertex.h"
#include "face.h"
#include "halfedge.h"

#include "objfile.h"

class Mesh {

public:
  Mesh();
  Mesh(OBJFile *loadedOBJFile);
  ~Mesh();

  void setTwins(unsigned int numHalfEdges, unsigned int indexH);

  QVector<Vertex> Vertices;
  QVector<Face> Faces;
  QVector<HalfEdge> HalfEdges;

  QVector<QVector<unsigned int> > PotentialTwins;

  void setFaceNormal(Face* currentFace);
  QVector3D computeVertexNormal(Vertex* currentVertex);

  // For debugging
  void dispVertInfo(Vertex* dVert);
  void dispHalfEdgeInfo(HalfEdge* dHalfEdge);
  void dispFaceInfo(Face* dFace);

};

#endif // MESH_H
