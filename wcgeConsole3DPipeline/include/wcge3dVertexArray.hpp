#pragma once

#include "wcge3dVertex.hpp"

#include <vector>
#include <cstdint>

namespace wcge {
	namespace c3d {
		typedef std::vector<Vertex> VertexBuffer;
		typedef std::vector<uint32_t> IndexBuffer;

		struct VertexArray {
			inline VertexArray( const VertexBuffer& vVertexBuffer, const IndexBuffer& vIndexBuffer ) : m_vVertexBuffer(vVertexBuffer), m_vProcessedVertexBuffer( vVertexBuffer.size() ), m_vIndexBuffer(vIndexBuffer) {}

			VertexBuffer m_vVertexBuffer;
			VertexBuffer m_vProcessedVertexBuffer;
			IndexBuffer m_vIndexBuffer;
		};
	}
}