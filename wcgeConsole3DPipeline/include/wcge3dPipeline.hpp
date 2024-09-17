#pragma once

#include <wcgeConsoleGameEngine.hpp>
#include "wcge3dVertex.hpp"
#include "wcge3dConstantBuffer.hpp"
#include "wcge3dShaders.hpp"
#include "wcge3dVertexArray.hpp"

#include <vector>
#include <unordered_map>

namespace wcge {
	namespace c3d {

		class Pipeline {
		public:
			Pipeline( ConsoleGameEngine* pEngine );
			~Pipeline();

			inline size_t CreateVertexArray( const VertexBuffer& vVertexBuffer, const IndexBuffer& vIndexBuffer ) { m_vVertexArrays.emplace_back( vVertexBuffer, vIndexBuffer ); return m_vVertexArrays.size() - 1; }
			inline size_t CreateConstantBuffer() { ConstantBuffer buf{}; m_vConstantBuffers.push_back( buf ); return m_vConstantBuffers.size() - 1; }
			inline size_t CreateTexture(const std::wstring& filename) { m_vTextures.emplace_back(Image(filename)); return m_vTextures.size() - 1; }
			inline size_t CreateTexture( const uint32_t w, const uint32_t h ) { m_vTextures.emplace_back( w, h ); return m_vTextures.size() - 1; }
			inline size_t CreateVertexShader( PFNVERTEXSHADER pFnShader ) { m_vVertexShaders.emplace_back( pFnShader ); return m_vVertexShaders.size() - 1; }
			inline size_t CreatePixelShader( PFNPIXELSHADER pFnShader ) { m_vPixelShaders.emplace_back( pFnShader ); return m_vPixelShaders.size() - 1; }

			inline ConstantBuffer& GetConstantBuffer( const size_t i ) { return m_vConstantBuffers[i]; }
			inline CBImage& GetTexture( const size_t i ) { return m_vTextures[i]; }

			inline void BindVertexArray( const size_t i ) { m_pBoundVertexArray = &(m_vVertexArrays[i]); }
			inline void BindVertexShader( const size_t i ) { m_pBoundVertexShader = &(m_vVertexShaders[i]); }
			inline void BindPixelShader( const size_t i ) { m_pBoundPixelShader = &(m_vPixelShaders[i]); }

			inline void BindVertexShaderBuffer( const size_t nBufferId, const size_t nPosition ) { m_pBoundVertexShader->binding.pCbuffers[nPosition] = &(m_vConstantBuffers[nBufferId]); }
			inline void BindVertexShaderTexture( const size_t nTextureId, const size_t nPosition ) { m_pBoundVertexShader->binding.pTextures[nPosition] = &(m_vTextures[nTextureId]); }

			inline void BindPixelShaderBuffer( const size_t nBufferId, const size_t nPosition ) { m_pBoundPixelShader->binding.pCbuffers[nPosition] = &(m_vConstantBuffers[nBufferId]); }
			inline void BindPixelShaderTexture( const size_t nTextureId, const size_t nPosition ) { m_pBoundPixelShader->binding.pTextures[nPosition] = &(m_vTextures[nTextureId]); }

			void DrawIndexed();

			void ClearDepthBuffer();

		private:
			struct Triangle {
				Vertex v0;
				Vertex v1;
				Vertex v2;
			};

			void AssembleTriangles( const VertexArray& va );
			void ClipTriangle( Triangle& t );
			void TransformTriangle( Triangle& t );
			void CullTriangle( Triangle& t );
			void RasterizeTriangle( Triangle& t );

		private:
			ConsoleGameEngine* m_pEngine = nullptr;

			std::vector<VertexArray> m_vVertexArrays;
			std::vector<ConstantBuffer> m_vConstantBuffers;
			std::vector<CBImage> m_vTextures;
			std::vector<VertexShader> m_vVertexShaders;
			std::vector<PixelShader> m_vPixelShaders;
		
			VertexArray* m_pBoundVertexArray = nullptr;
			VertexShader* m_pBoundVertexShader = nullptr;
			PixelShader* m_pBoundPixelShader = nullptr;

			float* m_pDepthBuffer = nullptr;
		};
	}
}