/*
 * amd_am79973.h
 *
 *  Created on: 16-Feb-2022
 *      Author: teja
 */

#ifndef INC_DRIVERS_AMD_AM79973_H_
#define INC_DRIVERS_AMD_AM79973_H_

#include<hardwarecommunication/interrupts.h>
#include<hardwarecommunication/port.h>
#include<hardwarecommunication/pci.h>
#include<common/types.h>
#include<drivers/Driver.h>


using namespace gubios::hardwarecommunication;
using namespace gubios::common;
using namespace gubios::drivers;

namespace gubios
{
	namespace drivers
	{
		class amd_am79973;

		class RawDataHandler
		{
		protected:
			amd_am79973* backend;
		public:
			RawDataHandler(amd_am79973* backend);
			~RawDataHandler();
			virtual bool onRawDataReceived(uint8_t* buffer,size_t size);
			void Send(uint8_t* buffer,size_t size);
		};

	//	class PeripheralComponentInterfaceConrollerDescriptor;
		class amd_am79973:public Driver, public InterruptHandler
		{
			struct InitializationBlock
			{
				uint16_t mode;
				unsigned reserved1 :4;
				unsigned numSendBuffers : 4;
				unsigned reserved2 : 4;
				unsigned numRecvBuffers : 4;
				uint64_t physicalAddress : 48;
				uint16_t reserved3;
				uint32_t logicalAddress;
				uint32_t recvBufferDescAddr;
				uint32_t sendBufferDescAddr;

			}__attribute__((packed));

			struct BufferDescriptor
			{
				uint32_t address;
				uint32_t flags;
				uint32_t flags2;
				uint32_t avail;
			}__attribute__((packed));

			Port16Bit MACAddress0port;
			Port16Bit MACAddress1port;
			Port16Bit MACAddress2port;
			Port16Bit registerDataPort;
			Port16Bit registerAddressPort;
			Port16Bit resetPort;
			Port16Bit busControlRgisterDataPort;

			InitializationBlock initBlock;

			BufferDescriptor* sendBufferDesc;
			uint8_t sendBufferDescMemory[2048 + 15];
			uint8_t sendBuffers[2 * 1024 + 15][8];
			uint8_t currentSendBuffer;

			BufferDescriptor* recvBufferDesc;
			uint8_t recvBufferDescMemory[2048 + 15];
			uint8_t recvBuffers[2 * 1024 + 15][8];
			uint8_t currenRecvBuffer;

			RawDataHandler* rawDataHandler;
		public:
			amd_am79973(PeripheralComponentInterfaceConrollerDescriptor *pciDescriptor
					,InterruptManager* inteerruptManager);
			~amd_am79973();
			 void Activate();
			 int Reset();
			uint32_t HandleInterrupt(uint32_t esp);
			void Send(uint8_t* buffer,int count);
			void Receive();
			void setHandler(RawDataHandler* rawDataHandler);
			common::uint64_t GetMACAddr();
			common::uint32_t GetIPAddr();
			void SetIPAddr(common::uint32_t ip_BE);
		};
	}
}

#endif /* INC_DRIVERS_AMD_AM79973_H_ */
