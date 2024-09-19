#include <wcgeConsoleGameEngine.hpp>
#include <wcge3dPipeline.hpp>
#include <wcge3dCamera.hpp>
#include <wcge3dObject.hpp>

#include <iostream>

using namespace wcge;
using namespace wcge::c3d;
using namespace wcge::Math;

wcge::CB CreateCharacterBlockBasedOnIntensity(float intensity) {
	intensity = std::min(1.0f, std::max(0.0f, intensity));

	CB pixel;

	int pixel_bw = (int)(intensity * 13.0f);

	switch (pixel_bw) {
	case  0: pixel.color = static_cast<COLOUR>(static_cast<WORD>(COLOUR::COL_BG_BLACK) | static_cast<WORD>(COLOUR::COL_FG_BLACK)); pixel.character = static_cast<SHADED_CHARACTERS>(static_cast<WCHAR>(SHADED_CHARACTERS::SC_SOLID)); break;
	case  1: pixel.color = static_cast<COLOUR>(static_cast<WORD>(COLOUR::COL_BG_BLACK) | static_cast<WORD>(COLOUR::COL_FG_DARK_GREY)); pixel.character = static_cast<SHADED_CHARACTERS>(static_cast<WCHAR>(SHADED_CHARACTERS::SC_QUARTER)); break;
	case  2: pixel.color = static_cast<COLOUR>(static_cast<WORD>(COLOUR::COL_BG_BLACK) | static_cast<WORD>(COLOUR::COL_FG_DARK_GREY)); pixel.character = static_cast<SHADED_CHARACTERS>(static_cast<WCHAR>(SHADED_CHARACTERS::SC_HALF)); break;
	case  3: pixel.color = static_cast<COLOUR>(static_cast<WORD>(COLOUR::COL_BG_BLACK) | static_cast<WORD>(COLOUR::COL_FG_DARK_GREY)); pixel.character = static_cast<SHADED_CHARACTERS>(static_cast<WCHAR>(SHADED_CHARACTERS::SC_THREEQUARTERS)); break;
	case  4: pixel.color = static_cast<COLOUR>(static_cast<WORD>(COLOUR::COL_BG_BLACK) | static_cast<WORD>(COLOUR::COL_FG_DARK_GREY)); pixel.character = static_cast<SHADED_CHARACTERS>(static_cast<WCHAR>(SHADED_CHARACTERS::SC_SOLID)); break;
	case  5: pixel.color = static_cast<COLOUR>(static_cast<WORD>(COLOUR::COL_BG_DARK_GREY) | static_cast<WORD>(COLOUR::COL_FG_GREY)); pixel.character = static_cast<SHADED_CHARACTERS>(static_cast<WCHAR>(SHADED_CHARACTERS::SC_QUARTER)); break;
	case  6: pixel.color = static_cast<COLOUR>(static_cast<WORD>(COLOUR::COL_BG_DARK_GREY) | static_cast<WORD>(COLOUR::COL_FG_GREY)); pixel.character = static_cast<SHADED_CHARACTERS>(static_cast<WCHAR>(SHADED_CHARACTERS::SC_HALF)); break;
	case  7: pixel.color = static_cast<COLOUR>(static_cast<WORD>(COLOUR::COL_BG_DARK_GREY) | static_cast<WORD>(COLOUR::COL_FG_GREY)); pixel.character = static_cast<SHADED_CHARACTERS>(static_cast<WCHAR>(SHADED_CHARACTERS::SC_THREEQUARTERS)); break;
	case  8: pixel.color = static_cast<COLOUR>(static_cast<WORD>(COLOUR::COL_BG_DARK_GREY) | static_cast<WORD>(COLOUR::COL_FG_GREY)); pixel.character = static_cast<SHADED_CHARACTERS>(static_cast<WCHAR>(SHADED_CHARACTERS::SC_SOLID)); break;
	case  9: pixel.color = static_cast<COLOUR>(static_cast<WORD>(COLOUR::COL_BG_GREY) | static_cast<WORD>(COLOUR::COL_FG_WHITE)); pixel.character = static_cast<SHADED_CHARACTERS>(static_cast<WCHAR>(SHADED_CHARACTERS::SC_QUARTER)); break;
	case 10: pixel.color = static_cast<COLOUR>(static_cast<WORD>(COLOUR::COL_BG_GREY) | static_cast<WORD>(COLOUR::COL_FG_WHITE)); pixel.character = static_cast<SHADED_CHARACTERS>(static_cast<WCHAR>(SHADED_CHARACTERS::SC_HALF)); break;
	case 11: pixel.color = static_cast<COLOUR>(static_cast<WORD>(COLOUR::COL_BG_GREY) | static_cast<WORD>(COLOUR::COL_FG_WHITE)); pixel.character = static_cast<SHADED_CHARACTERS>(static_cast<WCHAR>(SHADED_CHARACTERS::SC_THREEQUARTERS)); break;
	case 12: pixel.color = static_cast<COLOUR>(static_cast<WORD>(COLOUR::COL_BG_GREY) | static_cast<WORD>(COLOUR::COL_FG_WHITE)); pixel.character = static_cast<SHADED_CHARACTERS>(static_cast<WCHAR>(SHADED_CHARACTERS::SC_SOLID)); break;
	default:
		pixel.color = static_cast<COLOUR>(static_cast<WORD>(COLOUR::COL_BG_GREY) | static_cast<WORD>(COLOUR::COL_FG_WHITE)); pixel.character = static_cast<SHADED_CHARACTERS>(static_cast<WCHAR>(SHADED_CHARACTERS::SC_SOLID)); break;
	}

	return pixel;
}

class TestBed : public wcge::ConsoleGameEngine {
public:
	using ConsoleGameEngine::ConsoleGameEngine;

private:
	bool OnStart() override {
		SetApplicationName( L"TestBed" );

		m_pPipeline = new wcge::c3d::Pipeline( this );

		//m_m4ProjectionMatrix = Mat4MakeProjectionMatrix( GetBackBuffer().GetWidth(), GetBackBuffer().GetHeight(), 90.0f, 0.5f, 100.0f );
		m_pCamera = new r3d::Camera(
			static_cast<uint32_t>(GetBackBuffer().GetWidth()), 
			static_cast<uint32_t>(GetBackBuffer().GetHeight()), 
			90.0f, 
			1.0f, 
			500.0f
		);

		m_pCamera->SetMovementSpeed(25.0f);
		m_pCamera->SetRotationSpeed(3.0f);

		m_pCamera->SetPosition(Vector3(0.0f, 0.0f, -10.0f));

	




		//m_vObjects.push_back(new r3d::Object(
		//	m_pPipeline,
		//	r3d::CreateCubeVertexBuffer(3.0f),
		//	r3d::CreateCubeIndexBuffer(),
		//	L"res/texture.jpg"
		//));
		//m_vObjects[1]->GetPosition() = Vector3(0.0f, 200.0f, 0.0f);

		m_vObjects.push_back(new r3d::Object(
			m_pPipeline,
			r3d::CreateCubeVertexBuffer(1.0f),
			r3d::CreateCubeIndexBuffer(),
			L"res/texture.jpg"
		));		
		
		m_vObjects.push_back(new r3d::Object(
			m_pPipeline,
			r3d::CreateCubeVertexBuffer(1.0f),
			r3d::CreateCubeIndexBuffer(),
			L"res/texture_2.jpg"
		));

		m_vObjects.push_back(new r3d::Object(
			m_pPipeline,
			r3d::CreateCubeVertexBuffer(1.0f),
			r3d::CreateCubeIndexBuffer(),
			L"res/texture_3.jpg"
		));

		m_vObjects[1]->GetPosition() += Vector3(5.0f, 0.0f, 0.0f);
		m_vObjects[2]->GetPosition() += Vector3(-5.0f, 0.0f, 0.0f);

		m_vObjects.push_back(r3d::Object::LoadFromFile(
			m_pPipeline,
			"res/dragon.obj",
			L"res/dragon.png",
			false
		));

		m_vObjects[3]->GetScale() = Vector3(1.0f, 1.0f, 1.0f);
		m_vObjects[3]->GetPosition() = Vector3(0.0f, -50.0f, 0.0f);

		for (auto& object : m_vObjects) {
			object->SetCamera(m_pCamera);
		}

		return true;
	}

	bool OnUpdate( const float deltaTime ) override {

		float dt = deltaTime;

		if (m_bIsSpaceReleased) {
			m_bIsSpaceReleased = false;
		}

		if (GetKey(KEY::SPACE)) {
			m_bIsSpacePressed = true;
		}
		else {
			if (m_bIsSpacePressed) {
				m_bIsSpaceReleased = true;
				m_bIsSpacePressed = false;
			}
		}


		if (m_bIsSpaceReleased) {
			m_bFroze = !m_bFroze;
		}

		if (m_bFroze) {
			dt = 0.0f;
		}

		HandleMovement(dt);

		GetBackBuffer().Clear( CB(COLOUR::COL_FG_BLACK, SHADED_CHARACTERS::SC_SOLID));
		m_pPipeline->ClearDepthBuffer();

		for (auto object : m_vObjects) {
			object->Draw();
		}

		return true;
	}

	bool OnDestroy() override {

		for (auto& object : m_vObjects) {
			delete object;
		}
		delete m_pCamera;
		delete m_pPipeline;
		return true;
	}



	void HandleMovement(float dt) {
		if (GetKey(KEY::W)) {
			m_pCamera->MoveAlongZ(dt);
		}

		if (GetKey(KEY::S)) {
			m_pCamera->MoveAlongZ(-dt);
		}

		if (GetKey(KEY::A)) {
			m_pCamera->MoveAlongX(-dt);
		}

		if (GetKey(KEY::D)) {
			m_pCamera->MoveAlongX(dt);
		}

		if (GetKey(KEY::Q)) {
			m_pCamera->MoveAlongY(dt);
		}

		if (GetKey(KEY::E)) {
			m_pCamera->MoveAlongY(-dt);
		}

		if (GetKey(KEY::UP)) {
			m_pCamera->RotateAroundX(-dt);
		}

		if (GetKey(KEY::DOWN)) {
			m_pCamera->RotateAroundX(dt);
		}

		if (GetKey(KEY::LEFT)) {
			m_pCamera->RotateAroundY(-dt);
		}

		if (GetKey(KEY::RIGHT)) {
			m_pCamera->RotateAroundY(dt);
		}

		if (GetKey(KEY::Z)) {
			m_pCamera->RotateAroundZ(dt);
		}

		if (GetKey(KEY::X)) {
			m_pCamera->RotateAroundZ(-dt);
		}
	}

private:
	Pipeline* m_pPipeline = nullptr;

	bool m_bFroze = false;

	bool m_bIsSpacePressed = false;
	bool m_bIsSpaceReleased = false;

	r3d::Camera* m_pCamera;

	std::vector<r3d::Object*> m_vObjects;
};

int main() {
	TestBed(300, 150, 4, 4).Run();
}