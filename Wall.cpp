#include "Wall.h"
#include "CollidableObj.h"
#include "myCube.h"
#include <array>
#include <iostream>

#define WALL_SCALER glm::vec3(0.5f, 0.5f, 0.5f)

Collisions Wall::calc_colision(const glm::vec3 &other_pos) {
  // Let's check if the other object is inside the horizontal bounds of the
  // wall, then we will check if the person is standing on the block or just
  // hitting it

  float x_min = x_pos - 0.5f;
  float x_max = x_pos + 0.5f;
  float y_floor = m_position.y + 0.4f;
  float y_ceiling = 0.4f;
  float z_min = y_pos - 0.5f;
  float z_max = y_pos + 0.5f;

  std::cout << "y_floor: " << y_floor << std::endl;
  std::cout << "y_ceiling: " << y_ceiling << std::endl;
  std::cout << "other_pos.y: " << other_pos.y << std::endl;

  return Collisions{
      std::pair<bool, bool>{other_pos.x >= x_min, other_pos.x <= x_max},
      std::pair<bool, bool>{other_pos.y >= y_floor, other_pos.y <= y_ceiling},
      std::pair<bool, bool>{other_pos.z >= z_min, other_pos.z <= z_max}};
}

void Wall::draw(const glm::mat4 &baseM, ShaderProgram *sp) {
  glm::mat4 M2 = glm::scale(baseM, WALL_SCALER);
  glm::mat4 M3 = glm::translate(M2, m_position);

  glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M3));
  glEnableVertexAttribArray(sp->a("vertex"));
  glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, myCubeVertices);
  glEnableVertexAttribArray(sp->a("color"));
  glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, myCubeColors);
  glDrawArrays(GL_TRIANGLES, 0, myCubeVertexCount);
  glDisableVertexAttribArray(sp->a("vertex"));
  glDisableVertexAttribArray(sp->a("color"));
}

Wall::Wall(int x_pos, int y_pos, int layer)
    : CollidableObj(glm::vec3(x_pos, 0, y_pos), glm::vec3(0, 0, 0),
                    glm::vec3(2, 2, 2)) {
  this->x_pos = x_pos;
  this->y_pos = y_pos;
  this->layer = layer;
  this->m_position = glm::vec3(2 * x_pos, 1.02f + 2 * layer, y_pos * 2);
}
