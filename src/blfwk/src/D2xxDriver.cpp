
#include "blfwk/D2xxDriver.h"
#include "blfwk/format_string.h"

using namespace std;
using namespace blfwk;

D2xxDriver::D2xxDriver()
{
	try
	{
		LoadDLL();
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(format_string("Error: Cannot initialize D2xx driver: %s", e.what()));
	}
}

D2xxDriver::~D2xxDriver()
{
	if (m_hmodule != NULL)
		FreeLibrary(m_hmodule);
}

void D2xxDriver::LoadDLL()
{
	m_hmodule = LoadLibrary("Ftd2xx.dll");
	if (m_hmodule == NULL)
	{
		throw std::runtime_error(
			format_string("Error: Cannot load Ftd2xx.dll"));
	}
	m_pWrite = (PtrToWrite)GetProcAddress(m_hmodule, "FT_Write");
	if (m_pWrite == NULL)
	{
		throw std::runtime_error(format_string("Error: Can't Find FT_Write"));
		return;
	}

	m_pRead = (PtrToRead)GetProcAddress(m_hmodule, "FT_Read");
	if (m_pRead == NULL)
	{
		throw std::runtime_error(format_string("Error: Can't Find FT_Read"));
		return;
	}

	m_pOpen = (PtrToOpen)GetProcAddress(m_hmodule, "FT_Open");
	if (m_pOpen == NULL)
	{
		throw std::runtime_error(format_string("Error: Can't Find FT_Open"));
		return;
	}

	m_pOpenEx = (PtrToOpenEx)GetProcAddress(m_hmodule, "FT_OpenEx");
	if (m_pOpenEx == NULL)
	{
		throw std::runtime_error(format_string("Error: Can't Find FT_OpenEx"));
		return;
	}

	m_pListDevices = (PtrToListDevices)GetProcAddress(m_hmodule, "FT_ListDevices");
	if (m_pListDevices == NULL)
	{
		throw std::runtime_error(format_string("Error: Can't Find FT_ListDevices"));
		return;
	}

	m_pClose = (PtrToClose)GetProcAddress(m_hmodule, "FT_Close");
	if (m_pClose == NULL)
	{
		throw std::runtime_error(format_string("Error: Can't Find FT_Close"));
		return;
	}

	m_pResetDevice = (PtrToResetDevice)GetProcAddress(m_hmodule, "FT_ResetDevice");
	if (m_pResetDevice == NULL)
	{
		throw std::runtime_error(format_string("Error: Can't Find FT_ResetDevice"));
		return;
	}

	m_pPurge = (PtrToPurge)GetProcAddress(m_hmodule, "FT_Purge");
	if (m_pPurge == NULL)
	{
		throw std::runtime_error(format_string("Error: Can't Find FT_Purge"));
		return;
	}

	m_pSetTimeouts = (PtrToSetTimeouts)GetProcAddress(m_hmodule, "FT_SetTimeouts");
	if (m_pSetTimeouts == NULL)
	{
		throw std::runtime_error(format_string("Error: Can't Find FT_SetTimeouts"));
		return;
	}

	m_pGetQueueStatus = (PtrToGetQueueStatus)GetProcAddress(m_hmodule, "FT_GetQueueStatus");
	if (m_pGetQueueStatus == NULL)
	{
		throw std::runtime_error(format_string("Error: Can't Find FT_GetQueueStatus"));
		return;
	}

	m_pSetBaudRate = (PtrToSetBaudRate)GetProcAddress(m_hmodule, "FT_SetBaudRate");
	if (m_pSetBaudRate == NULL)
	{
		throw std::runtime_error(format_string("Error: Can't Find FT_SetBaudRate"));
		return;
	}
}

FT_STATUS D2xxDriver::Read(LPVOID lpvBuffer, DWORD dwBuffSize, LPDWORD lpdwBytesRead)
{
	if (!m_pRead)
	{
		throw std::runtime_error(format_string("FT_Read is not valid!"));
		return FT_STATUS::FT_INVALID_HANDLE;
	}

	return (*m_pRead)(m_ftHandle, lpvBuffer, dwBuffSize, lpdwBytesRead);
}

FT_STATUS D2xxDriver::Write(LPVOID lpvBuffer, DWORD dwBuffSize, LPDWORD lpdwBytes)
{
	if (!m_pWrite)
	{
		throw std::runtime_error(format_string("FT_Write is not valid!"));
		return FT_STATUS::FT_INVALID_HANDLE;
	}

	return (*m_pWrite)(m_ftHandle, lpvBuffer, dwBuffSize, lpdwBytes);
}

FT_STATUS D2xxDriver::Open(PVOID pvDevice)
{
	if (!m_pOpen)
	{
		throw std::runtime_error(format_string("FT_Open is not valid!"));
		return FT_STATUS::FT_INVALID_HANDLE;
	}

	return (*m_pOpen)(pvDevice, &m_ftHandle);
}

FT_STATUS D2xxDriver::OpenEx(PVOID pArg1, DWORD dwFlags)
{
	if (!m_pOpenEx)
	{
		throw std::runtime_error(format_string("FT_OpenEx is not valid!"));
		return FT_STATUS::FT_INVALID_HANDLE;
	}

	return (*m_pOpenEx)(pArg1, dwFlags, &m_ftHandle);
}

FT_STATUS D2xxDriver::ListDevices(PVOID pArg1, PVOID pArg2, DWORD dwFlags)
{
	if (!m_pListDevices)
	{
		throw std::runtime_error(format_string("FT_ListDevices is not valid!"));
		return FT_STATUS::FT_INVALID_HANDLE;
	}

	return (*m_pListDevices)(pArg1, pArg2, dwFlags);
}

FT_STATUS D2xxDriver::Close()
{
	if (!m_pClose)
	{
		throw std::runtime_error(format_string("FT_Close is not valid!"));
		return FT_STATUS::FT_INVALID_HANDLE;
	}

	return (*m_pClose)(m_ftHandle);
}

FT_STATUS D2xxDriver::ResetDevice()
{
	if (!m_pResetDevice)
	{
		throw std::runtime_error(format_string("FT_ResetDevice is not valid!"));
		return FT_STATUS::FT_INVALID_HANDLE;
	}

	return (*m_pResetDevice)(m_ftHandle);
}

FT_STATUS D2xxDriver::Purge(ULONG dwMask)
{
	if (!m_pPurge)
	{
		throw std::runtime_error(format_string("FT_Purge is not valid!"));
		return FT_STATUS::FT_INVALID_HANDLE;
	}

	return (*m_pPurge)(m_ftHandle, dwMask);
}

FT_STATUS D2xxDriver::SetTimeouts(ULONG dwReadTimeout, ULONG dwWriteTimeout)
{
	if (!m_pSetTimeouts)
	{
		throw std::runtime_error(format_string("FT_SetTimeouts is not valid!"));
		return FT_STATUS::FT_INVALID_HANDLE;
	}

	return (*m_pSetTimeouts)(m_ftHandle, dwReadTimeout, dwWriteTimeout);
}

FT_STATUS D2xxDriver::GetQueueStatus(LPDWORD lpdwAmountInRxQueue)
{
	if (!m_pGetQueueStatus)
	{
		throw std::runtime_error(format_string("FT_GetQueueStatus is not valid!"));
		return FT_STATUS::FT_INVALID_HANDLE;
	}

	return (*m_pGetQueueStatus)(m_ftHandle, lpdwAmountInRxQueue);
}

FT_STATUS D2xxDriver::SetBaudRate(ULONG dwBaudRate)
{
	if (!m_pSetBaudRate)
	{
		throw std::runtime_error(format_string("FT_SetBaudRate is not valid!"));
		return FT_STATUS::FT_INVALID_HANDLE;
	}

	return (*m_pSetBaudRate)(m_ftHandle, dwBaudRate);
}