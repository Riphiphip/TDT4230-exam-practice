#pragma once
#include <vector>
#include "meshUtil.hpp"

Mesh createBezierMesh(std::vector<Vertex> controlPoints, unsigned int nSegments);