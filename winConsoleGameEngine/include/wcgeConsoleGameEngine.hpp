#pragma once

#include "wcgeWindowsHeader.hpp"
#include "wcgeCBImage.hpp"
#include "wcgeKeys.hpp"

#include <cstdint>
#include <string>
#include <mutex>

namespace wcge {
	class ConsoleGameEngine {
	public:
		ConsoleGameEngine(
			const uint32_t w = 300,
			const uint32_t h = 150,
			const uint32_t fw = 4,
			const uint32_t fh = 4
		);

		~ConsoleGameEngine();

		void Run();

	public:
		virtual bool OnStart() = 0;
		virtual bool OnUpdate( const float dt ) = 0;
		virtual bool OnDestroy() = 0;

		inline CBImage& GetBackBuffer() { return *m_pBackBuffer; }
		inline void SetApplicationName( const std::wstring& name ) { m_sAppName = name; }

		bool GetKey( const KEY key ) const;

		void DrawLine(
			uint32_t x1, uint32_t y1,
			uint32_t x2, uint32_t y2,
			const CB cb = CB( COLOUR::COL_BG_BLACK | COLOUR::COL_FG_WHITE, SHADED_CHARACTERS::SC_SOLID )
		);

		void DrawTriangle(
			uint32_t x1, uint32_t y1,
			uint32_t x2, uint32_t y2,
			uint32_t x3, uint32_t y3,
			const CB cb = CB( COLOUR::COL_BG_BLACK | COLOUR::COL_FG_WHITE, SHADED_CHARACTERS::SC_SOLID )
		);

	private:
		static BOOL CloseHandler( DWORD evt );

	private:
		void EngineThread();

		std::wstring m_sAppName = L"Console Game Engine Application";

		const uint32_t m_nFontWidth;
		const uint32_t m_nFontHeight;

		CBImage* m_pBackBuffer = nullptr;
		CBImage* m_pFrontBuffer = nullptr;

		HANDLE m_hConsole = nullptr;

		std::mutex m_muxBufferSwap;
		std::condition_variable m_cvBufferSwapped;
		bool m_bBufferSwappingDone = false;


		static std::atomic_bool m_bEngineRunning;
		static std::mutex m_muxGame;
		static std::condition_variable m_cvGameFinished;
	};
}