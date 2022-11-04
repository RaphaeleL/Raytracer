#include "kdtree.h"

BoundingBox::BoundingBox() {}

BoundingBox::BoundingBox(Vector<FLOAT, 3> min, Vector<FLOAT, 3> max)
    : min(min), max(max) {}

void BoundingBox::split(BoundingBox &left, BoundingBox &right) {
  // from here
  // TODO: your code
  FLOAT firstEdge = max[0] - min[0];  // Calc len of frist Edge
  FLOAT secondEdge = max[1] - min[1]; // Calc len of second Edge
  FLOAT thirdEdge = max[2] - min[2];  // Calc len of third Edge
  // is frist the longest edge?
  if (firstEdge < secondEdge && firstEdge < thirdEdge) {
    // calc the center point
    FLOAT center0 = min[0] + (max[0] - min[0]) / 2;
    // calc the left point
    left = {{min[0], min[1], min[2]}, {center0, max[1], max[2]}};
    // calc the right point
    right = {{center0, min[1], min[2]}, {max[0], max[1], max[2]}};
  } else if (secondEdge < secondEdge && secondEdge < thirdEdge) {
    FLOAT center1 = min[1] + (max[1] - min[1]) / 2;
    left = {{min[0], min[1], min[2]}, {max[0], center1, max[2]}};
    right = {{min[0], center1, min[2]}, {max[0], max[1], max[2]}};
  } else {
    FLOAT center2 = min[2] + (max[2] - min[2]) / 2;
    left = {{min[0], min[1], min[2]}, {max[0], max[1], center2}};
    right = {{min[0], min[1], center2}, {max[0], max[1], max[2]}};
  }
  // to here
}

bool BoundingBox::contains(Vector<FLOAT, 3> v) {
  // from here
  // TODO: your code
  // check if the first is inside
  if (v[0] < min[0] && v[0] > max[0]) {
    return false;
  } else if (v[1] < min[1] && v[1] > max[1]) {
    return false;
  } else if (v[2] < min[2] && v[2] > max[2]) {
    return false;
  }
  return true;
  // to here
}

bool BoundingBox::contains(Triangle<FLOAT> *triangle) {
  // from here
  // TODO: your code
  // check the complete data
  return (contains(triangle->p1) | contains(triangle->p2) |
          contains(triangle->p3));
  // to here
}

bool BoundingBox::intersects(Vector<FLOAT, 3> eye, Vector<FLOAT, 3> direction) {
  // slab test implementation
  FLOAT tmin[3] = {(min[0] - eye[0]) / direction[0],
                   (min[1] - eye[1]) / direction[1],
                   (min[2] - eye[2]) / direction[2]};
  FLOAT tmax[3] = {(max[0] - eye[0]) / direction[0],
                   (max[1] - eye[1]) / direction[1],
                   (max[2] - eye[2]) / direction[2]};
  FLOAT tminimum = std::min(tmin[0], tmax[0]);
  FLOAT tmaximum = std::max(tmin[0], tmax[0]);
  tminimum = std::max(tminimum, std::min(tmin[1], tmax[1]));
  tmaximum = std::min(tmaximum, std::max(tmin[1], tmax[1]));
  tminimum = std::max(tminimum, std::min(tmin[2], tmax[2]));
  tmaximum = std::min(tmaximum, std::max(tmin[2], tmax[2]));

  return tmaximum >= tminimum;
}

KDTree::~KDTree() {
  delete left;
  delete right;
}

KDTree *KDTree::buildTree(KDTree *tree,
                          std::vector<Triangle<FLOAT> *> &triangles) {
  // from here
  // TODO: your code
  tree->left = new KDTree();                            // create a left tree
  tree->right = new KDTree();                           // create a right tree
  tree->box.split(tree->left->box, tree->right->box);   // split them
  std::vector<Triangle<FLOAT> *> leftTriangles = {};    // create vec for them
  std::vector<Triangle<FLOAT> *> rightTriangles = {};   // create vec for them
  for (Triangle<FLOAT> *triangle : triangles) {         // iterate all triangles
    bool inLeft = tree->left->box.contains(triangle);   // are they inside left?
    bool inRight = tree->right->box.contains(triangle); // or right?
    if (inLeft && inRight) {                            // start to handle them
      tree->triangles.push_back(triangle);
    } else if (inLeft) {
      leftTriangles.push_back(triangle);
    } else {
      rightTriangles.push_back(triangle);
    }
  }
  tree->left->buildTree(tree->left, leftTriangles);    // build the left tree
  tree->right->buildTree(tree->right, rightTriangles); // build the right one
  // to here
  return tree;
}

KDTree *KDTree::buildTree(std::vector<Triangle<FLOAT> *> &triangles) {
  KDTree *root = new KDTree();
  // from here
  // TODO: your code
  Vector<FLOAT, 3> s = {INFINITY, INFINITY, INFINITY};    // define a min vec
  Vector<FLOAT, 3> b = {-INFINITY, -INFINITY, -INFINITY}; // define a max vec
  for (Triangle<FLOAT> *triangle : triangles) {        // iterate the triangles
    for (int axis = 0; axis < 3; ++axis) {             // iterate the axis
      s[axis] = std::min(s[axis], triangle->p1[axis]); // find min for p1
      s[axis] = std::min(s[axis], triangle->p2[axis]);
      s[axis] = std::min(s[axis], triangle->p3[axis]);
      b[axis] = std::max(b[axis], triangle->p1[axis]); // find max for p1
      b[axis] = std::max(b[axis], triangle->p2[axis]);
      b[axis] = std::max(b[axis], triangle->p3[axis]);
    }
  }
  root->box = {s, b};               // create box
  root->buildTree(root, triangles); // build the tree
  // to here
  return root;
}

bool KDTree::hasNearestTriangle(Vector<FLOAT, 3> eye,
                                Vector<FLOAT, 3> direction,
                                Triangle<FLOAT> *&nearest_triangle, FLOAT &t,
                                FLOAT &u, FLOAT &v, FLOAT minimum_t) {
  // from here
  // TODO: your code
  FLOAT minimum_u = 0.0;
  FLOAT minimum_v = 0.0;
  // KDTree contains a triangle that intersects with the ray
  if (this->box.intersects(eye, direction)) {
    return true;
  }
  // get minimum u, v and t for the left side
  if (this->left != nullptr &&
      this->left->hasNearestTriangle(eye, direction, nearest_triangle, t, u, v,
                                     minimum_t)) {
    minimum_t = t;
    minimum_u = u;
    minimum_v = v;
  }
  // get minimum u, v and t for the right side
  if (this->right != nullptr &&
      this->right->hasNearestTriangle(eye, direction, nearest_triangle, t, u, v,
                                      minimum_t)) {
    minimum_t = t;
    minimum_u = u;
    minimum_v = v;
  }
  // iterate the triangles
  for (Triangle<FLOAT> *triangle : this->triangles) {
    if (triangle->intersects(eye, direction, t, u, v, minimum_t)) {
      if (nearest_triangle == nullptr || t < minimum_t) {
        minimum_t = t;
        minimum_u = u;
        minimum_v = v;
        nearest_triangle = triangle;
      }
    }
  }
  // reset the main values
  t = minimum_t;
  u = minimum_u;
  v = minimum_v;
  // to here
  return nearest_triangle != nullptr;
}
