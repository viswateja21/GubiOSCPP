#include <common/types.h>
#include <gdt.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/pci.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/Driver.h>
#include <drivers/vga.h>
#include <MemoryManager.h>
#include <drivers/amd_am79973.h>
#include <multitasking.h>
#include <syscall.h>
#include <net/etherframe.h>
#include <net/arp.h>

using namespace gubios::common;
using namespace gubios::drivers;
using namespace gubios::hardwarecommunication;
using namespace gubios;
using namespace gubios::net;

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;

extern "C" void CallConstructor()
{
	for(constructor* i = &start_ctors; i != &end_ctors;i++)
	{
		(*i)();
	}
}


void printf(const char* str)
{
	static uint16_t* videoMemory = (uint16_t*)0xb8000;
	static uint16_t x=0,y=0;


	for(int i=0;str[i]!='\0';i++)
	{
		switch(str[i])
		{
		case '\n' :
				y++;
				x=0;
				break;
		default:
				videoMemory[(y * 80) + x] = (videoMemory[(y * 80 )+ x] & 0xff00) | str[i];
				x++;
				break;
		}

		if(x>=80)
		{
			x  = 0;
			y++;
		}

		if(y >= 25)
		{
			for(y = 0;y<25;y++)
			{
				for(x=0;x<80;x++)
				{
					videoMemory[(y * 80) + x] |= ' ';
				}
			}
			x = 0;
			y = 0;
		}
	}
}


void printfHex(uint8_t key)
{
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    printf(foo);
}
class PrintfKeyboardHandler : public KeyboardEventHandler
{
public:
	void onKeyDown(char c)
	{
		char *foo=" ";
		foo[0] = c;
		printf(foo);

	}
};


class MouseCursor : public MouseEventHandler
{
	int8_t x,y;
public:

	MouseCursor()
	{
		static uint16_t* videoMemory = (uint16_t*)0xb8000;
		x=40;
		y=12;
		videoMemory[80*12 + 40] = (videoMemory[80*12 + 40] & 0xF000) >> 4 |
										(videoMemory[80*12 + 40] & 0x0F00) << 4 |
										(videoMemory[80*12 + 40] & 0x00FF);
	}

	void onMouseMove(int xoffset,int yoffset)
	{

		static uint16_t* videoMemory = (uint16_t*)0xb8000;
		videoMemory[80*y + x] = (videoMemory[80*y + x] & 0xF000) >> 4 |
										(videoMemory[80*y + x] & 0x0F00) << 4 |
										(videoMemory[80*y + x] & 0x00FF);


			x += xoffset;
			if(x < 0 )
			{
				x = 0;
			}
			if(x >= 80)
			{
				x = 79;
			}

			y += yoffset;
			if (y < 0) {
				y = 0;
			}
			if (y >= 25) {
				y = 24;
			}

		videoMemory[80*y + x] = (videoMemory[80*y + x] & 0xF000) >> 4 |
									(videoMemory[80*y + x] & 0x0F00) << 4 |
									(videoMemory[80*y + x] & 0x00FF);
	}
};

void sysprintf(char* str)
{
	asm("int $0x80"::"a"(4),"b"(str));
	}

void TaskA()
{
	printf("TaskA Entered\n");
	while(true) sysprintf("Task A\n");
	}

void TaskB()
{
	printf("TaskB Entered\n");
	while(true) sysprintf("Task B\n");
}

extern "C" void EntryPoint(void* multiboot_structure,unsigned int magicNumber)
{
	printf("Hello Assholes! Welcome to AAAnother world\n");

	GlobalDescriptorTable gdt;
	size_t heapSize = 10*1024 * 1024;

	uint32_t* memupper = (uint32_t*)((size_t)multiboot_structure + 8);
	MemoryManager memoryManager(heapSize,(*memupper) * 1024 - heapSize - 10*1024);

	printf("heap: 0x");
	printfHex((heapSize >> 24) & 0xFF);
	printfHex((heapSize>> 16) & 0xFF);
	printfHex((heapSize>> 8) & 0xFF);
	printfHex((heapSize) & 0xFF);

//	void *allocated = memoryManager.malloc(1024);
//
//	printf("\nAllocated heap: 0x");
//	printfHex(((size_t)allocated>> 24) & 0xFF);
//	printfHex(((size_t)allocated>> 16) & 0xFF);
//	printfHex(((size_t)allocated>> 8) & 0xFF);
//	printfHex(((size_t)allocated) & 0xFF);
	printf("\n");

	printf("Testing second string \n");
	TaskManager taskManager;
	Task task1(&gdt,TaskA);
	Task task2(&gdt,TaskB);
	taskManager.AddTask(&task1);
	taskManager.AddTask(&task2);

	InterruptManager interruptManager(0x20,&gdt);
	SysCallHander sysCall(0x80,&interruptManager);
	MouseCursor mCursor;
	MouseDriver mDriver(&interruptManager,&mCursor);
	PrintfKeyboardHandler keyboardHandler;
	KeyboardDriver kDriver(&interruptManager,&keyboardHandler);

	Driver::DriverManager driverManager;
	driverManager.AddDriver(&mDriver);
	driverManager.AddDriver(&kDriver);

	PeripheralComponentInterfaceConroller PCIController;
	PCIController.selectDrivers(driverManager,&interruptManager);

	VideoGraphicsArray vga;

	driverManager.ActivateAll();
	uint8_t ip1=10, ip2=0, ip3=2, ip4=15;
	uint32_t ip_be = ((uint32_t)ip4 << 24)
					| ((uint32_t)ip3 << 16)
					| ((uint32_t)ip2 << 8)
					| ((uint32_t)ip1 << 0);

	uint8_t gip1=10, gip2=0, gip3=2, gip4=2;
	uint32_t gip_be = ((uint32_t)gip4 << 24)
					| ((uint32_t)gip3 << 16)
					| ((uint32_t)gip2 << 8)
					| ((uint32_t)gip1 << 0);

	amd_am79973* eth0 = (amd_am79973*)driverManager.drivers[2];



	//eth0->SetIPAddr(ip_be);
	//eth0->Send((uint8_t*)"Hello Network", 13);
	EtherFrameProvider etherFrameHandler(eth0);

	AddressResolutionProtocol arp(&etherFrameHandler);



	//etherFrameHandler.Send(0xFFFFFFFFFFFF, 0x0608, (uint8_t*)"FOOD", 4);
	interruptManager.ActivateInterrupts();
//	printf("\n\n\n");

	arp.Resolve(gip_be);

	printf("After Arp Resolve\n");
//	vga.setMode(320, 200, 8);
////	printf("After Interrupts \n");
//	for (uint16_t y = 0; y < 200; ++y) {
//		for (uint16_t x = 0; x < 320; ++x) {
//			 vga.putPixel(x, y, 0x00, 0x00, 0xA8);
//		}
//	}
	while(1);//So the program will not  end
}



