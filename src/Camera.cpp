#include "Camera.h"

Camera::Camera()
	: m_position(0.0f, 0.0f, 0.0f), m_rotation(0.0f, 1.0f, 0.0f, 0.0f)
{
}

void Camera::ApplyTransformation()
{
    view_matrix.SetIdentity();
    view_matrix.RotateMatrix(m_rotation.GetEulerAngles());
    view_matrix.TranslateMatrix(m_position);
	//m_rotation.MulMatrix();
	//glTranslatef(-m_position.x, -m_position.y, -m_position.z);
}

void Camera::GetViewMatrix(Matrix4x4f &viewMatrix)
{
	viewMatrix = m_rotation.GetMatrix() * Matrix4x4f::TranslateMatrix(-m_position);
}
