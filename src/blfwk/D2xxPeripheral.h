#pragma once
#include "Peripheral.h"
#include "packet/command_packet.h"
#include "blfwk/FTD2XX.H"
#include "blfwk/D2xxDriver.h"

//! @addtogroup d2xx_peripheral
//! @{

namespace blfwk
{
	/*!
	* @brief Peripheral that talks to the target device over serial using FTDI d2xx driver.
	*/
	class D2xxPeripheral :
		public Peripheral
	{
    public:
        //! @breif Constants.
        enum _d2xx_peripheral_constants
        {
            // The read() implementation for the UartPeripheral does not use this the timeout parameter.
            kD2xxPeripheral_UnusedTimeout = 0,
            // Serial timeout is set to this default during init().
            kD2xxPeripheral_DefaultReadTimeoutMs = 1000,
            kD2xxPeripheral_DefaultWriteTimeoutMs = 1000,
            kD2xxPeripheral_DefaultBaudRate = 9600
        };

    public:
        //! @brief Parameterized constructor that opens the serial.
        //!
        //! Opens and configures the serial. Throws exception if serial configuration fails.
        //!
        //! Note: following serial configuration is assumed: 8 bits, 1 stop bit, no parity.
        //!
        //! @param description - Serial device description name
        //! @param speed serial speed, e.g. 9600.
        D2xxPeripheral(const char* description = "", long speed = kD2xxPeripheral_DefaultBaudRate);

        //! @brief Destructor.
        virtual ~D2xxPeripheral();

        //! @brief Flush.
        //!
        //! should be called on an open serial in order to flush any remaining data in the UART RX buffer
        void flushRX();

        //! @brief Read bytes.
        //!
        //! @param buffer Pointer to buffer.
        //! @param requestedBytes Number of bytes to read.
        //! @param actualBytes Number of bytes actually read.
        //! @param timeoutMs Time in milliseconds to wait for read to complete.
        virtual status_t read(uint8_t* buffer, uint32_t requestedBytes, uint32_t* actualBytes, uint32_t unused_timeoutMs);

        //! @brief Write bytes.
        //!
        //! @param buffer Pointer to buffer to write
        //! @param byteCount Number of bytes to write
        virtual status_t write(const uint8_t* buffer, uint32_t byteCount);

    protected:
        //! @brief Initialize.
        //!
        //! Opens and configures the port.
        //!
        //! Note: following COM port configuration is assumed: 8 bits, 1 stop bit, no parity.
        //!
        //! @param port OS file path for COM port. For example "COM1" on Windows.
        //! @param speed Port speed, e.g. 9600.
        bool init(const char* description, long speed);

        D2xxDriver* m_hDriver;
        uint8_t m_buffer[kDefaultMaxPacketSize]; //!< Buffer for bytes used to build read packet.
    };
}

//! @}

////////////////////////////////////////////////////////////////////////////////
// EOF
////////////////////////////////////////////////////////////////////////////////
