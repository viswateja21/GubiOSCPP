/*
 * arp.cpp
 *
 *  Created on: 28-Apr-2022
 *      Author: teja
 */

#include <net/arp.h>
using namespace gubios;
using namespace gubios::common;
using namespace gubios::net;

extern void printf(const char* ptr);
AddressResolutionProtocol::AddressResolutionProtocol(EtherFrameProvider* backend) : EtherFrameHandler(backend, 0x806)
{
	numCacheEntries = 0;
}

AddressResolutionProtocol::~AddressResolutionProtocol()
{

}

bool AddressResolutionProtocol::OnEtherFrameReceived(uint8_t* etherframepayload,uint32_t size)
{
	if(size < sizeof(AddressResolutionProtocolMessage))
	{
		return false;
	}
	AddressResolutionProtocolMessage* arp = (AddressResolutionProtocolMessage*)etherframepayload;
	if (arp->hardwareType == 0x0100)
	{
		if(arp->protocol == 0x0008
		   && arp->hardwareAddrSize == 6
		   && arp->protocolAddrSize == 4
		   && arp->dstIPAddr == backend->GetIPAddr())
		{
			switch(arp->command)
			{
			case 0x0100: //request
					arp->command =0x0200;
					arp->dstIPAddr = arp->srcIPAddr;
					arp->dstMACAddr = arp->srcMACAddr;
					arp->srcIPAddr = backend->GetIPAddr();
					arp->srcMACAddr = backend->GetMACAddr();
					return true;
					break;

			case 0x0200: //response
					if(numCacheEntries < 128)
					{
						IpCache[numCacheEntries] = arp->srcIPAddr;
						MACCache[numCacheEntries] = arp->srcMACAddr;
						numCacheEntries++;
					}
					break;
			}
		}
	}

}


void AddressResolutionProtocol::ReqMACAddr(uint32_t ip_BE)
{
	printf("ReqMACAddr Enter\n");
	AddressResolutionProtocolMessage arp;
	arp.hardwareType = 0x0100; // Ethernet
	arp.protocol = 0x0008;  //ipv4
	arp.hardwareAddrSize = 6; // MAC addr size
	arp.protocolAddrSize = 4; // ipv4 addr size
	arp.command = 0x0100; //request

	arp.srcMACAddr = backend->GetMACAddr();
	arp.srcIPAddr = backend->GetIPAddr();
	arp.dstMACAddr = 0xFFFFFFFFFFFF; //Broadcast addr
	arp.dstIPAddr = ip_BE;
	send((uint8_t*)&arp, arp.dstMACAddr, sizeof(AddressResolutionProtocolMessage));
	printf("ReqMACAddr Exit\n");
}


uint64_t AddressResolutionProtocol::GetMACFromCache(uint32_t ip_BE)
{
	for (uint16_t var =  0; var < numCacheEntries; ++var)
	{
			if(ip_BE == IpCache[var])
			{
				return MACCache[var];
			}

	}
	return 0xFFFFFFFFFFFF; // broadcast address


}

uint64_t AddressResolutionProtocol::Resolve(uint32_t IP_BE)
{
	uint64_t resolve = GetMACFromCache(IP_BE);
	if(resolve == 0xFFFFFFFFFFFF)
	{
		ReqMACAddr(IP_BE);
	}
//	while(resolve == 0xFFFFFFFFFFFF)  // possible infinite loop
	{
		//printf("AddressResolutionProtocol::Resolve loop\n");
		resolve = GetMACFromCache(IP_BE);
	}
	printf("AddressResolutionProtocol::Resolve Exit\n");
	return resolve;
}

