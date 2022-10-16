#ifndef __GUBIOS_DRIVERS_KEYBOARD_H
#define __GUBIOS_DRIVERS_KEYBOARD_H

#include <hardwarecommunication/interrupts.h>
#include <common/types.h>
#include <hardwarecommunication/port.h>
#include <drivers/Driver.h>

namespace gubios
{
	namespace drivers
	{
		class KeyboardEventHandler
		{
		public:
			KeyboardEventHandler();
			virtual void onKeyDown(char);
			virtual void onKeyUp(char);

			};

			class KeyboardDriver : public gubios::hardwarecommunication::InterruptHandler,public Driver
			{
				gubios::hardwarecommunication::Port8Bit dataPort;
				gubios::hardwarecommunication::Port8Bit commandPort;
			public:
				KeyboardDriver(gubios::hardwarecommunication::InterruptManager* interruptManager, KeyboardEventHandler* handler);
				~KeyboardDriver();
				KeyboardEventHandler* handler;
				virtual uint32_t HandleInterrupt(uint32_t esp);
				virtual void Activate();

			};
	}
}
#endif
