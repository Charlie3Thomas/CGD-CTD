#pragma once

#include <Eigen/Core>
#include <Eigen/Dense>
#include <assimp/scene.h>

aiMesh& operator *= (aiMesh& mesh, const Eigen::Matrix3f& matrix);
aiMesh& operator += (aiMesh& mesh, const Eigen::Vector3f& vector);


namespace CT
{
/// @brief Create a translation matrix for rotation
Eigen::Matrix3f MakeRotation(float x, float y, float z);
}