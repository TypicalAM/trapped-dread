#pragma once

#include "CollidableObj.h"
#include <glm/fwd.hpp>

enum AltarType { START, END };
enum AltarColor { BLUE_ALTAR, RED_ALTAR };

class Altar : public CollidableObj {
private:
  AltarType type;
  AltarColor color;

  float *verticesArray;
  float *normalsArray;
  float *texCoordsArray;
  float *colorsArray;
  size_t numVertices;

public:
  Altar(int x_pos, int y_pos, AltarType type, AltarColor color);
  virtual ~Altar() {}
  virtual void draw(const glm::mat4 &baseM, ShaderProgram *sp) override;
  bool hasColided(const glm::vec3 &other_pos) override;
};
