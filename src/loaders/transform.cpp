#include "transform.hpp"
#include <numbers>

aiMesh& operator *= (aiMesh& mesh, const Eigen::Matrix3f& matrix)
{
    for (size_t i = 0; i < mesh.mNumVertices; i++)
    {
        Eigen::Vector3f vertex(mesh.mVertices[i].x, mesh.mVertices[i].y, mesh.mVertices[i].z);
        vertex = matrix * vertex;
        mesh.mVertices[i].x = vertex.x();
        mesh.mVertices[i].y = vertex.y();
        mesh.mVertices[i].z = vertex.z();
    }

    return mesh;
}

aiMesh& operator += (aiMesh& mesh, const Eigen::Vector3f& vector)
{
    for (size_t i = 0; i < mesh.mNumVertices; i++)
    {
        mesh.mVertices[i].x += vector.x();
        mesh.mVertices[i].y += vector.y();
        mesh.mVertices[i].z += vector.z();
    }

    return mesh;
}


namespace CT
{
Eigen::Matrix3f MakeRotation(float x, float y, float z)
{
    return 
        Eigen::AngleAxisf(x * std::numbers::pi_v<float> / 180.0F, Eigen::Vector3f::UnitX()).matrix().cast<float>() *
        Eigen::AngleAxisf(y * std::numbers::pi_v<float> / 180.0F, Eigen::Vector3f::UnitY()).matrix().cast<float>() *
        Eigen::AngleAxisf(z * std::numbers::pi_v<float> / 180.0F, Eigen::Vector3f::UnitZ()).matrix().cast<float>();
}
}