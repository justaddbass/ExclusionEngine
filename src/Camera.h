#ifndef EE_CAMERA
#define EE_CAMERA

#include "utils/Quaternion.h"
#include "utils/Vec3f.h"

class Camera {
public:
	Camera();
	Vec3f m_position;
	Quaternion m_rotation;
	void ApplyTransformation();
	void GetViewMatrix(Matrix4x4f& viewMatrix);
	Matrix4x4f GetViewMatrix() {return view_matrix;}
private:
    Matrix4x4f view_matrix;
};

#endif
