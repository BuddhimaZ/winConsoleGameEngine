#pragma once

#include "wcge3dVertex.hpp"
#include "wcge3dVertexArray.hpp"
#include "wcge3dConstantBuffer.hpp"
#include "wcge3dPipelineCoreDescriptor.hpp"

#include <string>

namespace wcge {
	namespace c3d {

		class PipelineCore {
		public:
			PipelineCore(PipelineCoreDescriptor descriptor);
			virtual ~PipelineCore();

			size_t CreateVertexArray(const VertexBuffer& vVertexBuffer, const IndexBuffer& vIndexBuffer);
			size_t CreateConstantBuffer();
		
			ConstantBuffer& GetConstantBuffer(const size_t i);

			void BindVertexArray(const size_t i);
			void BindVertexShader(const size_t i);
			void BindPixelShader(const size_t i);

			virtual size_t CreateTexture(const std::wstring& filename) = 0;
			virtual size_t CreateTexture(const uint32_t w, const uint32_t h) = 0;

			virtual void BindVertexShaderBuffer(const size_t nBufferId, const size_t nPosition) = 0;
			virtual void BindVertexShaderTexture(const size_t nTextureId, const size_t nPosition) = 0;

			virtual void BindPixelShaderBuffer(const size_t nBufferId, const size_t nPosition) = 0;
			virtual void BindPixelShaderTexture(const size_t nTextureId, const size_t nPosition) = 0;

			void DrawIndexed();

			void ClearDepthBuffer();

			const float GetDepthBufferValue(const uint32_t x, const uint32_t y) const;

		protected:
			size_t CreateVertexShaderInternal(void* pFnShader);
			size_t CreatePixelShaderInternal(void* pFnShader);

			virtual void ProcessVertex(void* pVertexShader, Vertex& out, const Vertex& in) = 0;
			virtual void ProcessPixel(void* pPixelShader, uint32_t x, uint32_t y, const Vertex& in) = 0;


		private:
			struct Triangle {
				Vertex v0;
				Vertex v1;
				Vertex v2;
			};

			void AssembleTriangles(const VertexArray& va);
			void ClipTriangle(Triangle& t);
			void TransformTriangle(Triangle& t);
			void CullTriangle(Triangle& t);
			void RasterizeTriangle(Triangle& t);

		protected:
			inline const PipelineCoreDescriptor& GetDescriptor() const { return m_descriptor; }
			template <typename VertexShaderType>
			inline VertexShaderType& GetVertexShader(const size_t i) { return *reinterpret_cast<VertexShaderType*>(m_vpVertexShaders[i]); }
			template <typename PixelShaderType>
			inline PixelShaderType& GetPixelShader(const size_t i) { return *reinterpret_cast<PixelShaderType*>(m_vpPixelShaders[i]); }
			template <typename VertexShaderType>
			inline VertexShaderType& GetBoundVertexShader() { return *reinterpret_cast<VertexShaderType*>(m_pBoundVertexShader); }
			template <typename PixelShaderType>
			inline PixelShaderType& GetBoundPixelShader() { return *reinterpret_cast<PixelShaderType*>(m_pBoundPixelShader); }

		private:
			std::vector<VertexArray> m_vVertexArrays;
			std::vector<ConstantBuffer> m_vConstantBuffers;

			PipelineCoreDescriptor m_descriptor;
			std::vector<void*> m_vpVertexShaders;
			std::vector<void*> m_vpPixelShaders;
			void* m_pBoundVertexShader = nullptr;
			void* m_pBoundPixelShader = nullptr;

			VertexArray* m_pBoundVertexArray = nullptr;

			float* m_pDepthBuffer = nullptr;
		};
	}
}