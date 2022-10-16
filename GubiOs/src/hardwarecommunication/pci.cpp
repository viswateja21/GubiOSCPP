/*
 * pci.cpp
 *
 *  Created on: 11-Nov-2021
 *      Author: teja
 */

#include <hardwarecommunication/pci.h>
#include<drivers/amd_am79973.h>

using namespace gubios::hardwarecommunication;
using namespace gubios::common;
using namespace gubios::drivers;

void printf(const char* str);
void printfHex(uint8_t key);

 namespace gubios
 {
 	 namespace hardwarecommunication
	 {

PeripheralComponentInterfaceConroller::PeripheralComponentInterfaceConroller():dataport(0xCFC),commandPort(0xCF8)
{


	}
PeripheralComponentInterfaceConroller::~PeripheralComponentInterfaceConroller()
{

}

PeripheralComponentInterfaceConrollerDescriptor::PeripheralComponentInterfaceConrollerDescriptor()
{
	}

PeripheralComponentInterfaceConrollerDescriptor::~PeripheralComponentInterfaceConrollerDescriptor()
{

}

uint32_t PeripheralComponentInterfaceConroller::Read(uint16_t bus,uint16_t device,uint16_t funcion,uint32_t regOffset)
{

	uint32_t id =
			(0x1 << 31) |
			((bus & 0xFF) << 16) |
			((device & 0x1F) << 11) |
			((funcion & 0x07) << 8) |
			((regOffset & 0xFC));
	commandPort.write(id);
	uint32_t result = dataport.read();


	return result >> (8 * (regOffset % 4));
}

void PeripheralComponentInterfaceConroller::Write(uint16_t bus,uint16_t device,uint16_t funcion,uint32_t regOffset,uint32_t value)
{

	uint32_t id =
			(0x1 << 31) |
			((bus & 0xFF) << 16) |
			((device & 0x1F) << 11) |
			((funcion & 0x07) << 8) |
			((regOffset & 0xFC));
	commandPort.write(id);
	dataport.write(value);

}

bool PeripheralComponentInterfaceConroller::DeviceHasFunctions(uint16_t bus,uint16_t device)
{
	return Read(bus, device, 0, 0x0E) & (1 << 7);
}

void PeripheralComponentInterfaceConroller::selectDrivers(drivers::Driver::DriverManager& dManager,hardwarecommunication::InterruptManager *interruptManager)
{
	int noOFDevices=0;
	for(int bus=0; bus < 8; bus++)
	{
		for (int device = 0; device < 32; device++)
		{
			noOFDevices = DeviceHasFunctions(bus, device)?8 : 1;
			for(int funcion = 0; funcion < noOFDevices; funcion++)
			{
				PeripheralComponentInterfaceConrollerDescriptor des = GetDescriptor(bus, device, funcion);
				if(0x0000 == des.vendorId || 0xFFFF == des.vendorId)
				{
					continue;
				}

				for(uint8_t bar=0;bar<6;bar++)
				{
					BaseAddressRegister baseAddrReg = GetBaseAddressRegister(bus, device, funcion, bar);
					if (baseAddrReg.address && (baseAddrReg.type ==InputOutput))
					{
					      des.portBase = (uint32_t)baseAddrReg.address;
					}

				}

				drivers::Driver* driver = GetDriver(des, interruptManager);
				if(driver)
				{
					if(des.deviceId == 0x2000)
					{
						printf("Network Drived is added\n");
					}
					dManager.AddDriver(driver);
				}
				else
				{
					if(des.deviceId == 0x2000)
					{
						printf("Network Drived is not added\n");
					}
				}
//				printf("PCI BUS ");
//				printfHex(bus & 0xFF);
//
//				printf(", Device ");
//				printfHex(device & 0xFF);
//
//				printf(", Function ");
//				printfHex(funcion & 0xFF);
//
//				printf(", VendorId ");
//				printfHex((des.vendorId & 0xFF00) >> 8);
//				printfHex((des.vendorId & 0xFF));
//
//				printf(", DeviceId ");
//				printfHex((des.deviceId & 0xFF00) >> 8);
//				printfHex((des.deviceId & 0xFF));
//
//
//				printf("\n");


			}
		}
	}

}

PeripheralComponentInterfaceConrollerDescriptor
PeripheralComponentInterfaceConroller::GetDescriptor(common::uint16_t bus,common::uint16_t device,common::uint16_t funcion)
{

	PeripheralComponentInterfaceConrollerDescriptor descriptor;

	descriptor.bus = bus;
	descriptor.device = device;
	descriptor.function = funcion;

	descriptor.deviceId = Read(bus, device, funcion, 0x02);
	descriptor.vendorId = Read(bus, device, funcion, 0x00);

	descriptor.classId = Read(bus, device, funcion, 0x0b);
	descriptor.subclasssId = Read(bus, device, funcion, 0x0a);
	descriptor.interaceId = Read(bus, device, funcion, 0x09);

	descriptor.revision = Read(bus, device, funcion, 0x08);
	descriptor.interrupt = Read(bus, device, funcion, 0x3c);

	return descriptor;


	}


BaseAddressRegister PeripheralComponentInterfaceConroller::GetBaseAddressRegister(common::uint16_t bus,common::uint16_t device,common::uint16_t funcion,common::uint16_t bar)
{
	BaseAddressRegister result;
	uint32_t header = Read(bus, device, funcion, 0x0E) & 0x7F;
	uint8_t maxBar = 6 - (header * 4);
	if(bar >= maxBar)
	{
		return result;
	}

	uint32_t bar_result = Read(bus, device, funcion, 0x10 + (4 * bar));
    result.type = (bar_result & 0X01) ? InputOutput:MemoryMapped;

    if(MemoryMapped == result.type)
    {
    }
    else
    {
      result.address = ((uint8_t*)(bar_result & ((uint32_t)~0x03)));
    }

	return result;
}

drivers::Driver* PeripheralComponentInterfaceConroller::GetDriver(PeripheralComponentInterfaceConrollerDescriptor descriptor,InterruptManager* Manager)
{
	//printf("GetDriver Enter\n");
	drivers::Driver* driver = 0;
	switch(descriptor.vendorId )
	{

		case 0x1022:
			switch(descriptor.deviceId)
			{
			printf("GetDriver SWITCH Enter\n");
			case 0x2000: //amd network driver
				printf("GetDriver NETWORK Enter\n");
				if(MemoryManager::activeMemoryManager == 0)
				{
					printf("GetDriver NETWORK activeMemoryManager is NULL\n");
				}
				driver = (Driver*)MemoryManager::activeMemoryManager->malloc(sizeof(amd_am79973));
				printf("\nAllocated heap: 0x");
					printfHex(((size_t)driver>> 24) & 0xFF);
					printfHex(((size_t)driver>> 16) & 0xFF);
					printfHex(((size_t)driver>> 8) & 0xFF);
					printfHex(((size_t)driver) & 0xFF);
				if(0 != driver)
				{
					printf("GetDriver NETWORK Enter driver NOT NULL\n");

					new (driver)amd_am79973(&descriptor,Manager);
					return driver;
				}
				else
				{
					printf("GetDriver NETWORK Enter driver NULL\n");
				}

				break;
			}
			break;
		case 0x8086:

			break;
	}

	switch(descriptor.classId)
		{
		case 0x03:
			switch(descriptor.subclasssId)
			{
			case 0x00:
				break;
			}
		}
    return 0;
}

}
}
