#include "Altar.h"
#include "CollidableObj.h"
#include "load_obj.h"
#include "myCube.h"
#include <iostream>

#define ALTAR_SCALAR glm::vec3(0.1f, 0.1f, 0.1f)
#define ALTAR_OFFSET_Y -0.2F

void Altar::draw(const glm::mat4 &baseM, ShaderProgram *sp) {
  glm::mat4 M2 = glm::translate(baseM, m_position);
  glm::mat4 M3 = glm::scale(M2, ALTAR_SCALAR);

  glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M3));
  glEnableVertexAttribArray(sp->a("vertex"));
  glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, verticesArray);
  glDrawArrays(GL_TRIANGLES, 0, numVertices);
  glDisableVertexAttribArray(sp->a("vertex"));
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

Altar::Altar(int x_pos, int y_pos, AltarType type, AltarColor color)
    : CollidableObj(glm::vec3(x_pos, 0, y_pos), glm::vec3(0, 0, 0),
                    glm::vec3(1, 1, 1)) {
  this->m_position = glm::vec3(x_pos + -0.1f, ALTAR_OFFSET_Y, y_pos);
  this->type = type;
  this->color = color;

  // Load the object from file, no error handling because we HATE IT
  // TODO: Add coloring logic
  loadOBJ("models/table.obj", verticesArray, normalsArray, texCoordsArray,
          numVertices);

  std::cout << "Loaded object, size is " << numVertices << std::endl;
}
