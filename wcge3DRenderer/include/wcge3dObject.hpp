#pragma once

#include "wcge3dCamera.hpp"

#include <wcge3dPipeline.hpp>
#include <Math/wcgeMath.hpp>

namespace wcge {
	namespace r3d {
		enum ObjectConstantBufferPosition : size_t {
			VIEW_MATRIX = 0,
			ROTATION_MATRIX = 1
		};

		const c3d::VertexBuffer CreateCubeVertexBuffer(float size);
		const c3d::IndexBuffer CreateCubeIndexBuffer();

		class Object {
		public:
			Object(c3d::Pipeline* pPipeline, const c3d::VertexBuffer& vertexBuffer, const c3d::IndexBuffer& indexBuffer, const std::wstring& texturePath, const bool bDitheringEnabled = true);
			~Object();

			static Object* LoadFromFile(c3d::Pipeline* pPipeline, const std::string& path, const std::wstring& texturePath, const bool bDitheringEnabled = true);
			static void Delete(Object* pObject);

			inline Math::Vector3& GetPosition() { return m_vPosition; }
			inline Math::Vector3& GetRotation() { return m_vRotation; }
			inline Math::Vector3& GetScale() { return m_vScale; }

			inline const Math::Vector3& GetPosition() const { return m_vPosition; }
			inline const Math::Vector3& GetRotation() const { return m_vRotation; }
			inline const Math::Vector3& GetScale() const { return m_vScale; }

			inline void SetCamera(Camera* pCamera) { m_pCamera = pCamera; }

			void Draw();

		private:
			c3d::Pipeline* m_pPipeline;
			const Camera* m_pCamera;

			Math::Vector3 m_vPosition;
			Math::Vector3 m_vRotation;
			Math::Vector3 m_vScale;

			size_t m_nVertexArrayId;
			size_t m_nVertexShaderId;
			size_t m_nPixelShaderId;
			size_t m_nTextureId;
			size_t m_nViewMatrixId;
			size_t m_nRotationMatrixId;
		};
	}
}
