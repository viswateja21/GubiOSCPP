
#ifndef GUBIOS_NET_ARP_H
#define GUBIOS_NET_ARP_H

#include <net/etherframe.h>
#include <common/types.h>

namespace gubios
{
	namespace net
	{
		struct AddressResolutionProtocolMessage
		{
			common::uint16_t hardwareType;
			common::uint16_t protocol;
			common::uint8_t hardwareAddrSize;
			common::uint8_t protocolAddrSize;
			common::uint16_t command;

			common::uint64_t srcMACAddr:48;
			common::uint32_t srcIPAddr;
			common::uint64_t dstMACAddr:48;
			common::uint32_t dstIPAddr;



		}__attribute__((packed));

		class AddressResolutionProtocol : public EtherFrameHandler
		{
			common::uint32_t IpCache[128];
			common::uint64_t MACCache[128];
			uint16_t numCacheEntries;
		public:
			AddressResolutionProtocol(EtherFrameProvider* backend);
			~AddressResolutionProtocol();
			bool OnEtherFrameReceived(common::uint8_t* etherframepayload,common::uint32_t size);
			void ReqMACAddr(common::uint32_t ip_BE);
			common::uint64_t GetMACFromCache(common::uint32_t ip_BE);
			common::uint64_t Resolve(uint32_t IP_BE);

		};
	}
}
#endif
