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
  glEnableVertexAttribArray(sp->a("color"));
  glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colorsArray);
  glDrawArrays(GL_TRIANGLES, 0, numVertices);
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

Altar::Altar(int x_pos, int y_pos, AltarType type, AltarColor color)
    : CollidableObj(glm::vec3(x_pos, 0, y_pos), glm::vec3(0, 0, 0),
                    glm::vec3(1, 1, 1)) {
  this->m_position = glm::vec3(x_pos + -0.1f, ALTAR_OFFSET_Y, y_pos);
  this->type = type;
  this->color = color;

  // Load the object from file, no error handling because we HATE IT
  loadOBJ("models/table.obj", verticesArray, normalsArray, texCoordsArray,
          numVertices);

  colorsArray = new float[numVertices * 4];
  for (int i = 0; i < numVertices; i++) {
    colorsArray[i * 4] = color == AltarColor::RED_ALTAR ? 1.0f : 0.0f;
    colorsArray[i * 4 + 1] = color == AltarColor::RED_ALTAR ? 0.0f : 0.0f;
    colorsArray[i * 4 + 2] = color == AltarColor::RED_ALTAR ? 0.0f : 1.0f;
    colorsArray[i * 4 + 3] = 1.0f;
  }

  std::cout << "Loaded object, size is " << numVertices << std::endl;
}
