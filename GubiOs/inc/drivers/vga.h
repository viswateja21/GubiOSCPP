#ifndef __GUBIOS_DRIVERS_VGA_H
#define __GUBIOS_DRIVERS_VGA_H

//#include <hardwarecommunication/interrupts.h>
#include <common/types.h>
#include <hardwarecommunication/port.h>
#include <drivers/Driver.h>

namespace gubios
{
	namespace drivers
	{

		class VideoGraphicsArray
		{
		protected:
			hardwarecommunication::Port8Bit miscPort;
			hardwarecommunication::Port8Bit crtcIndrxPort;
			hardwarecommunication::Port8Bit crtcDataPort;
			hardwarecommunication::Port8Bit sequenceIndexPort;
			hardwarecommunication::Port8Bit sequenceDataPort;
			hardwarecommunication::Port8Bit graphicsControllerIndexPort;
			hardwarecommunication::Port8Bit graphicsControllerDataPort;
			hardwarecommunication::Port8Bit attributeControllerIndexPort;
			hardwarecommunication::Port8Bit attributeControllerReadPort;
			hardwarecommunication::Port8Bit attributeControllerWritePort;
			hardwarecommunication::Port8Bit attributeControllerResetPort;

			void writeRegisters(uint8_t* registers);
			uint8_t* GetFrameBufferSegment();
		    virtual uint8_t getColorIndex(uint8_t red,uint8_t green,uint8_t blue);

		public:
			VideoGraphicsArray();
			 ~VideoGraphicsArray();

			virtual bool setMode(uint32_t width,uint32_t height,uint32_t colorDepth);
			virtual bool supportsMode(uint32_t width,uint32_t height,uint32_t colorDepth);
			virtual void putPixel(uint32_t x,uint32_t y,uint8_t red,uint8_t green,uint8_t blue);
			virtual void putPixel(uint32_t x,uint32_t y,uint8_t colorIndex);

		};
	}
}
#endif
