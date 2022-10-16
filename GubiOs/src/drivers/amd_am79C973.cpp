#include<drivers/amd_am79973.h>

extern void printf(const char*);
extern void printfHex(uint8_t key);


RawDataHandler::RawDataHandler(amd_am79973* backend)
{
	this->backend = backend;
	backend->setHandler(this);
}
RawDataHandler::~RawDataHandler()
{
	backend->setHandler(0);
}

bool RawDataHandler::onRawDataReceived(uint8_t* buffer,size_t size)
{
	return false;
}
void RawDataHandler::Send(uint8_t* buffer,size_t size)
{
	backend->Send(buffer, size);
}

amd_am79973::amd_am79973(PeripheralComponentInterfaceConrollerDescriptor *pciDescriptor
		,InterruptManager* inteerruptManager):Driver(),
		InterruptHandler(pciDescriptor->interrupt + inteerruptManager->getHardwareOffset(), inteerruptManager),
		MACAddress0port(pciDescriptor->portBase ),
		MACAddress1port(pciDescriptor->portBase + 0x02),
		MACAddress2port(pciDescriptor->portBase + 0x04),
		registerDataPort(pciDescriptor->portBase + 0x10),
		registerAddressPort(pciDescriptor->portBase + 0x12),
		resetPort(pciDescriptor->portBase + 0x14),
		busControlRgisterDataPort(pciDescriptor->portBase + 0x16)
{
	this->rawDataHandler = 0;
	currenRecvBuffer = currentSendBuffer = 0;
	uint64_t MAC0 = MACAddress0port.read() % 256 ;
	uint64_t MAC1 = MACAddress0port.read() / 256 ;
	uint64_t MAC2 = MACAddress1port.read() % 256 ;
	uint64_t MAC3 = MACAddress1port.read() / 256 ;
	uint64_t MAC4 = MACAddress2port.read() % 256 ;
	uint64_t MAC5 = MACAddress2port.read() / 256 ;

	registerAddressPort.write(20);//setting 32 bit mode
	busControlRgisterDataPort.write(0x102);

	registerAddressPort.write(0);
	registerDataPort.write(0x04);

	uint64_t MAC = (MAC5 << 40) |
				   (MAC4 << 32) |
				   (MAC3 << 24) |
				   (MAC2 << 16) |
				   (MAC1 << 8) |
				   (MAC0 ) ;

	initBlock.mode = 0x0000; //promiscuous mode = false
	initBlock.reserved1 = 0;
	initBlock.numSendBuffers = 3;
	initBlock.reserved2 = 0;
	initBlock.numRecvBuffers = 3;
	initBlock.physicalAddress = MAC;
	initBlock.reserved3 = 0;
	initBlock.logicalAddress = 0;

	sendBufferDesc = ((BufferDescriptor*)((((uint32_t)(&sendBufferDescMemory[0])) + 15) & ~((uint32_t)0xF)));
	initBlock.sendBufferDescAddr = (uint32_t)sendBufferDesc;
	recvBufferDesc = ((BufferDescriptor*)((((uint32_t)(&recvBufferDescMemory[0])) + 15) & ~((uint32_t)0xF)));
	initBlock.recvBufferDescAddr = (uint32_t)recvBufferDesc;

		for ( uint8_t var = 0;  var < 8; var++)
		{
			sendBufferDesc[var].address = (((uint32_t)(&sendBuffers[var])) + 15) & ~((uint32_t)0xF);
			sendBufferDesc[var].flags = 0x7FF | 0xF000;
			sendBufferDesc[var].flags2 = 0;
			sendBufferDesc[var].avail = 0;

			recvBufferDesc[var].address = (((uint32_t)(&recvBuffers[var])) + 15) & ~((uint32_t)0xF);
			recvBufferDesc[var].flags = 0xF7FF | 0x80000000;
			recvBufferDesc[var].flags2 =0 ;
			recvBufferDesc[var].avail = 0;
		}
		registerAddressPort.write(1);
		registerDataPort.write((uint32_t)(&initBlock) & 0xFFFF);
		registerAddressPort.write(2);
		registerDataPort.write(((uint32_t)(&initBlock) >> 16)& 0xFFFF);
	}
amd_am79973::~amd_am79973()
{
	}
void amd_am79973::Activate()
{
	registerAddressPort.write(0);
	registerDataPort.write(0x41);

	registerAddressPort.write(4);
	uint32_t temp = registerDataPort.read();
	registerAddressPort.write(4);
	registerDataPort.write(temp | 0xC00);

	registerAddressPort.write(0);
	registerDataPort.write(0x42);
}

int amd_am79973::Reset()
{
	resetPort.read();
	resetPort.write(0);
	return 10;
}

uint32_t amd_am79973::HandleInterrupt(uint32_t esp)
{
	printf("INTERRUPT from AMD Network card\n");
	registerAddressPort.write(0);
	uint32_t tempData = registerDataPort.read();

	if((tempData & 0x8000) == 0x8000) printf("amd_am79973 ERROR\n");
	if((tempData & 0x2000) == 0x2000) printf("amd_am79973 COLLISION ERROR\n");
	if((tempData & 0x1000) == 0x1000) printf("amd_am79973 MISSED FRAME\n");
	if((tempData & 0x0800) == 0x0800) printf("amd_am79973 MEMORY ERROR\n");
	if((tempData & 0x0400) == 0x0400) Receive();
	if((tempData & 0x0200) == 0x0200) printf("amd_am79973 DATA SENT\n");

	//Acknowldge
	registerAddressPort.write(0);
	registerDataPort.write(tempData);
	if((tempData & 0x100) == 0x100) printf("amd_am79973 INIT DONE\n");

	return esp;

}

void amd_am79973::Send(uint8_t* buffer,int count)
{
	printf("Send Enter\n");
	int sendDescriptor = currentSendBuffer;
	currentSendBuffer = (currentSendBuffer + 1) % 8 ;
	if(count > 1518)
	{
		count = 1518;
	}
	for (uint8_t *src=buffer + count -1,
			*dst = (uint8_t*)(sendBufferDesc[sendDescriptor].address + count -1);
			src >= buffer; src--,dst--)
	{
		*dst = *src;
	}

    printf("\nSEND: ");
    for(int i = 0; i < 64; i++)
    {
        printfHex(buffer[i]);
        printf(" ");
    }
	sendBufferDesc[sendDescriptor].avail = 0;
	sendBufferDesc[sendDescriptor].flags2 = 0;
	sendBufferDesc[sendDescriptor].flags = 0x8300F000 | ((uint16_t)((-count) & 0xFFF));

	registerAddressPort.write(0);
	printf("amd_am79973::Send After registerAddressPort.write\n");
	registerDataPort.write(0x48);
	printf("amd_am79973::Send After registerDataPort.write\n");
	printf("amd_am79973::Send Exit\n");

}
void amd_am79973::Receive()
{
	printf("amd_am79973 DATA RECEIVED\n");
	for (;((recvBufferDesc[currenRecvBuffer].flags & 0x80000000) == 0) ;currenRecvBuffer = (currenRecvBuffer + 1) % 8)
	{
		if(!(recvBufferDesc[currenRecvBuffer].flags & 0x40000000) && (recvBufferDesc[currenRecvBuffer].flags & 0x03000000) == 0x03000000)
		{
			uint32_t size = recvBufferDesc[currenRecvBuffer].flags & 0xFFF;
			if(size > 64)
			{
				size -= 4;
			}
			uint8_t* buffer = (uint8_t*)(recvBufferDesc[currenRecvBuffer].address);



			if(0 != rawDataHandler)
			{
				if(rawDataHandler->onRawDataReceived(buffer,size))
				{
					Send(buffer, size);
				}
			}
			size = 64;
			for(int i=0;i<size;i++)
			{
				printfHex(buffer[i]);
				printf(" ");
			}
			recvBufferDesc[currenRecvBuffer].flags2 = 0;
			recvBufferDesc[currenRecvBuffer].flags = 0x80007FF;
		}
	}
}

void amd_am79973::setHandler(RawDataHandler* rawDataHandler)
{
	this->rawDataHandler = rawDataHandler;
}

uint64_t amd_am79973::GetMACAddr()
{
	return initBlock.physicalAddress;
}

uint32_t amd_am79973::GetIPAddr()
{
	return initBlock.logicalAddress;
}
void amd_am79973::SetIPAddr(uint32_t ip_BE)
{
	initBlock.logicalAddress = ip_BE;
	}

