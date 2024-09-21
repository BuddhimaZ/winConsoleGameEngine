#pragma once

#include <wcgeMath.hpp>

namespace wcge {
	namespace r3d {
		class Camera {
		public:
			Camera(uint32_t width, uint32_t height, float fov, float nearPlane, float farPlane, const Math::Vector3& position, const Math::Vector3& rotation);
			Camera(uint32_t width, uint32_t height, float fov, float nearPlane, float farPlane);


			Math::Mat4x4 GetCameraViewMatrix() const;

			inline const Math::Vector3& GetPosition() const { return m_vPosition; }
			inline const Math::Vector3& GetRotation() const { return m_vRotation; }
			inline const Math::Mat4x4& GetProjectionMatrix() const { return m_mProjectionMatrix; }
			inline uint32_t GetWidth() const { return m_uiWidth; }
			inline uint32_t GetHeight() const { return m_uiHeight; }
			inline float GetFOV() const { return m_fFOV; }
			inline float GetNearPlane() const { return m_fNearPlane; }
			inline float GetFarPlane() const { return m_fFarPlane; }

			inline void SetPosition(const Math::Vector3& position) { m_vPosition = position; UpdateProjectionMatrix(); }
			inline void SetRotation(const Math::Vector3& rotation) { m_vRotation = rotation; UpdateProjectionMatrix(); }
			inline void SetWidth(uint32_t width) { m_uiWidth = width; UpdateProjectionMatrix(); }
			inline void SetHeight(uint32_t height) { m_uiHeight = height; UpdateProjectionMatrix(); }
			inline void SetFOV(float fov) { m_fFOV = fov; UpdateProjectionMatrix(); }
			inline void SetNearPlane(float nearPlane) { m_fNearPlane = nearPlane; UpdateProjectionMatrix(); }
			inline void SetFarPlane(float farPlane) { m_fFarPlane = farPlane; UpdateProjectionMatrix(); }

			inline void SetMovementSpeed(float speed) { m_fMovementSpeed = speed; }
			inline void SetRotationSpeed(float speed) { m_fRotationSpeed = speed; }


			void MoveAlongZ(float dt);
			void MoveAlongX(float dt);
			void MoveAlongY(float dt);

			void RotateAroundX(float dt);
			void RotateAroundY(float dt);
			void RotateAroundZ(float dt);

		private:
			void UpdateProjectionMatrix();

		private:
			Math::Mat4x4 m_mProjectionMatrix;
			Math::Vector3 m_vPosition;
			Math::Vector3 m_vRotation;
			
			uint32_t m_uiWidth;
			uint32_t m_uiHeight;
			float m_fFOV;
			float m_fNearPlane;
			float m_fFarPlane;

			float m_fMovementSpeed = 1.0f;
			float m_fRotationSpeed = 10.0f;
		};
	}
}