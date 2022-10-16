#include<net/etherframe.h>

using namespace gubios;
using namespace gubios::common;
using namespace gubios::net;

EtherFrameHandler::EtherFrameHandler(EtherFrameProvider* backend,uint16_t etherType)
{
		this->backend = backend;
		this->etherType_BE = (etherType & 0x00FF) << 8 | (etherType & 0xFF00)>> 8;
		backend->handler[etherType] = this;
}

EtherFrameHandler::~EtherFrameHandler()
{
	backend->handler[etherType_BE] = 0;
}
bool EtherFrameHandler::OnEtherFrameReceived(common::uint8_t* etherframepayload,common::uint32_t size)
{
	return false;
}


void EtherFrameProvider::Send(common::uint64_t dstMAC_BE, common::uint16_t etherType, common::uint8_t* buffer, common::size_t size)
{
	uint8_t* buffer2 = (uint8_t*)MemoryManager::activeMemoryManager->malloc(sizeof(EtherFrameHeader) + size);
	EtherFrameHeader* frame = (EtherFrameHeader*)buffer2;
	frame->dstMAC_BE = dstMAC_BE;
	frame->srcMAC_BE = backend->GetMACAddr();
	frame->etherTypeBE = etherType;
	uint8_t* src = buffer;
	uint8_t* des = (uint8_t*)(buffer2 + sizeof(EtherFrameHeader));
	for (uint32_t var = 0; var < size; var++) {
		des[var] = src[var];
	}
	backend->Send(buffer2, size + sizeof(EtherFrameHeader));
}

EtherFrameProvider::EtherFrameProvider(amd_am79973* backend) : RawDataHandler(backend)
{
	for(uint32_t i = 0;i<65535;i++)
	{
		handler[i] = 0;
	}
}
EtherFrameProvider::~EtherFrameProvider()
{

}
bool EtherFrameProvider::onRawDataReceived(common::uint8_t* buffer,common::size_t size)
{
	EtherFrameHeader* frame = (EtherFrameHeader*)buffer;
	bool sendBack = false;

	if(frame->dstMAC_BE  == 0xFFFFFFFFFFFF || frame->dstMAC_BE == backend->GetMACAddr())
	{
		if(handler[frame->etherTypeBE] != 0)
		{
			sendBack = handler[frame->etherTypeBE]->OnEtherFrameReceived(buffer + sizeof(EtherFrameHeader),size -
					sizeof(EtherFrameHeader));
		}
	}

	if(sendBack)
	{
		frame->dstMAC_BE = frame->srcMAC_BE;
		frame->srcMAC_BE = backend->GetMACAddr();
	}
	return sendBack;
}

void EtherFrameHandler::send(uint8_t* etherframepayload,uint64_t dstMAC_BE,uint32_t size)
{
	backend->Send(dstMAC_BE, etherType_BE, etherframepayload, size);
}

uint32_t EtherFrameProvider::GetIPAddr()
{
	return backend->GetIPAddr();
}

uint64_t EtherFrameProvider::GetMACAddr()
{
	return backend->GetMACAddr();
}

