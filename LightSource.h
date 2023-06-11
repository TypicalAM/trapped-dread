#pragma once

#include "common.h"

class PointLightSource {
public:
  glm::vec3 position;
  float intensity;

  PointLightSource(glm::vec3 init_pos, float intensity)
      : position(init_pos), intensity(intensity) {}
};

class ConeLightSource {
public:
  glm::vec3 position;
  glm::vec3 downVec;
  float phi;
  float theta;
  float intensity;

  ConeLightSource(glm::vec3 pos, glm::vec3 down, float phi, float theta,
                  float intensity)
      : position(pos), downVec(down), phi(phi), theta(theta),
        intensity(intensity) {}
};