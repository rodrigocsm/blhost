#include "blfwk/D2xxPeripheral.h"
#include "blfwk/Logging.h"
#include "blfwk/format_string.h"
#include "blfwk/FTD2XX.h"

using namespace blfwk;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

D2xxPeripheral::D2xxPeripheral(const char* description, long speed)
	: m_hDriver(NULL)
{
	if (!init(description, speed))
	{
		throw std::runtime_error(
			format_string("Error: D2xxPeripheral() cannot open device(%s), speed(%d bauds).", description, speed));
	}
}

D2xxPeripheral::~D2xxPeripheral()
{
	try
	{
		if (m_hDriver != NULL)
		{
			FT_STATUS status = m_hDriver->Close();
			if (status != FT_STATUS::FT_OK)
			{
				if (Log::getLogger()->getFilterLevel() == Logger::log_level_t::kDebug)
				{
					Log::debug("Error: cannot close device");
				}
			}
		}
	}
	catch (const std::exception& e)
	{
		if (Log::getLogger()->getFilterLevel() == Logger::log_level_t::kDebug)
		{
			Log::debug(e.what());
		}
	}
}

void D2xxPeripheral::flushRX()
{
	try
	{
		m_hDriver->Purge(FT_PURGE_RX);
	}
	catch (const std::exception& e)
	{
		throw std::runtime_error(
			format_string("Error: flushRX() - %s ", e.what()));
	}
}

bool D2xxPeripheral::init(const char* description, long baud)
{
	FT_STATUS status = FT_STATUS::FT_OK;
	assert(description);
	if (description == NULL)
		return false;
	try
	{
		m_hDriver = new D2xxDriver();
		status = m_hDriver->OpenEx((PVOID)(LPCTSTR)description, FT_OPEN_BY_DESCRIPTION);
		if (status != FT_STATUS::FT_OK)
			return false;
		status = m_hDriver->SetBaudRate(baud);
		if (status != FT_STATUS::FT_OK)
			return false;
		flushRX();
		status = m_hDriver->SetTimeouts(kD2xxPeripheral_DefaultReadTimeoutMs, kD2xxPeripheral_DefaultWriteTimeoutMs);
		if (status != FT_STATUS::FT_OK)
			return false;
		return true;
	}
	catch (const std::exception& e)
	{
		if (Log::getLogger()->getFilterLevel() == Logger::log_level_t::kDebug)
		{
			Log::debug(e.what());
		}
		return false;
	}
}

status_t D2xxPeripheral::read(uint8_t* buffer, uint32_t requestedBytes, uint32_t* actualBytes, uint32_t unused_timeoutMs)
{
	assert(buffer);

	if (!buffer)
		return kStatus_Fail;
	int count = -1;
	try
	{
		FT_STATUS status = m_hDriver->Read((LPVOID)buffer, requestedBytes, (LPDWORD)&count);
		if (status != FT_STATUS::FT_OK)
			return kStatus_Fail;
	}
	catch (const std::exception& e)
	{
		if (Log::getLogger()->getFilterLevel() == Logger::log_level_t::kDebug)
		{
			Log::debug(e.what());
		}
		return kStatus_Fail;
	}
	if (actualBytes)
	{
		*actualBytes = count;
	}

	if (Log::getLogger()->getFilterLevel() == Logger::log_level_t::kDebug2)
	{
		// Log bytes read in hex
		Log::debug2("<");
		for (int i = 0; i < (int)count; i++)
		{
			Log::debug2("%02x", buffer[i]);
			if (i != (count - 1))
			{
				Log::debug2(" ");
			}
		}
		Log::debug2(">\n");
	}

	if (count < (int)requestedBytes)
	{
		// Anything less than requestedBytes is a timeout error.
		return kStatus_Timeout;
	}

	return kStatus_Success;
}

status_t D2xxPeripheral::write(const uint8_t* buffer, uint32_t byteCount)
{
	assert(buffer);
	if (!buffer)
		return kStatus_Fail;

	if (Log::getLogger()->getFilterLevel() == Logger::log_level_t::kDebug2)
	{
		// Log bytes written in hex
		Log::debug2("[");
		for (int i = 0; i < (int)byteCount; i++)
		{
			Log::debug2("%02x", buffer[i]);
			if (i != (byteCount - 1))
			{
				Log::debug2(" ");
			}
		}
		Log::debug2("]\n");
	}
	uint32_t bytesWritten = 0;
	try
	{
		FT_STATUS status = m_hDriver->Write((LPVOID)buffer, byteCount, (LPDWORD)&bytesWritten);
		if (status != FT_STATUS::FT_OK)
			return kStatus_Fail;
	}
	catch (const std::exception& e)
	{
		if (Log::getLogger()->getFilterLevel() == Logger::log_level_t::kDebug)
		{
			Log::debug(e.what());
		}
		return kStatus_Fail;
	}
	if (bytesWritten < byteCount)
	{
		// Anything less than byteCount is a timeout error.
		return kStatus_Timeout;
	}
	return kStatus_Success;
}
