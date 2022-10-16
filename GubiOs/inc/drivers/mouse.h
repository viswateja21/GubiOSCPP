#ifndef __GUBIOS_DRIVERS_MOUSE_H
#define __GUBIOS_DRIVERS_MOUSE_H

#include <hardwarecommunication/interrupts.h>
#include <common/types.h>
#include <hardwarecommunication/port.h>
#include <drivers/Driver.h>

namespace gubios
{
	namespace drivers
	{
		class MouseEventHandler
		{
			gubios::common::int8_t x,y;
		public:
			MouseEventHandler();
			virtual void onMouseDown(gubios::common::uint8_t button);
			virtual void onMouseUp(gubios::common::uint8_t button);
			virtual void onMouseMove(int xoffset,int yoffset);
			virtual void onActivate();
			};

		class MouseDriver : public gubios::hardwarecommunication::InterruptHandler,public gubios::drivers::Driver
		{
			gubios::hardwarecommunication::Port8Bit dataPort;
			gubios::hardwarecommunication::Port8Bit commandPort;

			uint8_t buffer[3];
			uint8_t offset;
			uint8_t button;

			MouseEventHandler* handler;

		public:
			MouseDriver(gubios::hardwarecommunication::InterruptManager* interruptManager,MouseEventHandler* handler);
			~MouseDriver();
			virtual uint32_t HandleInterrupt(uint32_t esp);
			virtual void Activate();

		};
	}
}
#endif
