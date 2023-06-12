#include "Altar.h"
#include "CollidableObj.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>

#define ALTAR_SCALAR glm::vec3(0.1f, 0.05f, 0.1f)
#define ALTAR_OFFSET_Y -0.01f

void Altar::draw(const glm::mat4 &baseM, ShaderProgram *sp) {
  glm::mat4 M2 = glm::translate(baseM, m_position);
  glm::mat4 M3 = glm::scale(M2, ALTAR_SCALAR);

  glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M3));
  glEnableVertexAttribArray(sp->a("vertex"));
  glVertexAttribPointer(sp->a("vertex"), 4, GL_FLOAT, false, 0,
                        vertices.data());
  glEnableVertexAttribArray(sp->a("color"));
  glVertexAttribPointer(sp->a("color"), 4, GL_FLOAT, false, 0, colors.data());
  glEnableVertexAttribArray(sp->a("normal"));
  glVertexAttribPointer(sp->a("normal"), 4, GL_FLOAT, false, 0, normals.data());
  glEnableVertexAttribArray(sp->a("texCoord0"));
  glVertexAttribPointer(sp->a("texCoord0"), 2, GL_FLOAT, false, 0,
                        texCoords.data());

  glActiveTexture(GL_TEXTURE0);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);

  glBindTexture(GL_TEXTURE_2D, m_texture);
  glGenerateMipmap(GL_TEXTURE_2D);
  glUniform1i(sp->u("textureMap0"), 0);
  glDrawArrays(GL_TRIANGLES, 0, vertices.size());
  glDisableVertexAttribArray(sp->a("vertex"));
  glDisableVertexAttribArray(sp->a("color"));
  glDisableVertexAttribArray(sp->a("normal"));
  glDisableVertexAttribArray(sp->a("TexCoord0"));
}

bool Altar::hasColided(const glm::vec3 &other_pos) {
  // TODO: the bounding box has to be smaller
  // than the placing box
  return false;
}

bool Altar::can_place_skull(const glm::vec3 &player_pos) {
  glm::vec3 diff = m_position - player_pos;
  return diff.x >= -m_bounding_box_radius.x &&
         diff.x <= m_bounding_box_radius.x &&
         diff.y >= -m_bounding_box_radius.y &&
         diff.y <= m_bounding_box_radius.y &&
         diff.z >= -m_bounding_box_radius.z &&
         diff.z <= m_bounding_box_radius.z;

  std::cout << "can place skull" << std::endl;
  std::cout << hasColided(player_pos) << std::endl;
  return hasColided(player_pos);
}

Altar::Altar(int x_pos, int y_pos, AltarType type, AltarColor color)
    : CollidableObj(glm::vec3(x_pos, 0, y_pos), glm::vec3(0, 0, 0),
                    glm::vec3(1, 1, 1)) {
  this->m_position = glm::vec3(x_pos, ALTAR_OFFSET_Y, y_pos);
  this->type = type;
  this->color = color;
  this->x_pos = x_pos;
  this->y_pos = y_pos;

  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(
      "models/altar_old.obj",
      aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals);

  if (!scene->HasMeshes())
    throw std::runtime_error("No meshes in file");

  aiMesh *mesh = scene->mMeshes[0];

  for (int i = 0; i < mesh->mNumVertices; i++) {
    aiVector3D vertex = mesh->mVertices[i];
    vertices.push_back(glm::vec4(vertex.x, vertex.y, vertex.z, 1.0f));

    aiVector3D normal = mesh->mNormals[i];
    normals.push_back(glm::vec4(normal.x, normal.y, normal.z, 0.0f));

    aiColor4D color =
        mesh->HasVertexColors(0) ? mesh->mColors[0][i] : aiColor4D(1.0f);
    colors.push_back(glm::vec4(color.r, color.g, color.b, color.a));

    if (mesh->GetNumUVChannels() > 0 && mesh->mNumUVComponents[0] > 0) {
      aiVector3D texCoord = mesh->mTextureCoords[0][i];
      texCoords.push_back(glm::vec2(texCoord.x, texCoord.y));
    }
  }

  numVertices = vertices.size();
}
