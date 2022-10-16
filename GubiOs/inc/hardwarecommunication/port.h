#ifndef __GUBIOS_HARDWARECOMMUNICATION_PORT_H
#define __GUBIOS_HARDWARECOMMUNICATION_PORT_H
#include <common/types.h>

using namespace gubios::common;
namespace gubios
{
	namespace hardwarecommunication
	{
		class Port
		{
		protected:
			gubios::common::uint16_t port;
			Port(gubios::common::uint16_t port);
			~Port();
		};

		class Port8Bit : public Port
		{
		public:
			Port8Bit(gubios::common::uint16_t port);
			virtual void  write(gubios::common::uint8_t data);
			virtual gubios::common::uint8_t read();
			~Port8Bit();

		protected:
		                static inline gubios::common::uint8_t Read8(gubios::common::uint16_t _port)
		                {
		                    gubios::common::uint8_t result;
		                    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (_port));
		                    return result;
		                }

		                static inline void Write8(gubios::common::uint16_t _port, gubios::common::uint8_t _data)
		                {
		                    __asm__ volatile("outb %0, %1" : : "a" (_data), "Nd" (_port));
		                }
		};

		class Port8BitSlow : public Port
		{
		public:
			Port8BitSlow(gubios::common::uint16_t port);
			virtual void  write(gubios::common::uint8_t data);
			~Port8BitSlow();
		 protected:
		                static inline void Write8Slow(gubios::common::uint16_t _port, gubios::common::uint8_t _data)
		                {
		                    __asm__ volatile("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (_data), "Nd" (_port));
		                }
		};

		class Port16Bit : public Port
		{
		public:
			Port16Bit(gubios::common::uint16_t port);
			virtual void  write(gubios::common::uint16_t data);
			virtual gubios::common::uint16_t read();
			~Port16Bit();
		 protected:
		                static inline gubios::common::uint16_t Read16(gubios::common::uint16_t _port)
		                {
		                    gubios::common::uint16_t result;
		                    __asm__ volatile("inw %1, %0" : "=a" (result) : "Nd" (_port));
		                    return result;
		                }

		                static inline void Write16(gubios::common::uint16_t _port, gubios::common::uint16_t _data)
		                {
		                    __asm__ volatile("outw %0, %1" : : "a" (_data), "Nd" (_port));
		                }
		};


		class Port32Bit : public Port
		{
		public:
			Port32Bit(gubios::common::uint16_t port);
			virtual void  write(gubios::common::uint32_t data);
			virtual gubios::common::uint32_t read();
			~Port32Bit();
		  protected:
		                static inline gubios::common::uint32_t Read32(gubios::common::uint16_t _port)
		                {
		                    gubios::common::uint32_t result;
		                    __asm__ volatile("inl %1, %0" : "=a" (result) : "Nd" (_port));
		                    return result;
		                }

		                static inline void Write32(gubios::common::uint16_t _port, gubios::common::uint32_t _data)
		                {
		                    __asm__ volatile("outl %0, %1" : : "a"(_data), "Nd" (_port));
		                }
		};
	}
}
#endif
