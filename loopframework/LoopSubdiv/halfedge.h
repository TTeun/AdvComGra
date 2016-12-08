#ifndef HALFEDGE
#define HALFEDGE

// Forward declarations
class Vertex;
class Face;

class HalfEdge {

public:
  Vertex* target;
  HalfEdge* next;
  HalfEdge* prev;
  HalfEdge* twin;
  Face* polygon;
  unsigned int index;
  unsigned short sharpness;

  // Inline constructors

  HalfEdge() {
    target = 0; //nullptr
    next = 0;
    prev = 0;
    twin = 0;
    polygon = 0;
    index = 0;
    sharpness = 0;
  }

  HalfEdge(Vertex* htarget, HalfEdge* hnext, HalfEdge* hprev, HalfEdge* htwin, Face* hpolygon, unsigned int hindex, float hsharpness=0) {
    target = htarget;
    next = hnext;
    prev = hprev;
    twin = htwin;
    polygon = hpolygon;
    index = hindex;
    sharpness = hsharpness;
  }
};

#endif // HALFEDGE
