/*
 * pci.h
 *
 *  Created on: 11-Nov-2021
 *      Author: teja
 */

#ifndef GUBIOS_HARDWARECOMMUNICATION_PCI_H_
#define GUBIOS_HARDWARECOMMUNICATION_PCI_H_

#include <common/types.h>
#include <hardwarecommunication/interrupts.h>
#include <hardwarecommunication/port.h>
#include <drivers/Driver.h>

namespace gubios
{

namespace hardwarecommunication
{


class PeripheralComponentInterfaceConrollerDescriptor
{
public:
	common::uint32_t portBase;
	common::uint32_t interrupt;

	common::uint16_t bus;
	common::uint16_t device;
	common::uint16_t function;

	common::uint16_t deviceId;
	common::uint16_t vendorId;

	common::uint8_t classId;
	common::uint8_t subclasssId;
	common::uint8_t interaceId;

	common::uint8_t revision;

	PeripheralComponentInterfaceConrollerDescriptor();
	~PeripheralComponentInterfaceConrollerDescriptor();



};
class PeripheralComponentInterfaceConroller
{

	Port32Bit dataport;
	Port32Bit commandPort;
public:
	PeripheralComponentInterfaceConroller();
	~PeripheralComponentInterfaceConroller();

	common::uint32_t Read(common::uint16_t bus,common::uint16_t device,common::uint16_t funcion,common::uint32_t regOffset);

	void Write(common::uint16_t bus,common::uint16_t device,common::uint16_t funcion,common::uint32_t regOffset,common::uint32_t value);


	bool DeviceHasFunctions(common::uint16_t bus,common::uint16_t device);

	void selectDrivers(drivers::Driver::DriverManager &dManager);
	PeripheralComponentInterfaceConrollerDescriptor GetDescriptor(common::uint16_t bus,common::uint16_t device,common::uint16_t funcion);
};
}
}
#endif /* GUBIOS_HARDWARECOMMUNICATION_PCI_H_ */
