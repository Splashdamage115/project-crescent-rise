#include "ModelStore.h"

std::vector<meshStorePair> MeshStore::m_meshStore;

renderObject MeshStore::loadMesh(const ufbx_mesh* mesh_fbx, const ufbx_scene* scene) {
    renderObject mesh = {};
    std::vector<float> positions;
    std::vector<float> normals;
    std::vector<float> uvs;
    std::vector<float> materialIDs;
    std::vector<uint32_t> indices;

    for (size_t mi = 0; mi < scene->meshes.count; mi++)
    {
        const ufbx_mesh* m = scene->meshes[mi];

        // Iterate per-face and per-corner so each triangle corner gets its own
        // UV, normal, and materialID directly from the FBX corner data.
        // This correctly handles UV seams where the same vertex has different
        // UV coordinates in different faces.
        for (size_t fi = 0; fi < m->num_faces; fi++)
        {
            ufbx_face face = m->faces[fi];
            float matID = (m->face_material.count > 0)
                ? static_cast<float>(m->face_material[fi])
                : static_cast<float>(mi);

            // Fan triangulation: works for triangles, quads, and n-gons
            for (uint32_t ti = 1; ti + 1 < face.num_indices; ti++)
            {
                uint32_t corners[3] = {
                    face.index_begin,
                    face.index_begin + ti,
                    face.index_begin + ti + 1
                };

                for (int ci = 0; ci < 3; ci++)
                {
                    uint32_t corner = corners[ci];
                    uint32_t vi = m->vertex_indices[corner];

                    ufbx_vec3 pos = m->vertices[vi];
                    positions.push_back((float)pos.x);
                    positions.push_back((float)pos.y);
                    positions.push_back((float)pos.z);

                    ufbx_vec2 uv = {};
                    if (m->vertex_uv.exists)
                        uv = m->vertex_uv[corner];
                    uvs.push_back((float)uv.x);
                    uvs.push_back(1.0f - (float)uv.y); // flip V: Blender (V=0 bottom) -> OpenGL (V=0 top)

                    ufbx_vec3 norm = {};
                    if (m->vertex_normal.exists)
                        norm = m->vertex_normal[corner];
                    normals.push_back((float)norm.x);
                    normals.push_back((float)norm.y);
                    normals.push_back((float)norm.z);

                    materialIDs.push_back(matID);
                    indices.push_back((uint32_t)indices.size());
                }
            }
        }
    }

    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);

    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);

    glGenBuffers(1, &mesh.uvs);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.uvs);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);

    glGenBuffers(1, &mesh.normals);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.normals);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(2);

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
