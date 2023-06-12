#include "Floor.h"
#include "CollidableObj.h"
#include "myCube.h"
#include <array>
#include <iostream>
#include <vector>

void Floor::draw(const glm::mat4 &baseM, ShaderProgram *sp) {
  glm::mat4 M2{baseM};

  M2 = glm::scale(M2, glm::vec3(0.5f, 0.01f, 0.5f));

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < length; j++) {
      glm::mat4 M3 = glm::translate(M2, glm::vec3(2 * i, 0, 2 * j));

      // rotate so it gets diffrent color

      glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M3));

      glEnableVertexAttribArray(
          sp->a("vertex")); // W��cz przesy�anie danych do atrybutu vertex
      glVertexAttribPointer(
          sp->a("vertex"), 4, GL_FLOAT, false, 0,
          myCubeVertices); // Wska� tablic� z danymi dla atrybutu vertex

      glEnableVertexAttribArray(sp->a("color"));
      glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0,
                            myCubeColors); // za�aduj kolory

      glEnableVertexAttribArray(sp->a("normal"));
      glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0,
                            myCubeNormals);

      glEnableVertexAttribArray(sp->a("texCoord0"));
      glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0,
                            myCubeTexCoordsScaled);

      glActiveTexture(GL_TEXTURE0);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                      GL_LINEAR_MIPMAP_LINEAR);

      glBindTexture(GL_TEXTURE_2D, m_texture);
      glGenerateMipmap(GL_TEXTURE_2D);

      glUniform1i(sp->u("textureMap0"), 0);

      glDrawArrays(GL_TRIANGLES, 0, myCubeVertexCount); // Narysuj obiekt

      glDisableVertexAttribArray(
          sp->a("vertex")); // Wy��cz przesy�anie danych do atrybutu vertex
      glDisableVertexAttribArray(
          sp->a("color")); // Wy��cz przesy�anie danych do atrybutu vertex

      glDisableVertexAttribArray(
          sp->a("normal")); // Wy��cz przesy�anie danych do atrybutu vertex

      glDisableVertexAttribArray(
          sp->a("TexCoord0")); // Wy��cz przesy�anie danych do atrybutu vertex
    }
  }
}

bool Floor::hasColided(const glm::vec3 &other_pos) {
  float diff = other_pos.y - m_position.y;
  if (diff < 0) {
    return true;
  }
  return false;
}

glm::vec3 Floor::modify_cam_pos(const glm::vec3 &old_cam_pos,
                                const glm::vec3 &new_cam_pos) {
  // set y on NEW position to floor level
  auto pos = new_cam_pos;
  pos.y = m_position.y;
  return pos;
}

Floor::Floor(float floor_level, int width, int length)
    : CollidableObj(glm::vec3(0, floor_level, 0), glm::vec3(0, 0, 0),
                    glm::vec3(0, 0, 0)) {
  this->width = width;
  this->length = length;
}
