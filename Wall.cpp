#include "Wall.h"
#include "CollidableObj.h"
#include "myCube.h"
#include <array>
#include <iostream>

#define WALL_SCALER glm::vec3(0.5f, 0.5f, 0.5f)
#define CUBE_OFSET_Y 0.51F

void Wall::draw(const glm::mat4 &baseM, ShaderProgram *sp) {
  glm::mat4 M2 = glm::translate(baseM, m_position);
  glm::mat4 M3 = glm::scale(M2, WALL_SCALER);

  glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M3));
  glEnableVertexAttribArray(sp->a("vertex"));
  glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0, myCubeVertices);
  glEnableVertexAttribArray(sp->a("color"));
  glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, myCubeColors);
  glEnableVertexAttribArray(sp->a("normal"));
  glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, myCubeNormals);
  glEnableVertexAttribArray(sp->a("texCoord0"));
  glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0,
                        myCubeTexCoords);

  glActiveTexture(GL_TEXTURE0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);

  glBindTexture(GL_TEXTURE_2D, m_texture);
  glGenerateMipmap(GL_TEXTURE_2D);
  glUniform1i(sp->u("textureMap0"), 0);
  glDrawArrays(GL_TRIANGLES, 0, myCubeVertexCount);
  glDisableVertexAttribArray(sp->a("vertex"));
  glDisableVertexAttribArray(sp->a("color"));
  glDisableVertexAttribArray(sp->a("normal"));
  glDisableVertexAttribArray(sp->a("TexCoord0"));
}

bool Wall::hasColided(const glm::vec3 &other_pos) {
  glm::vec3 diff = m_position - other_pos;
  std::cout << "start of hasColided" << std::endl;
  std::cout << "diff: " << diff.x << " " << diff.y << " " << diff.z
            << std::endl;
  std::cout << "m_bounding_box_radius: " << m_bounding_box_radius.x << " "
            << m_bounding_box_radius.y << " " << m_bounding_box_radius.z
            << std::endl;

  // Wall has a bounding box from -0.5 to 0.5 in all directions
  return diff.x >= -m_bounding_box_radius.x &&
         diff.x <= m_bounding_box_radius.x &&
         diff.y >= -m_bounding_box_radius.y &&
         diff.y <= m_bounding_box_radius.y &&
         diff.z >= -m_bounding_box_radius.z &&
         diff.z <= m_bounding_box_radius.z;
}

Wall::Wall(int x_pos, int y_pos, int layer)
    : CollidableObj(glm::vec3(x_pos, 0, y_pos), glm::vec3(0, 0, 0),
                    glm::vec3(0.7, 1, 0.7)) {
  this->x_pos = x_pos;
  this->y_pos = y_pos;
  this->layer = layer;
  this->m_position = glm::vec3(x_pos, layer + CUBE_OFSET_Y, y_pos);
}
