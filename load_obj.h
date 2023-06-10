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
             float *&normalsArray, float *&texCoordsArray,
             size_t &numVertices) {

  std::ifstream file(filename);
  std::string line;
  std::vector<Point3D> vertices;
  std::vector<Normal3D> normals;
  std::vector<TexCoord2D> texCoords;
  std::vector<Face> faces;

  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << filename << std::endl;
    return;
  }

  std::string token;
  while (std::getline(file, line)) {
    std::istringstream iss(line);
    iss >> token;

    if (token == "v") {
      Point3D vertex;
      iss >> vertex.x >> vertex.y >> vertex.z;
      vertex.w = 1.0f; // Homogeneous coordinate
      vertices.push_back(vertex);
    } else if (token == "vn") {
      Normal3D normal;
      iss >> normal.x >> normal.y >> normal.z;
      normals.push_back(normal);
    } else if (token == "vt") {
      TexCoord2D texCoord;
      iss >> texCoord.u >> texCoord.v;
      texCoords.push_back(texCoord);
    } else if (token == "f") {
      Face face;
      std::string vertexIndexStr;
      while (iss >> vertexIndexStr) {
        std::istringstream indexIss(vertexIndexStr);
        int vertexIndex, texCoordIndex, normalIndex;
        char delimiter;
        indexIss >> vertexIndex >> delimiter >> texCoordIndex >> delimiter >>
            normalIndex;
        face.vertexIndices.push_back(vertexIndex - 1);
        face.texCoordIndices.push_back(texCoordIndex - 1);
        face.normalIndices.push_back(normalIndex - 1);
      }
      faces.push_back(face);
    }
  }

  file.close();

  // Create separate vectors for vertices, normals, and texture coordinates
  std::vector<float> vertexArray;
  std::vector<float> normalArray;
  std::vector<float> texCoordArray;
  for (const auto &face : faces) {
    const int numFaceVertices = face.vertexIndices.size();
    if (numFaceVertices < 3) {
      continue; // Skip degenerate faces
    }

    for (int i = 0; i < numFaceVertices; ++i) {
      const int vertexIndex = face.vertexIndices[i];
      const int texCoordIndex = face.texCoordIndices[i];
      const int normalIndex = face.normalIndices[i];
      const Point3D &vertex = vertices[vertexIndex];
      const TexCoord2D &texCoord = texCoords[texCoordIndex];
      const Normal3D &normal = normals[normalIndex];
      std::cout << "tutaj sie wywalam" << std::endl;
      vertexArray.push_back(vertex.x);
      vertexArray.push_back(vertex.y);
      vertexArray.push_back(vertex.z);
      vertexArray.push_back(vertex.w);
      texCoordArray.push_back(texCoord.u);
      texCoordArray.push_back(texCoord.v);
      normalArray.push_back(normal.x);
      normalArray.push_back(normal.y);
      normalArray.push_back(normal.z);
      std::cout << "tutja sie wywalam 2?" << std::endl;
    }
  }

  // Copy vectors to arrays
  numVertices =
      vertexArray.size() / 4; // Each vertex has 4 components (x, y, z, w)

  verticesArray = new float[vertexArray.size()];
  normalsArray = new float[normalArray.size()];
  texCoordsArray = new float[texCoordArray.size()];

  std::copy(vertexArray.begin(), vertexArray.end(), verticesArray);
  std::copy(normalArray.begin(), normalArray.end(), normalsArray);
  std::copy(texCoordArray.begin(), texCoordArray.end(), texCoordsArray);
}
