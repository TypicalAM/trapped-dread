#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Point3D {
  float x, y, z, w;
};

struct Normal3D {
  float x, y, z;
};

struct TexCoord2D {
  float u, v;
};

struct Face {
  std::vector<int> vertexIndices;
  std::vector<int> normalIndices;
  std::vector<int> texCoordIndices;
};

void loadOBJ(const std::string &filename, float *&verticesArray,
             float *&normalsArray, float *&texCoordsArray, size_t &numVertices);
