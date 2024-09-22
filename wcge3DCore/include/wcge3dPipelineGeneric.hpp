#pragma once

#include "wcge3dPipelineCore.hpp"
#include "wcge3dShadersGeneric.hpp"

namespace wcge {
	namespace c3d {
		template <typename PixelType, typename TextureType>
		struct PipelineGeneric : public PipelineCore {
		public:
			using PipelineCore::PipelineCore;

			virtual ~PipelineGeneric() {
				for (auto p : m_vpTextures) {
					delete p;
				}
			}

			virtual size_t CreateTexture(const std::wstring& filename) override {
				m_vpTextures.push_back(new TextureType(filename));
				return m_vpTextures.size() - 1;
			}
			virtual size_t CreateTexture(const uint32_t w, const uint32_t h) override {
				m_vpTextures.push_back(new TextureType(w, h));
				return m_vpTextures.size() - 1;
			}

			virtual TextureType& GetTexture(const size_t i) {
				return *m_vpTextures[i];
			}

			size_t CreateVertexShader(PFNVERTEXSHADER<TextureType> pFnVertexShader) {
				VertexShaderGeneric<TextureType>* pShader = new VertexShaderGeneric<TextureType>(pFnVertexShader);
				return CreateVertexShaderInternal(reinterpret_cast<void*>(pShader));
			}

			size_t CreatePixelShader(PFNPIXELSHADER<PixelType, TextureType> pFnPixelShader) {
				PixelShaderGeneric<PixelType, TextureType>* pShader = new PixelShaderGeneric<PixelType, TextureType>(pFnPixelShader);
				return CreatePixelShaderInternal(reinterpret_cast<void*>(pShader));
			}

			virtual void BindVertexShaderBuffer(const size_t nBufferId, const size_t nPosition) override {
				GetBoundVertexShader<VertexShaderGeneric<TextureType>>().binding.pCbuffers[nPosition] = &(GetConstantBuffer(nBufferId));
			}
			virtual void BindVertexShaderTexture(const size_t nTextureId, const size_t nPosition) override {
				GetBoundVertexShader<VertexShaderGeneric<TextureType>>().binding.pTextures[nPosition] = &(GetTexture(nTextureId));
			}

			virtual void BindPixelShaderBuffer(const size_t nBufferId, const size_t nPosition) override {
				GetBoundPixelShader<PixelShaderGeneric<PixelType, TextureType>>().binding.pCbuffers[nPosition] = &(GetConstantBuffer(nBufferId));
			}
			virtual void BindPixelShaderTexture(const size_t nTextureId, const size_t nPosition) override {
				GetBoundPixelShader<PixelShaderGeneric<PixelType, TextureType>>().binding.pTextures[nPosition] = &(GetTexture(nTextureId));
			}



		protected:
			virtual void ProcessVertex(void* pVertexShader, Vertex& out, const Vertex& in) override {
				VertexShaderGeneric<TextureType>& shader = *reinterpret_cast<VertexShaderGeneric<TextureType>*>(pVertexShader);
				shader.main(out, in, shader.binding);
			}
			virtual void ProcessPixel(void* pPixelShader, uint32_t x, uint32_t y, const Vertex& in) override {
				PixelShaderGeneric<PixelType, TextureType>& shader = *reinterpret_cast<PixelShaderGeneric<PixelType, TextureType>*>(pPixelShader);
				PixelType p = shader.main(in, shader.binding);
				DrawPixel(x, y, p);
			}

			virtual void DrawPixel(const uint32_t x, const uint32_t y, PixelType pixel) = 0;
			
			std::vector<TextureType*> m_vpTextures;
		};
	}
}