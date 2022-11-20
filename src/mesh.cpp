#include "mesh.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/vec3.hpp>
#include <fmt/core.h>
#include <memory>

mesh::mesh()
{ }

mesh::mesh(const std::string& filename, std::shared_ptr<material> mat, f32 scale)
  : mesh(filename, mat, glm::vec3{0.0f,0.0f,0.0f}, scale)
{ }

mesh::mesh(const std::string& filename, std::shared_ptr<material> mat, const glm::vec3& offset, f32 scale) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename.c_str(),
        aiProcess_JoinIdenticalVertices | aiProcess_Triangulate);
    if (scene == nullptr) {
        fmt::print("Failed to load object {}. ({})", filename, importer.GetErrorString());
        return;
    }

    for (u32 m = 0; m < scene->mNumMeshes; ++m) {
        auto mesh = scene->mMeshes[0];
        std::vector<glm::vec3> vertices;
        vertices.reserve(mesh->mNumVertices * 3);
        std::vector<glm::vec3> normals;
        for (u32 i = 0; i < mesh->mNumVertices; ++i) {
            vertices.emplace_back(scale * (mesh->mVertices[i].x + offset.x),
                                  scale * (mesh->mVertices[i].y + offset.y),
                                  scale * (mesh->mVertices[i].z + offset.z));
        }

        for (u32 i = 0; i < mesh->mNumFaces; ++i) {
            auto face = mesh->mFaces[i];
            m_faces.add(std::make_shared<triangle>(vertices[face.mIndices[0]], 
                vertices[face.mIndices[1]], vertices[face.mIndices[2]], mat));
        }

        fmt::print("Loaded {} triangles\n", mesh->mNumFaces);
    }
}

bool mesh::hit(const ray& r, f32 t_min, f32 t_max, hit_record& rec) const {
    return m_faces.hit(r, t_min, t_max, rec);
}
