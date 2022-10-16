#ifndef __GUBIOS_HARDWARECOMMUNICATION_INTERRUPTS_H
#define __GUBIOS_HARDWARECOMMUNICATION_INTERRUPTS_H

#include <common/types.h>
#include <gdt.h>
#include <hardwarecommunication/port.h>

namespace gubios
{
	namespace hardwarecommunication
	{

		class InterruptManager;

		class InterruptHandler
		{
		protected:
			gubios::common::uint8_t InterruptNumber;
			InterruptManager* interruptManager;

			InterruptHandler(gubios::common::uint8_t interruptNumber, InterruptManager* interruptManager);
			~InterruptHandler();
		public:
			virtual gubios::common::uint32_t HandleInterrupt(gubios::common::uint32_t esp);

			};


			class InterruptManager
			{
				friend class InterruptHandler;
				uint16_t hardwareOffset;
				protected:

					gubios::common::uint16_t hardwareInterruptOffset;
					static InterruptManager* activeInterruptManager;
					InterruptHandler* handlers[256];

					struct GateDescriptor
					{
						gubios::common::uint16_t handlerAddressLowBits;
						gubios::common::uint16_t gdt_table_selector;
						gubios::common::uint8_t reserved;
						gubios::common::uint8_t access;
						gubios::common::uint16_t handlerAddressHighBits;
					}__attribute__((packed));

					struct IDTPtr
					{
						gubios::common::uint16_t size;
						gubios::common::uint32_t base;
					}__attribute__((packed));

					static GateDescriptor interruptdesctable[256];

					static void setIDTEntry(
							gubios::common::uint8_t interruptNumber,
							gubios::common::uint16_t gdt_table_selector,
							void (*handle)(),
							gubios::common::uint8_t DescriptorPrevilage,
							gubios::common::uint8_t DescriptorType
							);
					Port8BitSlow masterCommand;
					Port8BitSlow masterData;
					Port8BitSlow slaveCommand;
					Port8BitSlow slaveData;

				public:
					InterruptManager(gubios::common::uint16_t hardwareInterruptOffset,GlobalDescriptorTable *gdt,uint16_t HardWareOffset=0x20);
					~InterruptManager();
					static gubios::common::uint32_t handleInterrupt(gubios::common::uint8_t interruptNumber,gubios::common::uint32_t esp);
					gubios::common::uint32_t doHandleInterrupt(gubios::common::uint8_t interruptNumber,gubios::common::uint32_t esp);
					void ActivateInterrupts();
					void Deactivate();

	                static void HandleInterruptRequest0x00();
	                static void HandleInterruptRequest0x01();
	                static void HandleInterruptRequest0x02();
	                static void HandleInterruptRequest0x03();
	                static void HandleInterruptRequest0x04();
	                static void HandleInterruptRequest0x05();
	                static void HandleInterruptRequest0x06();
	                static void HandleInterruptRequest0x07();
	                static void HandleInterruptRequest0x08();
	                static void HandleInterruptRequest0x09();
	                static void HandleInterruptRequest0x0A();
	                static void HandleInterruptRequest0x0B();
	                static void HandleInterruptRequest0x0C();
	                static void HandleInterruptRequest0x0D();
	                static void HandleInterruptRequest0x0E();
	                static void HandleInterruptRequest0x0F();
	                static void HandleInterruptRequest0x31();
	                static void HandleInterruptRequest0x80();

	                              static void HandleException0x00();
	                               static void HandleException0x01();
	                               static void HandleException0x02();
	                               static void HandleException0x03();
	                               static void HandleException0x04();
	                               static void HandleException0x05();
	                               static void HandleException0x06();
	                               static void HandleException0x07();
	                               static void HandleException0x08();
	                               static void HandleException0x09();
	                               static void HandleException0x0A();
	                               static void HandleException0x0B();
	                               static void HandleException0x0C();
	                               static void HandleException0x0D();
	                               static void HandleException0x0E();
	                               static void HandleException0x0F();
	                               static void HandleException0x10();
	                               static void HandleException0x11();
	                               static void HandleException0x12();
	                               static void HandleException0x13();


					static void InterruptIgnore();
					uint16_t getHardwareOffset();

				};
	}
}
#endif
