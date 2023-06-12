#pragma once

#include "CollidableObj.h"
#include <assimp/vector3.h>
#include <glm/fwd.hpp>
#include <vector>

class Exit : public CollidableObj {
private:
  int x_pos;
  int y_pos;

  std::vector<glm::vec4> vertices;
  std::vector<glm::vec4> colors;
  std::vector<glm::vec4> normals;
  std::vector<glm::vec2> texCoords;
  unsigned int numVertices;

public:
  Exit(int x_pos, int y_pos);
  virtual ~Exit() {}
  virtual void draw(const glm::mat4 &baseM, ShaderProgram *sp) override;
  bool hasColided(const glm::vec3 &other_pos) override;
};
