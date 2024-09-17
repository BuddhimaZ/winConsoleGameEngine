#include <wcge3dCamera.hpp>

inline wcge::r3d::Camera::Camera(
	uint32_t width, 
	uint32_t height, 
	float fov, 
	float nearPlane, 
	float farPlane,
	const Math::Vector3& position,
	const Math::Vector3& rotation
) : 
	m_uiWidth(width),
	m_uiHeight(height), 
	m_fFOV(fov), 
	m_fNearPlane(nearPlane), 
	m_fFarPlane(farPlane),
	m_vPosition(position),
	m_vRotation(rotation)
{
	UpdateProjectionMatrix();
}

wcge::r3d::Camera::Camera(
	uint32_t width, 
	uint32_t height, 
	float fov, 
	float nearPlane, 
	float farPlane
) : 
Camera::Camera(
	width, 
	height, 
	fov, 
	nearPlane, 
	farPlane, 
	Math::Vector3(), 
	Math::Vector3()
) {}

wcge::Math::Mat4x4 wcge::r3d::Camera::GetCameraViewMatrix() const {
	using namespace wcge::Math;
	return Mat4MakeTranslation(-m_vPosition) * Mat4MakeRotationYXZ(-m_vRotation) * m_mProjectionMatrix;
}

void wcge::r3d::Camera::MoveAlongZ(float dt) {
	using namespace wcge::Math;
	m_vPosition += VEC3_FORWARD * m_fMovementSpeed * dt * Mat3MakeRotationZXY(m_vRotation);
}

void wcge::r3d::Camera::MoveAlongX(float dt) {
	using namespace wcge::Math;
	m_vPosition += VEC3_RIGHT * m_fMovementSpeed * dt * Mat3MakeRotationZXY(m_vRotation);
}

void wcge::r3d::Camera::MoveAlongY(float dt) {
	using namespace wcge::Math;
	m_vPosition += VEC3_UP * m_fMovementSpeed * dt * Mat3MakeRotationZXY(m_vRotation);
}

void wcge::r3d::Camera::RotateAroundX(float dt) {
	m_vRotation.x += m_fRotationSpeed * dt;
}

void wcge::r3d::Camera::RotateAroundY(float dt) {
	m_vRotation.y += m_fRotationSpeed * dt;
}

void wcge::r3d::Camera::RotateAroundZ(float dt) {
	m_vRotation.z += m_fRotationSpeed * dt;
}

inline void wcge::r3d::Camera::UpdateProjectionMatrix() {
	using namespace wcge::Math;
	m_mProjectionMatrix = Mat4MakeProjectionMatrix(m_uiWidth, m_uiHeight, m_fFOV, m_fNearPlane, m_fFarPlane);
}
