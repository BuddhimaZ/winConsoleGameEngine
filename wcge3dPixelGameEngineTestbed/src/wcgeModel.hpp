#pragma once

#include <wcge3dVertex.hpp>
#include <wcge3dVertexArray.hpp>
#include <wcge3dCamera.hpp>

#include "wcgePixelPipeline.hpp"

namespace wcge {
	namespace r3d {
		enum ObjectConstantBufferPosition : size_t {
			VIEW_MATRIX = 0,
			ROTATION_MATRIX = 1
		};

		const c3d::VertexBuffer CreateCubeVertexBuffer(float size);
		const c3d::IndexBuffer CreateCubeIndexBuffer();

		class Model {
		public:
			Model(c3d::PixelPipeline* pPipeline, const c3d::VertexBuffer& vertexBuffer, const c3d::IndexBuffer& indexBuffer, const std::wstring& texturePath);
			~Model();

			static Model* LoadFromFile(c3d::PixelPipeline* pPipeline, const std::string& path, const std::wstring& texturePath);
			static void Delete(Model* pObject);

			inline Math::Vector3& GetPosition() { return m_vPosition; }
			inline Math::Vector3& GetRotation() { return m_vRotation; }
			inline Math::Vector3& GetScale() { return m_vScale; }

			inline const Math::Vector3& GetPosition() const { return m_vPosition; }
			inline const Math::Vector3& GetRotation() const { return m_vRotation; }
			inline const Math::Vector3& GetScale() const { return m_vScale; }

			inline void SetCamera(Camera* pCamera) { m_pCamera = pCamera; }

			void Draw();

		private:
			c3d::PixelPipeline* m_pPipeline;
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