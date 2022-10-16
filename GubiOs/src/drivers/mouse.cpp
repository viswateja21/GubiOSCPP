#include <drivers/mouse.h>

using namespace gubios::common;
using namespace gubios::drivers;
using namespace gubios::hardwarecommunication;

void printf(const char*);

MouseDriver::MouseDriver(InterruptManager* interruptManager,MouseEventHandler* handler):InterruptHandler(0x2C, interruptManager),
		dataPort(0x60),commandPort(0x64)
{
	//printf("MouseDriver constructor Start\n");
	this->handler = handler;
//	printf("MouseDriver constructor Ended\n");
}

uint32_t MouseDriver::HandleInterrupt(uint32_t esp) {
	uint8_t key = commandPort.read();
	if(!(key & 0x20)|| 0 == handler)
	{
		return esp;
	}
	static int8_t x=40,y=12;
	buffer[offset] = dataPort.read();
	offset = (offset+1) % 3;
	if(offset == 0)
	{
		handler->onMouseMove(buffer[1],-buffer[2]);


		for(int i=0;i<3;i++)
		{
			if((buffer[0] & (0x01 << i)) != (button & (0x01 << i)))
			{

				if(button & (0x01 << i))
				{
					handler->onMouseUp(i+1);
				}
				else
				{
					handler->onMouseDown(i+1);
				}
			}
		}
		button = buffer[0];

	}



	return esp;
}
 void MouseDriver::Activate()
 {
		offset = 0;
		button = 0;




		commandPort.write(0xA8);  // Activate Interrupts
		commandPort.write(0x20);  // get Current State
		uint8_t status = (dataPort.read() | 2 ) ;
		commandPort.write(0x60);
		dataPort.write(status);
		commandPort.write(0xD4);
		dataPort.write(0xF4);
		dataPort.read();
 }

MouseDriver::~MouseDriver()
{
	}


void MouseEventHandler::onMouseDown(uint8_t button)
{

	}

void MouseEventHandler::onMouseUp(uint8_t button)
{
	}

void MouseEventHandler::onMouseMove(int xoffset,int yoffset)
{
	}

MouseEventHandler::MouseEventHandler()
{

	}

void MouseEventHandler::onActivate()
{
	}
