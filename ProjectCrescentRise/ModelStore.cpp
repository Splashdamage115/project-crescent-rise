#include "ModelStore.h"

std::vector<meshStorePair> MeshStore::m_meshStore;

renderObject MeshStore::loadMesh(const ufbx_mesh* mesh_fbx, const ufbx_scene* scene) {
    renderObject mesh = {};
    std::vector<float> positions;
    std::vector<float> normals;
    std::vector<uint32_t> indices;

    for (size_t i = 0; i < mesh_fbx->num_vertices; i++) {
        ufbx_vec3 pos = mesh_fbx->vertices[i];
        ufbx_vec3 norm = mesh_fbx->vertex_normal[i];
        positions.push_back(pos.x);
        positions.push_back(pos.y);
        positions.push_back(pos.z);
        normals.push_back(norm.x);
        normals.push_back(norm.y);
        normals.push_back(norm.z);
    }

    for (size_t i = 0; i < mesh_fbx->num_indices; i++) {
        indices.push_back(mesh_fbx->vertex_indices[i]);
    }

    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);

    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float),
        positions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &mesh.normals);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.normals);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float),
        normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &mesh.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t),
        indices.data(), GL_STATIC_DRAW);

    mesh.indexLength = indices.size();

    glBindVertexArray(0);
    return mesh;
}

renderObject MeshStore::getMesh(std::string meshLocation)
{
    for (const auto& entry : m_meshStore) {
        if (entry.first == meshLocation) {
            return entry.second;
        }
    }

    ufbx_load_opts opts = {};
    ufbx_error error;
    ufbx_scene* scene = ufbx_load_file(meshLocation.c_str(), &opts, &error);

    if (!scene) {
        return renderObject();
    }

    if (scene->meshes.count == 0) {
        ufbx_free_scene(scene);
        return renderObject();
    }

    renderObject mesh = loadMesh(scene->meshes[0], scene);
    m_meshStore.push_back({ meshLocation, mesh });

    ufbx_free_scene(scene);
    return mesh;
}
