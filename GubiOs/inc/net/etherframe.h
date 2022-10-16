#ifndef GUBIOS_NET_ETHERFRAME_H
#define GUBIOS_NET_ETHERFRAME_H

#include <common/types.h>
#include <drivers/amd_am79973.h>
#include <MemoryManager.h>

namespace gubios
{
	namespace net
	{

		struct EtherFrameHeader
		{
			common::uint64_t dstMAC_BE:48;
			common::uint64_t srcMAC_BE:48;
			common::uint16_t etherTypeBE;

		}__attribute__((packed));

		typedef common::uint32_t EtherFrameFooter;

		class EtherFrameProvider;

		class EtherFrameHandler
		{
		protected:
			EtherFrameProvider* backend;
			common::uint16_t etherType_BE;
		public:
			EtherFrameHandler(EtherFrameProvider* backend,common::uint16_t etherType);
			~EtherFrameHandler();
			virtual bool OnEtherFrameReceived(common::uint8_t* etherframepayload,common::uint32_t size);
			void send(common::uint8_t* etherframepayload,common::uint64_t dstMAC_BE,common::uint32_t size);
		};

		class EtherFrameProvider : public RawDataHandler
		{
			friend class EtherFrameHandler;
		protected:
			EtherFrameHandler* handler[65535];
		public:
			EtherFrameProvider(amd_am79973* backend);
			~EtherFrameProvider();
			 bool onRawDataReceived(common::uint8_t* buffer,common::size_t size);
			 void Send(common::uint64_t dstMAC_BE, common::uint16_t etherType, common::uint8_t* buffer, common::size_t size);
			 common::uint32_t GetIPAddr();
			 common::uint64_t GetMACAddr();
		};
	}
}
#endif
