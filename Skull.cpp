#include "Skull.h"
#include "load_obj.h"

#define SKULL_SCALAR glm::vec3(0.2f, 0.2f, 0.2f)
#define SKULL_OFFSET_Y 0.44F

void Skull::draw(const glm::mat4 &baseM, ShaderProgram *sp) {
  glm::mat4 M2 = glm::translate(baseM, m_position);
  glm::mat4 M3 = glm::scale(M2, SKULL_SCALAR);

  glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M3));
  glEnableVertexAttribArray(sp->a("vertex"));
  glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, verticesArray);
  glDrawArrays(GL_TRIANGLES, 0, numVertices);
  glDisableVertexAttribArray(sp->a("vertex"));
}

bool Skull::hasColided(const glm::vec3 &other_pos) { return false; }

Skull ::Skull(int x_pos, int y_pos, SkullColor color)
    : CollidableObj(glm::vec3(x_pos, 0, y_pos), glm::vec3(0, 0, 0),
                    glm::vec3(1, 1, 1)) {
  this->x_pos = x_pos;
  this->y_pos = y_pos;
  this->m_position = glm::vec3(x_pos, SKULL_OFFSET_Y, y_pos);
  this->color = color;

  // Load the object from file, no error handling because we HATE IT
  // TODO: Add coloring logic
  loadOBJ("models/skull.obj", verticesArray, normalsArray, texCoordsArray,
          numVertices);

  std::cout << "Loaded object, size is " << numVertices << std::endl;
}
