#include "wcgeModel.hpp"

void VertexShader(wcge::c3d::Vertex& out, const wcge::c3d::Vertex& in, const wcge::c3d::ShaderBindingGeneric<wcge::Image>& binding) {
	using namespace wcge::Math;
	using namespace wcge::r3d;

	const Mat4x4& viewMatrix = binding.pCbuffers[ObjectConstantBufferPosition::VIEW_MATRIX]->mat4_00;
	// const Mat3x3& rotationMatrix = binding.pCbuffers[ObjectConstantBufferPosition::ROTATION_MATRIX]->mat3_00;

	out.position = in.position * viewMatrix;
	// out.normal = in.normal * rotationMatrix;
	out.uv = in.uv;
}

wcge::Pixel PixelShader(const wcge::c3d::Vertex& in, const wcge::c3d::ShaderBindingGeneric<wcge::Image>& binding) {
	uint32_t x = (uint32_t)(in.uv.x * binding.pTextures[0]->GetWidth());
	uint32_t y = (uint32_t)(in.uv.y * binding.pTextures[0]->GetHeight());
	return binding.pTextures[0]->GetPixel(x, y);
}

wcge::r3d::Model::Model(c3d::PixelPipeline* pPipeline, const c3d::VertexBuffer& vertexBuffer, const c3d::IndexBuffer& indexBuffer, const std::wstring& texturePath)
	: m_pPipeline(pPipeline), m_pCamera(nullptr), m_vPosition(), m_vRotation(), m_vScale(1.0f, 1.0f, 1.0f) {
	m_nVertexArrayId = m_pPipeline->CreateVertexArray(vertexBuffer, indexBuffer);
	m_nVertexShaderId = m_pPipeline->CreateVertexShader(VertexShader);
	m_nPixelShaderId = m_pPipeline->CreatePixelShader(PixelShader);
	m_nTextureId = m_pPipeline->CreateTexture(texturePath);
	m_nViewMatrixId = m_pPipeline->CreateConstantBuffer();
	m_nRotationMatrixId = m_pPipeline->CreateConstantBuffer();
}

wcge::r3d::Model::~Model()
{
}

wcge::r3d::Model* wcge::r3d::Model::LoadFromFile(c3d::PixelPipeline* pPipeline, const std::string& path, const std::wstring& texturePath) {
	struct VertexIndex {
		uint32_t nPositionIndex;
		uint32_t nNormalIndex;
		uint32_t nUVIndex;

		inline bool operator==(const VertexIndex& other) const {
			return nPositionIndex == other.nPositionIndex && nNormalIndex == other.nNormalIndex && nUVIndex == other.nUVIndex;
		}
	};

	struct VertexIndexHashKey {
		size_t operator()(const VertexIndex& vi) const {
			return std::hash<uint32_t>()(vi.nPositionIndex) ^ std::hash<uint32_t>()(vi.nNormalIndex) ^ std::hash<uint32_t>()(vi.nUVIndex);
		}
	};

	struct Face {
		uint32_t nVerticesPerFace;
		std::vector<VertexIndex> vVertexIndices;
	};

	std::vector<Math::Vector3> vPositions;
	std::vector<Math::Vector3> vNormals;
	std::vector<Math::Vector2> vUVs;
	std::vector<Face> vFaces;

	bool bUVsPresent = false;
	bool bNormalsPresent = false;

	std::unordered_map<VertexIndex, uint32_t, VertexIndexHashKey> mVertexIndexMap;

	c3d::VertexBuffer vertexBuffer;
	c3d::IndexBuffer indexBuffer;

	const auto SplitString = [](const std::string& str, const char delimiter) -> std::vector<std::string> {
		std::stringstream ss(str);
		std::string segment;
		std::vector<std::string> segments;

		while (std::getline(ss, segment, delimiter)) {
			segments.push_back(segment);
		}
		return segments;
		};

	const auto LoadObjectIntoMemory = [&]() -> bool {
		vPositions.clear();
		vNormals.clear();
		vUVs.clear();
		vFaces.clear();

		std::ifstream file(path);

		if (!file.is_open()) {
			return false;
		}

		// Loop through all the lines in the file
		std::string line;
		while (std::getline(file, line)) {

			// break the line into words
			std::istringstream iss(line);
			std::vector<std::string> words(
				std::istream_iterator<std::string>{iss},
				std::istream_iterator<std::string>()
			);

			if (!words.empty()) {
				if (words[0] == "v") {
					vPositions.push_back({ -std::stof(words[1]), std::stof(words[2]), std::stof(words[3]) });
				}
				else if (words[0] == "vt") {
					bUVsPresent = true;
					vUVs.push_back({ std::stof(words[1]), 1.0f - std::stof(words[2]) });
				}
				else if (words[0] == "vn") {
					bNormalsPresent = true;
					vNormals.push_back({ -std::stof(words[1]), std::stof(words[2]), std::stof(words[3]) });
				}
				else if (words[0] == "f") {
					Face face;
					face.nVerticesPerFace = (uint32_t)words.size() - 1;
					face.vVertexIndices.resize((size_t)face.nVerticesPerFace);

					for (uint32_t i = face.nVerticesPerFace; i > 0; i--) {
						VertexIndex& f = face.vVertexIndices[face.nVerticesPerFace - i];
						std::vector<std::string> indices = SplitString(words[i], '/');
						f.nPositionIndex = (uint32_t)std::stoi(indices[0]) - 1u;
						f.nUVIndex = bUVsPresent ? (uint32_t)std::stoi(indices[1]) - 1u : 0u;
						f.nNormalIndex = bNormalsPresent ? (uint32_t)std::stoi(indices[2]) - 1u : 0u;
					}

					vFaces.push_back(face);
				}
			}
		}

		// close the file
		file.close();
		return true;

		};

	const auto CreateBuffers = [&]() -> void {
		for (const auto& face : vFaces) {
			for (uint32_t i = 1; i < face.nVerticesPerFace - 1u; i++) {

				const auto& v0 = face.vVertexIndices[0];
				const auto& v1 = face.vVertexIndices[i];
				const auto& v2 = face.vVertexIndices[i + 1];

				if (mVertexIndexMap.find(v0) == mVertexIndexMap.end()) {
					// a new vertex found
					c3d::Vertex v;
					v.position = vPositions.at((size_t)v0.nPositionIndex);
					v.uv = bUVsPresent ? vUVs.at((size_t)v0.nUVIndex) : Math::Vector2{};
					v.normal = bNormalsPresent ? vNormals.at((size_t)v0.nNormalIndex) : Math::Vector3{};

					vertexBuffer.push_back(v);
					uint32_t index = (uint32_t)(vertexBuffer.size() - 1);
					mVertexIndexMap.insert({ v0,index });
					indexBuffer.push_back(index);
				}
				else {
					// vertex already exists
					indexBuffer.push_back(mVertexIndexMap.at(v0));
				}

				if (mVertexIndexMap.find(v1) == mVertexIndexMap.end()) {
					// a new vertex found
					c3d::Vertex v;
					v.position = vPositions.at((size_t)v1.nPositionIndex);
					v.uv = bUVsPresent ? vUVs.at((size_t)v1.nUVIndex) : Math::Vector2{};
					v.normal = bNormalsPresent ? vNormals.at((size_t)v1.nNormalIndex) : Math::Vector3{};

					vertexBuffer.push_back(v);
					uint32_t index = (uint32_t)(vertexBuffer.size() - 1);
					mVertexIndexMap.insert({ v1,index });
					indexBuffer.push_back(index);
				}
				else {
					// vertex already exists
					indexBuffer.push_back(mVertexIndexMap.at(v1));
				}

				if (mVertexIndexMap.find(v2) == mVertexIndexMap.end()) {
					// a new vertex found
					c3d::Vertex v;
					v.position = vPositions.at((size_t)v2.nPositionIndex);
					v.uv = bUVsPresent ? vUVs.at((size_t)v2.nUVIndex) : Math::Vector2{};
					v.normal = bNormalsPresent ? vNormals.at((size_t)v2.nNormalIndex) : Math::Vector3{};

					vertexBuffer.push_back(v);
					uint32_t index = (uint32_t)(vertexBuffer.size() - 1);
					mVertexIndexMap.insert({ v2,index });
					indexBuffer.push_back(index);
				}
				else {
					// vertex already exists
					indexBuffer.push_back(mVertexIndexMap.at(v2));
				}
			}
		}
		};

	if (LoadObjectIntoMemory()) {

		CreateBuffers();
		return new Model(pPipeline, vertexBuffer, indexBuffer, texturePath);
	}

	return nullptr;
}

void wcge::r3d::Model::Delete(Model* pObject) {
	if (pObject == nullptr) return;
	delete pObject;
}

void wcge::r3d::Model::Draw() {
	using namespace wcge::Math;

	if (m_pCamera == nullptr) return;

	m_pPipeline->BindVertexArray(m_nVertexArrayId);
	m_pPipeline->BindVertexShader(m_nVertexShaderId);
	m_pPipeline->BindVertexShaderBuffer(m_nViewMatrixId, ObjectConstantBufferPosition::VIEW_MATRIX);
	m_pPipeline->BindVertexShaderBuffer(m_nRotationMatrixId, ObjectConstantBufferPosition::ROTATION_MATRIX);
	m_pPipeline->BindPixelShader(m_nPixelShaderId);
	m_pPipeline->GetConstantBuffer(m_nViewMatrixId).mat4_00 = Mat4MakeScale(m_vScale) * Mat4MakeRotationZXY(m_vRotation) * Mat4MakeTranslation(m_vPosition) * m_pCamera->GetCameraViewMatrix();
	m_pPipeline->GetConstantBuffer(m_nRotationMatrixId).mat3_00 = Mat3MakeRotationZXY(m_vRotation);
	m_pPipeline->BindPixelShaderTexture(m_nTextureId, 0);

	m_pPipeline->DrawIndexed();
}

const wcge::c3d::VertexBuffer wcge::r3d::CreateCubeVertexBuffer(float size) {
	using namespace wcge::c3d;
	using namespace wcge::Math;

	return c3d::VertexBuffer{
			Vertex(Vector4(-size, -size, -size), Vector3(0.0f, 0.0f, -1.0f), Vector2(0.0f, 1.0f)),
			Vertex(Vector4(-size,  size, -size), Vector3(0.0f, 0.0f, -1.0f), Vector2(0.0f, 0.0f)),
			Vertex(Vector4(size,  size, -size), Vector3(0.0f, 0.0f, -1.0f), Vector2(1.0f, 0.0f)),
			Vertex(Vector4(size, -size, -size), Vector3(0.0f, 0.0f, -1.0f), Vector2(1.0f, 1.0f)),

			Vertex(Vector4(size, -size, -size), Vector3(1.0f, 0.0f, 0.0f), Vector2(0.0f, 1.0f)),
			Vertex(Vector4(size,  size, -size), Vector3(1.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f)),
			Vertex(Vector4(size,  size,  size), Vector3(1.0f, 0.0f, 0.0f), Vector2(1.0f, 0.0f)),
			Vertex(Vector4(size, -size,  size), Vector3(1.0f, 0.0f, 0.0f), Vector2(1.0f, 1.0f)),

			Vertex(Vector4(size, -size,  size), Vector3(0.0f, 0.0f, 1.0f), Vector2(0.0f, 1.0f)),
			Vertex(Vector4(size,  size,  size), Vector3(0.0f, 0.0f, 1.0f), Vector2(0.0f, 0.0f)),
			Vertex(Vector4(-size,  size,  size), Vector3(0.0f, 0.0f, 1.0f), Vector2(1.0f, 0.0f)),
			Vertex(Vector4(-size, -size,  size), Vector3(0.0f, 0.0f, 1.0f), Vector2(1.0f, 1.0f)),

			Vertex(Vector4(-size, -size,  size), Vector3(-1.0f, 0.0f, 0.0f), Vector2(0.0f, 1.0f)),
			Vertex(Vector4(-size,  size,  size), Vector3(-1.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f)),
			Vertex(Vector4(-size,  size, -size), Vector3(-1.0f, 0.0f, 0.0f), Vector2(1.0f, 0.0f)),
			Vertex(Vector4(-size, -size, -size), Vector3(-1.0f, 0.0f, 0.0f), Vector2(1.0f, 1.0f)),

			Vertex(Vector4(-size,  size, -size), Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 1.0f)),
			Vertex(Vector4(-size,  size,  size), Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 0.0f)),
			Vertex(Vector4(size,  size,  size), Vector3(0.0f, 1.0f, 0.0f), Vector2(1.0f, 0.0f)),
			Vertex(Vector4(size,  size, -size), Vector3(0.0f, 1.0f, 0.0f), Vector2(1.0f, 1.0f)),

			Vertex(Vector4(size,  -size, -size), Vector3(0.0f, -1.0f, 0.0f), Vector2(0.0f, 1.0f)),
			Vertex(Vector4(size,  -size,  size), Vector3(0.0f, -1.0f, 0.0f), Vector2(0.0f, 0.0f)),
			Vertex(Vector4(-size, -size,  size), Vector3(0.0f, -1.0f, 0.0f), Vector2(1.0f, 0.0f)),
			Vertex(Vector4(-size, -size, -size), Vector3(0.0f, -1.0f, 0.0f), Vector2(1.0f, 1.0f)),
	};
}

const wcge::c3d::IndexBuffer wcge::r3d::CreateCubeIndexBuffer() {
	return c3d::IndexBuffer{
		0, 1, 2,
		0, 2, 3,

		4, 5, 6,
		4, 6, 7,

		8, 9, 10,
		8, 10, 11,

		12, 13, 14,
		12, 14, 15,

		16, 17, 18,
		16, 18, 19,

		20, 21, 22,
		20, 22, 23
	};
}

