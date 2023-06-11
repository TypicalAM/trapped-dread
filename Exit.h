#pragma once

#include "CollidableObj.h"
#include <glm/fwd.hpp>

class Exit : public CollidableObj {
private:
  int x_pos;
  int y_pos;

  float *verticesArray;
  float *normalsArray;
  float *texCoordsArray;
  float *colorsArray;
  size_t numVertices;

public:
  Exit(int x_pos, int y_pos);
  virtual ~Exit() {}
  virtual void draw(const glm::mat4 &baseM, ShaderProgram *sp) override;
  bool hasColided(const glm::vec3 &other_pos) override;
};
