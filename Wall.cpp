#include "Wall.h"
#include "CollidableObj.h"
#include "myCube.h"
#include <array>
#include <iostream>

#define WALL_SCALER glm::vec3(0.5f, 0.5f, 0.5f)

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

bool Wall::hasColided(const glm::vec3& other_pos)
{
    glm::vec3 diff = m_position - other_pos;
    return
        diff.x >= -m_bounding_box_radius.x && diff.x <= m_bounding_box_radius.x &&
        diff.y >= -m_bounding_box_radius.y && diff.y <= m_bounding_box_radius.y &&
        diff.z >= -m_bounding_box_radius.z && diff.z <= m_bounding_box_radius.z;
   

}

Wall::Wall(int x_pos, int y_pos, int layer)
    : CollidableObj(glm::vec3(x_pos, 0, y_pos), glm::vec3(0, 0, 0),
                    glm::vec3(1, 1, 1)) {
  this->x_pos = x_pos;
  this->y_pos = y_pos;
  this->layer = layer;
  this->m_position = glm::vec3(2 * x_pos, 1.02f + 2 * layer, y_pos * 2);
}
