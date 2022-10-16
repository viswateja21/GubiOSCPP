/*
 * pci.cpp
 *
 *  Created on: 11-Nov-2021
 *      Author: teja
 */

#include <hardwarecommunication/pci.h>

using namespace gubios::hardwarecommunication;
using namespace gubios::common;

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

void PeripheralComponentInterfaceConroller::selectDrivers(drivers::Driver::DriverManager dManager)
{
	int noOFDevices=0;
	for(int bus=0; bus < 1; bus++)
	{
		for (int device = 0; device < 32; device++)
		{
			noOFDevices = DeviceHasFunctions(bus, device)?8 : 1;
			for(int funcion = 0; funcion < noOFDevices; funcion++)
			{
				PeripheralComponentInterfaceConrollerDescriptor des = GetDescriptor(bus, device, funcion);
				if(0x0000 == des.deviceId || 0xFFFF == des.deviceId)
				{
					break;
				}

				printf("PCI BUS ");
				printfHex(bus & 0xFF);

				printf(", Device ");
				printfHex(device & 0xFF);

				printf(", Function ");
				printfHex(funcion & 0xFF);

				printf(", VendorId ");
				printfHex((des.vendorId & 0xFF00) >> 8);
				printfHex((des.vendorId & 0xFF));

				printf(", DeviceId ");
				printfHex((des.deviceId & 0xFF00) >> 8);
				printfHex((des.deviceId & 0xFF));


				printf("\n");


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



}
}
