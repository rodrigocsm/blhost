#pragma once
#include <wtypes.h>
#include "blfwk/FTD2XX.H"

namespace blfwk
{
	class D2xxDriver
	{
	public:
		D2xxDriver();
		~D2xxDriver();

		FT_STATUS Open(PVOID);
		FT_STATUS OpenEx(PVOID, DWORD);
		FT_STATUS ListDevices(PVOID, PVOID, DWORD);
		FT_STATUS Close();
		FT_STATUS Read(LPVOID, DWORD, LPDWORD);
		FT_STATUS Write(LPVOID, DWORD, LPDWORD);
		FT_STATUS Purge(ULONG);
		FT_STATUS ResetDevice();
		FT_STATUS SetTimeouts(ULONG, ULONG);
		FT_STATUS GetQueueStatus(LPDWORD);
		FT_STATUS SetBaudRate(ULONG);
	private:
		HMODULE m_hmodule;
		FT_HANDLE m_ftHandle;
		void LoadDLL();

		typedef FT_STATUS(WINAPI* PtrToOpen)(PVOID, FT_HANDLE*);
		PtrToOpen m_pOpen;


		typedef FT_STATUS(WINAPI* PtrToOpenEx)(PVOID, DWORD, FT_HANDLE*);
		PtrToOpenEx m_pOpenEx;

		typedef FT_STATUS(WINAPI* PtrToListDevices)(PVOID, PVOID, DWORD);
		PtrToListDevices m_pListDevices;

		typedef FT_STATUS(WINAPI* PtrToClose)(FT_HANDLE);
		PtrToClose m_pClose;

		typedef FT_STATUS(WINAPI* PtrToRead)(FT_HANDLE, LPVOID, DWORD, LPDWORD);
		PtrToRead m_pRead;

		typedef FT_STATUS(WINAPI* PtrToWrite)(FT_HANDLE, LPVOID, DWORD, LPDWORD);
		PtrToWrite m_pWrite;

		typedef FT_STATUS(WINAPI* PtrToResetDevice)(FT_HANDLE);
		PtrToResetDevice m_pResetDevice;

		typedef FT_STATUS(WINAPI* PtrToPurge)(FT_HANDLE, ULONG);
		PtrToPurge m_pPurge;

		typedef FT_STATUS(WINAPI* PtrToSetTimeouts)(FT_HANDLE, ULONG, ULONG);
		PtrToSetTimeouts m_pSetTimeouts;

		typedef FT_STATUS(WINAPI* PtrToGetQueueStatus)(FT_HANDLE, LPDWORD);
		PtrToGetQueueStatus m_pGetQueueStatus;

		typedef FT_STATUS(WINAPI* PtrToSetBaudRate)(FT_HANDLE, ULONG);
		PtrToSetBaudRate m_pSetBaudRate;

	};
}