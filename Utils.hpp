#pragma once

#include "PolygonalMesh.hpp"

namespace PolygonLibrary{

bool ImportMesh(PolygonalMesh& mesh);

bool ImportCell0Ds(PolygonalMesh& mesh);

bool ImportCell1Ds(PolygonalMesh& mesh);

bool ImportCell2Ds(PolygonalMesh& mesh);

bool CheckEdgeLengthNonZero(const Vector2d& vertex1, const Vector2d& vertex2);

bool CheckTriangleAreaNonZero(const vector<unsigned int>& vertices, const vector<Vector2d>& coordinates);

}
