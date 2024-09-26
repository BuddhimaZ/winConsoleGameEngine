#include "olcPixelGameEngine.h"

#include "wcge3dCamera.hpp"
#include "wcgeModel.hpp"
#include "wcgePixelPipeline.hpp"

using namespace wcge;
using namespace wcge::c3d;
using namespace wcge::Math;

class Pxel3DEngneTestbed : public olc::PixelGameEngine {
public:
	Pxel3DEngneTestbed() {
		sAppName = "Pxel3DEngneTestbed";
	}

private:
	bool OnUserCreate() override {

		m_pPipeline = new wcge::c3d::PixelPipeline(this);

		m_pCamera = new wcge::r3d::Camera(
			static_cast<uint32_t>(ScreenWidth()),
			static_cast<uint32_t>(ScreenHeight()),
			m_fFOV,
			m_fZNear,
			m_fZFar
		);

		m_pCamera->SetMovementSpeed(25.0f);
		m_pCamera->SetRotationSpeed(3.0f);

		m_pCamera->SetPosition(Vector3(0.0f, 0.0f, -10.0f));

		//m_vObjects.push_back(new r3d::Object(
//	m_pPipeline,
//	r3d::CreateCubeVertexBuffer(3.0f),
//	r3d::CreateCubeIndexBuffer(),
//	L"wcgeResources/texture.jpg"
//));
//m_vObjects[1]->GetPosition() = Vector3(0.0f, 200.0f, 0.0f);

		m_vObjects.push_back(new r3d::Model(
			m_pPipeline,
			r3d::CreateCubeVertexBuffer(1.0f),
			r3d::CreateCubeIndexBuffer(),
			L"wcgeResources/texture.jpg"
		));

		m_vObjects.push_back(new r3d::Model(
			m_pPipeline,
			r3d::CreateCubeVertexBuffer(1.0f),
			r3d::CreateCubeIndexBuffer(),
			L"wcgeResources/texture_2.jpg"
		));

		m_vObjects.push_back(new r3d::Model(
			m_pPipeline,
			r3d::CreateCubeVertexBuffer(1.0f),
			r3d::CreateCubeIndexBuffer(),
			L"wcgeResources/texture_3.jpg"
		));

		m_vObjects[1]->GetPosition() += Vector3(5.0f, 0.0f, 0.0f);
		m_vObjects[2]->GetPosition() += Vector3(-5.0f, 0.0f, 0.0f);

		m_vObjects.push_back(r3d::Model::LoadFromFile(
			m_pPipeline,
			"wcgeResources/dragon.obj",
			L"wcgeResources/dragon.png"
		));

		m_vObjects[3]->GetScale() = Vector3(1.0f, 1.0f, 1.0f);
		m_vObjects[3]->GetPosition() = Vector3(0.0f, -50.0f, 0.0f);

		for (auto& object : m_vObjects) {
			object->SetCamera(m_pCamera);
		}

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override {
		float dt = fElapsedTime;

		if (GetKey(olc::Key::SPACE).bReleased) {
			m_bFroze = !m_bFroze;
		}

		if (m_bFroze) {
			dt = 0.0f;
		}

		HandleMovement(dt);

		Clear(olc::BLACK);
		m_pPipeline->ClearDepthBuffer();

		for (auto object : m_vObjects) {
			object->Draw();
		}

		for (uint32_t y = 0; y < ScreenHeight(); y++) {
			for (uint32_t x = 0; x < ScreenWidth(); x++) {
				float fZNormalized = GetZNormalized(m_pPipeline->GetDepthBufferValue(x, y));

				olc::Pixel p = GetDrawTarget()->GetPixel(x, y);
				p = // p * fZNormalized;
					olc::PixelF(fZNormalized, fZNormalized, fZNormalized, 1.0f);

				Draw(x, y, p);
			}
		}

		return true;
	}

	bool OnUserDestroy() override {
		for (auto& object : m_vObjects) {
			delete object;
		}
		delete m_pCamera;
		delete m_pPipeline;
		return true;
	}


	void HandleMovement(float dt) {
		if (GetKey(olc::Key::W).bHeld) {
			m_pCamera->MoveAlongZ(dt);
		}

		if (GetKey(olc::Key::S).bHeld) {
			m_pCamera->MoveAlongZ(-dt);
		}

		if (GetKey(olc::Key::A).bHeld) {
			m_pCamera->MoveAlongX(-dt);
		}

		if (GetKey(olc::Key::D).bHeld) {
			m_pCamera->MoveAlongX(dt);
		}

		if (GetKey(olc::Key::Q).bHeld) {
			m_pCamera->MoveAlongY(dt);
		}

		if (GetKey(olc::Key::E).bHeld) {
			m_pCamera->MoveAlongY(-dt);
		}

		if (GetKey(olc::Key::UP).bHeld) {
			m_pCamera->RotateAroundX(-dt);
		}

		if (GetKey(olc::Key::DOWN).bHeld) {
			m_pCamera->RotateAroundX(dt);
		}

		if (GetKey(olc::Key::LEFT).bHeld) {
			m_pCamera->RotateAroundY(-dt);
		}

		if (GetKey(olc::Key::RIGHT).bHeld) {
			m_pCamera->RotateAroundY(dt);
		}

		if (GetKey(olc::Key::Z).bHeld) {
			m_pCamera->RotateAroundZ(dt);
		}

		if (GetKey(olc::Key::X).bHeld) {
			m_pCamera->RotateAroundZ(-dt);
		}
	}


	wcge::c3d::PixelPipeline* m_pPipeline = nullptr;

	bool m_bFroze = false;

	wcge::r3d::Camera* m_pCamera;

	std::vector<wcge::r3d::Model*> m_vObjects;

	static constexpr float m_fFOV = 90.0f;
	static constexpr float m_fZNear = 1.0f;
	static constexpr float m_fZFar = 300.0f;

	static inline float GetZNormalized(const float fPerspectiveCorrectedDepth) {
		constexpr float fQ = m_fZFar / (m_fZFar - m_fZNear);
		float fZ = fQ * m_fZNear / (fQ - fPerspectiveCorrectedDepth);
		return 1.0f - (fZ - m_fZNear) / (m_fZFar - m_fZNear);
	}

};

int main() {
	Pxel3DEngneTestbed demo;
	if (demo.Construct(800, 600, 1, 1) == olc::rcode::OK) demo.Start();
}