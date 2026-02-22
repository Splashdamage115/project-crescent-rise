#include "ModelStore.h"

std::vector<meshStorePair> MeshStore::m_meshStore;

renderObject MeshStore::loadMesh(const ufbx_mesh* mesh_fbx, const ufbx_scene* scene) {
    renderObject mesh = {};
    std::vector<float> positions;
    std::vector<float> normals;
    std::vector<float> materialIDs;
    std::vector<uint32_t> indices;

    uint32_t vertexOffset = 0;
    for (size_t mi = 0; mi < scene->meshes.count; mi++)
    {
        const ufbx_mesh* m = scene->meshes[mi];

        // Default every vertex in this mesh to the mesh index
        std::vector<float> meshMatIDs(m->num_vertices, static_cast<float>(mi));

        // If the mesh has per-face material assignments, use those instead
        if (m->face_material.count > 0)
        {
            for (size_t fi = 0; fi < m->num_faces; fi++)
            {
                ufbx_face face = m->faces[fi];
                float matID = static_cast<float>(m->face_material[fi]);
                for (uint32_t vi = 0; vi < face.num_indices; vi++)
                {
                    uint32_t vertIdx = m->vertex_indices[face.index_begin + vi];
                    meshMatIDs[vertIdx] = matID;
                }
            }
        }

        for (size_t i = 0; i < m->num_vertices; i++) {
            ufbx_vec3 pos  = m->vertices[i];
            ufbx_vec3 norm = m->vertex_normal[i];
            positions.push_back(pos.x);  positions.push_back(pos.y);  positions.push_back(pos.z);
            normals.push_back(norm.x);   normals.push_back(norm.y);   normals.push_back(norm.z);
            materialIDs.push_back(meshMatIDs[i]);
        }

        for (size_t i = 0; i < m->num_indices; i++)
            indices.push_back(m->vertex_indices[i] + vertexOffset);

        vertexOffset += static_cast<uint32_t>(m->num_vertices);
    }

    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);

    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &mesh.normals);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.normals);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);

    GLuint matVBO;
    glGenBuffers(1, &matVBO);
    glBindBuffer(GL_ARRAY_BUFFER, matVBO);
    glBufferData(GL_ARRAY_BUFFER, materialIDs.size() * sizeof(float), materialIDs.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(3);

    glGenBuffers(1, &mesh.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);

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
