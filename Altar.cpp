#include "Altar.h"
#include "CollidableObj.h"
#include "myCube.h"

#define WALL_SCALER glm::vec3(0.5f, 0.5f, 0.5f)
#define CUBE_OFSET_Y 0.51F

void Altar::draw(const glm::mat4 &baseM, ShaderProgram *sp) {
  glm::mat4 M2 = glm::translate(baseM, m_position);
  glm::mat4 M3 = glm::scale(M2, WALL_SCALER);

  glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M3));
  glEnableVertexAttribArray(sp->a("vertex"));
  glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, myCubeVertices);
  glEnableVertexAttribArray(sp->a("color"));
  glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, myCubeColors);
  glDrawArrays(GL_TRIANGLES, 0, myCubeVertexCount);
  glDisableVertexAttribArray(sp->a("vertex"));
  glDisableVertexAttribArray(sp->a("color"));
}

bool Altar::hasColided(const glm::vec3 &other_pos) {
  glm::vec3 diff = m_position - other_pos;
  return diff.x >= -m_bounding_box_radius.x &&
         diff.x <= m_bounding_box_radius.x &&
         diff.y >= -m_bounding_box_radius.y &&
         diff.y <= m_bounding_box_radius.y &&
         diff.z >= -m_bounding_box_radius.z &&
         diff.z <= m_bounding_box_radius.z;
}

Altar ::Altar(int x_pos, int y_pos, AltarType type)
    : CollidableObj(glm::vec3(x_pos, 0, y_pos), glm::vec3(0, 0, 0),
                    glm::vec3(1, 1, 1)) {
  this->x_pos = x_pos;
  this->y_pos = y_pos;
  this->m_position = glm::vec3(x_pos, 0, y_pos);
  this->type = type;
}
