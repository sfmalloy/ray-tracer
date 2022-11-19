#include "mesh.hpp"
#include "bvh.hpp"
#include "utils.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/vec3.hpp>
#include <fmt/core.h>
#include <memory>

mesh::mesh()
{ }

mesh::mesh(const std::string& filename, std::shared_ptr<material> mat)
  : mesh(filename, mat, glm::vec3{0,0,0})
{ }

mesh::mesh(const std::string& filename, std::shared_ptr<material> mat, const glm::vec3& offset) {
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
            vertices.emplace_back(mesh->mVertices[i].x + offset.x,
                                  mesh->mVertices[i].y + offset.y,
                                  mesh->mVertices[i].z + offset.z);
        }

        for (u32 i = 0; i < mesh->mNumFaces; ++i) {
            auto face = mesh->mFaces[i];
            m_faces.add(std::make_shared<triangle>(vertices[face.mIndices[0]], 
                vertices[face.mIndices[1]], vertices[face.mIndices[2]], mat));
        }

        fmt::print("Loaded {} triangles\n", mesh->mNumFaces);
    }

    m_bvh = std::make_shared<bvh_node>(m_faces, 0, 1);
}

bool mesh::hit(const ray& r, f32 t_min, f32 t_max, hit_record& rec) const {
    return m_bvh->hit(r, t_min, t_max, rec);
}

bool mesh::bounding_box(f32 t0, f32 t1, aabb& output_box) const {    
    return m_bvh->bounding_box(t0, t1, output_box);
}
