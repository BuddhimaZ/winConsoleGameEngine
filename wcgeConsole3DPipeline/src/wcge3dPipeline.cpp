#include <wcge3dPipeline.hpp>

wcge::c3d::Pipeline::Pipeline( ConsoleGameEngine* pEngine ) : m_pEngine( pEngine ) {
	m_pDepthBuffer = new float[m_pEngine->GetBackBuffer().GetWidth() * m_pEngine->GetBackBuffer().GetHeight()];
	ClearDepthBuffer();
}

wcge::c3d::Pipeline::~Pipeline() {
	delete[] m_pDepthBuffer;
}

void wcge::c3d::Pipeline::DrawIndexed() {
	if ( !m_pBoundVertexArray || !m_pBoundVertexShader || !m_pBoundPixelShader ) return;

	for ( size_t i = 0; i < m_pBoundVertexArray->m_vVertexBuffer.size(); i++ ) {
		m_pBoundVertexShader->main( m_pBoundVertexArray->m_vProcessedVertexBuffer[i], m_pBoundVertexArray->m_vVertexBuffer[i], m_pBoundVertexShader->binding );
	}

	AssembleTriangles( *m_pBoundVertexArray );
}

void wcge::c3d::Pipeline::ClearDepthBuffer() {
	for (uint64_t i = 0; i < static_cast<uint64_t>(m_pEngine->GetBackBuffer().GetWidth()) * static_cast<uint64_t>(m_pEngine->GetBackBuffer().GetHeight()); i++) {
		m_pDepthBuffer[i] = 1.0f;
	}
}

void wcge::c3d::Pipeline::AssembleTriangles( const VertexArray& va ) {
	for ( size_t i = 0; i < va.m_vIndexBuffer.size(); i += 3 ) {
		Triangle t = {
				va.m_vProcessedVertexBuffer[va.m_vIndexBuffer[i]],
				va.m_vProcessedVertexBuffer[va.m_vIndexBuffer[i + 1]],
				va.m_vProcessedVertexBuffer[va.m_vIndexBuffer[i + 2]],
		};
		ClipTriangle( t );
	}
}

void wcge::c3d::Pipeline::ClipTriangle( Triangle& t ) {
	if ( t.v0.position.x > t.v0.position.w && t.v1.position.x > t.v1.position.w && t.v2.position.x > t.v2.position.w ) return;
	if ( t.v0.position.x < -t.v0.position.w && t.v1.position.x < -t.v1.position.w && t.v2.position.x < -t.v2.position.w ) return;

	if ( t.v0.position.y > t.v0.position.w && t.v1.position.y > t.v1.position.w && t.v2.position.y > t.v2.position.w ) return;
	if ( t.v0.position.y < -t.v0.position.w && t.v1.position.y < -t.v1.position.w && t.v2.position.y < -t.v2.position.w ) return;

	if ( t.v0.position.z > t.v0.position.w && t.v1.position.z > t.v1.position.w && t.v2.position.z > t.v2.position.w ) return;
	if ( t.v0.position.z < 0.0f && t.v1.position.z < 0.0f && t.v2.position.z < 0.0f ) return;

	const auto ClipTriangleZnearPlane1Vertex = [this]( Triangle& t ) {
		const float alphaA = (-(t.v0.position.z)) / ((t.v1.position.z) - (t.v0.position.z));
		const float alphaB = (-(t.v0.position.z)) / ((t.v2.position.z) - (t.v0.position.z));

		Vertex vertex0a = Math::Interpolate( t.v0, t.v1, alphaA );
		Vertex vertex0b = Math::Interpolate( t.v0, t.v2, alphaB );

		Triangle tA = { vertex0a, t.v1, t.v2 };
		Triangle tB = { vertex0a, t.v2, vertex0b };

		TransformTriangle( tA );
		TransformTriangle( tB );
	};

	const auto ClipTriangleZNearPlane2Vertices = [this]( Triangle& t ) {
		const float alpha0 = (-t.v0.position.z) / (t.v2.position.z - t.v0.position.z);
		const float alpha1 = (-t.v1.position.z) / (t.v2.position.z - t.v1.position.z);

		t.v0 = Math::Interpolate( t.v0, t.v2, alpha0 );
		t.v1 = Math::Interpolate( t.v1, t.v2, alpha1 );

		TransformTriangle( t );
	};

	if ( t.v0.position.z < 0.0f ) {
		if ( t.v1.position.z < 0.0f ) {
			ClipTriangleZNearPlane2Vertices( t );
		}
		else if ( t.v2.position.z < 0.0f ) {
			t = { t.v2, t.v0, t.v1 };
			ClipTriangleZNearPlane2Vertices( t );
		}
		else {
			ClipTriangleZnearPlane1Vertex( t );
		}
	}
	else if ( t.v1.position.z < 0.0f ) {
		if ( t.v2.position.z < 0.0f ) {
			t = { t.v1, t.v2, t.v0 };
			ClipTriangleZNearPlane2Vertices( t );
		}
		else {
			t = { t.v1, t.v2, t.v0 };
			ClipTriangleZnearPlane1Vertex( t );
		}
	}
	else if ( t.v2.position.z < 0.0f ) {
		t = { t.v2, t.v0, t.v1 };
		ClipTriangleZnearPlane1Vertex( t );
	}
	else {
		TransformTriangle( t );
	}
}

void wcge::c3d::Pipeline::TransformTriangle( Triangle& t ) {
	float fWInverse = 1.0f / t.v0.position.w;
	t.v0 = t.v0 * fWInverse;
	t.v0.position.w = fWInverse;

	fWInverse = 1.0f / t.v1.position.w;
	t.v1 = t.v1 * fWInverse;
	t.v1.position.w = fWInverse;

	fWInverse = 1.0f / t.v2.position.w;
	t.v2 = t.v2 * fWInverse;
	t.v2.position.w = fWInverse;

	CullTriangle( t );
}

void wcge::c3d::Pipeline::CullTriangle( Triangle& t ) {
	if ( Math::Vec4CrossProduct( t.v1.position - t.v0.position, t.v2.position - t.v0.position ).z <= 0.0f ) {
		t.v0.position.x = (0.5f + (t.v0.position.x * 0.5f)) * static_cast<float>(m_pEngine->GetBackBuffer().GetWidth());
		t.v0.position.y = (0.5f - (t.v0.position.y * 0.5f)) * static_cast<float>(m_pEngine->GetBackBuffer().GetHeight());
		
		t.v1.position.x = (0.5f + (t.v1.position.x * 0.5f)) * static_cast<float>(m_pEngine->GetBackBuffer().GetWidth());
		t.v1.position.y = (0.5f - (t.v1.position.y * 0.5f)) * static_cast<float>(m_pEngine->GetBackBuffer().GetHeight());
		
		t.v2.position.x = (0.5f + (t.v2.position.x * 0.5f)) * static_cast<float>(m_pEngine->GetBackBuffer().GetWidth());
		t.v2.position.y = (0.5f - (t.v2.position.y * 0.5f)) * static_cast<float>(m_pEngine->GetBackBuffer().GetHeight());

		RasterizeTriangle( t );
	}
}

void wcge::c3d::Pipeline::RasterizeTriangle( Triangle& t ) {
	const auto DepthCheck = [this]( const uint32_t x, const uint32_t y, const float z ) {
		if ( x < m_pEngine->GetBackBuffer().GetWidth() && y < m_pEngine->GetBackBuffer().GetHeight() ) {
			float& current_depth = m_pDepthBuffer[x + y * m_pEngine->GetBackBuffer().GetWidth()];
			if ( z < current_depth ) {
				current_depth = z;
				return true;
			}
		}
		return false;
	};

	const auto RasterizeTopFlatTriangle = [&](const Vertex& v0, const Vertex& v1, const Vertex& v2) {

		const int yStart = static_cast<int>( std::ceilf( v0.position.y ) );
		const int yEnd = static_cast<int>( std::ceilf( v2.position.y ) );

		float dy = v2.position.y - v0.position.y;


		for (int y = yStart; y < yEnd; y++) {
			// float alpha = static_cast<float>(y - yStart) / static_cast<float>(yEnd - yStart);
			float alpha = (static_cast<float>(y) - v0.position.y) / dy;
			Vertex vEdge0 = Math::Interpolate(v0, v2, alpha);
			Vertex vEdge1 = Math::Interpolate(v1, v2, alpha);

			const int xStart = static_cast<int>(std::ceilf(vEdge0.position.x));
			const int xEnd = static_cast<int>(std::ceilf(vEdge1.position.x));

			float dx = vEdge1.position.x - vEdge0.position.x;

			for (int x = xStart; x < xEnd; x++) {
				// alpha = static_cast<float>(x - xStart) / static_cast<float>(xEnd - xStart);
				alpha = (static_cast<float>(x) - vEdge0.position.x) / dx;
				Vertex vFinal = Math::Interpolate(vEdge0, vEdge1, alpha);

				if (DepthCheck(x, y, vFinal.position.z)) {
					const float w = 1.0f / vFinal.position.w;
					CB cb = m_pBoundPixelShader->main(vFinal * w, m_pBoundPixelShader->binding);
					m_pEngine->GetBackBuffer().SetCB(static_cast<uint32_t>(x), static_cast<uint32_t>(y), cb);
				}
			}

		}
	};

	const auto RasterizeBottomFlatTriangle = [&](const Vertex& v0, const Vertex& v1, const Vertex& v2) {

		const int yStart = static_cast<int>(std::ceilf(v0.position.y));
		const int yEnd = static_cast<int>(std::ceilf(v2.position.y));

		float dy = v2.position.y - v0.position.y;

		for (int y = yStart; y < yEnd; y++) {
			// float alpha = static_cast<float>(y - yStart) / static_cast<float>(yEnd - yStart);
			float alpha = (static_cast<float>(y) - v0.position.y) / dy;
			Vertex vEdge1 = Math::Interpolate(v0, v2, alpha);
			Vertex vEdge2 = Math::Interpolate(v0, v1, alpha);

			const int xStart = static_cast<int>(std::ceilf(vEdge1.position.x));
			const int xEnd = static_cast<int>(std::ceilf(vEdge2.position.x));

			float dx = vEdge2.position.x - vEdge1.position.x;

			for (int x = xStart; x < xEnd; x++) {
				// alpha = static_cast<float>(x - xStart) / static_cast<float>(xEnd - xStart);
				alpha = (static_cast<float>(x) - vEdge1.position.x) / dx;
				Vertex vFinal = Math::Interpolate(vEdge1, vEdge2, alpha);

				if (DepthCheck(x, y, vFinal.position.z)) {
					const float w = 1.0f / vFinal.position.w;
					CB cb = m_pBoundPixelShader->main(vFinal * w, m_pBoundPixelShader->binding);
					m_pEngine->GetBackBuffer().SetCB(static_cast<uint32_t>(x), static_cast<uint32_t>(y), cb);
				}
			}

		}

	};

	Vertex* pV0 = reinterpret_cast<Vertex*>(&t.v0);
	Vertex* pV1 = reinterpret_cast<Vertex*>(&t.v1);
	Vertex* pV2 = reinterpret_cast<Vertex*>(&t.v2);

	// Sort vertices by y of ther position
	if (pV1->position.y < pV0->position.y) std::swap(pV0, pV1);
	if (pV2->position.y < pV0->position.y) std::swap(pV0, pV2);
	if (pV2->position.y < pV1->position.y) std::swap(pV1, pV2);


	if (pV0->position.y == pV1->position.y) {
		if (pV0->position.x > pV1->position.x) std::swap(pV0, pV1);
		RasterizeTopFlatTriangle(*pV0, *pV1, *pV2);
	}
	else if (pV1->position.y == pV2->position.y) {
		if (pV2->position.x > pV1->position.x) std::swap(pV1, pV2);
		RasterizeBottomFlatTriangle(*pV0, *pV1, *pV2);
	}
	else {
		const float alphaSplit = (pV1->position.y - pV0->position.y) / (pV2->position.y - pV0->position.y);
		Vertex vSplit = Math::Interpolate(*pV0, *pV2, alphaSplit);

		if (vSplit.position.x < pV1->position.x) {
			RasterizeBottomFlatTriangle(*pV0, *pV1, vSplit);
			RasterizeTopFlatTriangle(vSplit, *pV1, *pV2);
		}
		else {
			RasterizeBottomFlatTriangle(*pV0, vSplit, *pV1);
			RasterizeTopFlatTriangle(*pV1, vSplit, *pV2);
		}
	}

	//m_pEngine->DrawTriangle(
	//	static_cast<uint32_t>(t.v0.position.x), static_cast<uint32_t>(t.v0.position.y),
	//	static_cast<uint32_t>(t.v1.position.x), static_cast<uint32_t>(t.v1.position.y),
	//	static_cast<uint32_t>(t.v2.position.x), static_cast<uint32_t>(t.v2.position.y)
	//);
}