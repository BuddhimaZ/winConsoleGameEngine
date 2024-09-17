#include <wcgeConsoleGameEngine.hpp>

std::atomic_bool wcge::ConsoleGameEngine::m_bEngineRunning = false;
std::mutex wcge::ConsoleGameEngine::m_muxGame;
std::condition_variable wcge::ConsoleGameEngine::m_cvGameFinished;

wcge::ConsoleGameEngine::ConsoleGameEngine(
	const uint32_t w,
	const uint32_t h,
	const uint32_t fw,
	const uint32_t fh
) : m_nFontWidth(fw), m_nFontHeight(fh) {
	//Get the handle to the current console
	m_hConsole = GetStdHandle( STD_OUTPUT_HANDLE );

	// Set the cuurent screen buffer size according to the given screen sizes
	SetConsoleScreenBufferSize( m_hConsole, { (SHORT)w, (SHORT)h } );

	// Change the font information of the current console. Set the font size to given font width and height
	CONSOLE_FONT_INFOEX cfiex;
	cfiex.cbSize = sizeof( CONSOLE_FONT_INFOEX );
	cfiex.nFont = 0;
	cfiex.dwFontSize.X = (SHORT)m_nFontWidth;
	cfiex.dwFontSize.Y = (SHORT)m_nFontHeight;
	cfiex.FontFamily = FF_DONTCARE;
	cfiex.FontWeight = FW_NORMAL;
	wcscpy_s( cfiex.FaceName, L"Consolas" );

	SetCurrentConsoleFontEx( m_hConsole, FALSE, &cfiex );

	// Change the size of the current console window according to the given screen sizes and font sizes
	// I realy don't know why we have to substract 1 from both width and height of the screen size. But this is the only way it seem to be working.
	// If I set it to just screen width and height it won't work.
	SMALL_RECT rectConsoleSize = { 0, 0, (SHORT)(w - 1), (SHORT)(h - 1) };
	SetConsoleWindowInfo( m_hConsole, TRUE, &rectConsoleSize );

	//Allocate memory for back buffer
	m_pBackBuffer = new CBImage( w, h );

	//Allocate memory for front buffer
	m_pFrontBuffer = new CBImage( w, h );

	SetConsoleCtrlHandler( (PHANDLER_ROUTINE)CloseHandler, TRUE );
}

wcge::ConsoleGameEngine::~ConsoleGameEngine() {
	//Delete backbuffer before exiting
	if ( m_pBackBuffer ) delete m_pBackBuffer;
	if ( m_pFrontBuffer ) delete m_pFrontBuffer;
}

bool wcge::ConsoleGameEngine::GetKey( const KEY key ) const {
	return GetAsyncKeyState( static_cast<int>(key) ) & 0x8000;
}

BOOL wcge::ConsoleGameEngine::CloseHandler( DWORD evt ) {
	if ( evt == CTRL_CLOSE_EVENT ) {
		m_bEngineRunning = false;

		// Wait for thread to be exited
		std::unique_lock<std::mutex> ul( m_muxGame );
		m_cvGameFinished.wait( ul );

	}
	return true;
}

void wcge::ConsoleGameEngine::Run() {
	m_bEngineRunning = true;

	std::thread threadRender = std::thread( &ConsoleGameEngine::EngineThread, this );

	unsigned int nFrameCounter = 0;
	float fFrameTime = 0.0f;

	auto tp1 = std::chrono::steady_clock::now();
	auto tp2 = std::chrono::steady_clock::now();

	wchar_t sWindowTitle[256];
	swprintf_s( sWindowTitle, 256, L"%s | FPS: %d", m_sAppName.c_str(), 0 );
	SetConsoleTitleW( sWindowTitle );

	while ( m_bEngineRunning ) {

		auto tp2 = std::chrono::steady_clock::now();
		std::chrono::duration<float> elapsedTime = tp2 - tp1;
		tp1 = tp2;
		float fElapsedTime = elapsedTime.count();

		SMALL_RECT rectWriteRegion = { 0, 0, (SHORT)(m_pFrontBuffer->GetWidth() - 1), (SHORT)(m_pFrontBuffer->GetHeight() - 1) };
		
		std::unique_lock<std::mutex> lock( m_muxBufferSwap );
		m_cvBufferSwapped.wait( lock/*, [this]() { return m_bBufferSwappingDone; }*/);
		WriteConsoleOutputW(
			m_hConsole,
			reinterpret_cast<CHAR_INFO*>(m_pFrontBuffer->GetBufferPtr()),
			{ static_cast<SHORT>(rectWriteRegion.Right + 1), static_cast<SHORT>(rectWriteRegion.Bottom + 1) },
			{ 0,0 }, 
			&rectWriteRegion 
		);
		lock.unlock();

		nFrameCounter++;
		fFrameTime += fElapsedTime;

		if ( fFrameTime >= 1.0f ) {

			swprintf_s( sWindowTitle, 256, L"%s | FPS: %d", m_sAppName.c_str(), nFrameCounter );
			SetConsoleTitleW( sWindowTitle );

			nFrameCounter = 0;
			fFrameTime = 0.0f;
		}
	}

	threadRender.join();
	m_cvGameFinished.notify_one();
}

void wcge::ConsoleGameEngine::EngineThread() {
	std::chrono::steady_clock::time_point tp1 = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point tp2 = std::chrono::steady_clock::now();

	if ( !OnStart() ) {
		m_bEngineRunning = false;
	}

	while ( m_bEngineRunning ) {
		while ( m_bEngineRunning ) {

			tp2 = std::chrono::steady_clock::now();
			std::chrono::duration<float> EalapsedTime = tp2 - tp1;
			tp1 = tp2;
			float dt = EalapsedTime.count();

			if ( !OnUpdate( dt ) ) {
				m_bEngineRunning = false;
			}
			
			std::unique_lock<std::mutex> lock( m_muxBufferSwap );
			m_bBufferSwappingDone = false;
			//std::swap( m_pBackBuffer, m_pFrontBuffer );
			auto tmp = m_pFrontBuffer;
			m_pFrontBuffer = m_pBackBuffer;
			m_pBackBuffer = tmp;
			m_bBufferSwappingDone = true;
			lock.unlock();
			m_cvBufferSwapped.notify_one();

		}
		if ( !OnDestroy() ) {
			m_bEngineRunning = true;
		}
	}
}

void wcge::ConsoleGameEngine::DrawLine( uint32_t x1, uint32_t y1,
										uint32_t x2, uint32_t y2,
										const CB cb ) {
	if ( x1 == x2 && y1 == y2 ) {
		m_pBackBuffer->SetCB( x1, y1, cb );
	}
	else if ( x1 == x2 ) {
		if ( y1 > y2 ) std::swap( y1, y2 );
		for ( uint32_t y = y1; y <= y2; y++ ) m_pBackBuffer->SetCB( x1, y, cb );
	}
	else if ( y1 == y2 ) {
		if ( x1 > x2 ) std::swap( x1, x2 );
		for ( uint32_t x = x1; x <= x2; x++ ) m_pBackBuffer->SetCB( x, y1, cb );
	}
	else {
		if ( x1 > x2 ) {
			std::swap(x1, x2);
			std::swap(y1, y2);
		}

		uint32_t nDx = x2 - x1;
		int nDy = y2 - y1;

		if ( nDy > 0 ) {
			uint32_t nPrevY = y1;

			for ( uint32_t x = 0; x <= nDx; x++ ) {
				float fY = (float)x / (float)nDx;
				uint32_t y = (uint32_t)(fY * nDy) + y1;
				m_pBackBuffer->SetCB( x1 + x, y, cb );

				if ( y > (nPrevY + 1) ) {
					uint32_t _nDy = y - (nPrevY + 1);
					for ( uint32_t _y = 0; _y <= _nDy; _y++ ) {
						float _fX = (float)_y / (float)_nDy;
						uint32_t _x = (uint32_t)(_fX);
						m_pBackBuffer->SetCB( x1 + x + _x, _y + (nPrevY + 1), cb );
					}
				}

				nPrevY = y;
			}
		}
		else {
			uint32_t nPrevY = y1;

			for ( uint32_t x = 0; x <= nDx; x++ ) {
				float fY = (float)x / (float)nDx;
				uint32_t y = (uint32_t)(fY * nDy) + y1;
				m_pBackBuffer->SetCB( x1 + x, y, cb );

				if ( y < (nPrevY - 1) ) {
					uint32_t _nDy = (nPrevY - 1) - y;
					for ( uint32_t _y = 0; _y <= _nDy; _y++ ) {
						float _fX = (float)_y / (float)_nDy;
						uint32_t _x = (uint32_t)(_fX);
						m_pBackBuffer->SetCB( x1 + x - _x, y + _y, cb );
					}
				}

				nPrevY = y;
			}
		}

	}
}

void wcge::ConsoleGameEngine::DrawTriangle(
	uint32_t x1, uint32_t y1,
	uint32_t x2, uint32_t y2,
	uint32_t x3, uint32_t y3,
	const CB cb
) {
	DrawLine( x1, y1, x2, y2, cb );
	DrawLine( x2, y2, x3, y3, cb );
	DrawLine( x1, y1, x3, y3, cb );
}