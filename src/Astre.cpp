#include "Astronomical.h"


Astronomical::Astronomical(float* vertices, int nbVertices, float* normals, glm::vec3 position, float* uvs) {
    m_vertices = vertices;
    m_normals = normals;
    m_uvs = uvs;
    m_nbVertices = nbVertices;
}

void Astronomical::draw(Camera camera) {

}