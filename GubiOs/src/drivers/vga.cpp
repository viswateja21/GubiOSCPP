#include<drivers/vga.h>


namespace gubios
{
	namespace drivers
	{
		void VideoGraphicsArray::writeRegisters(uint8_t* registers)
		{
			miscPort.write(*(registers++));
			for ( uint8_t i = 0;  i < 5; ++i)
			{
				sequenceIndexPort.write(i);
				sequenceDataPort.write(*(registers++));
			}

			crtcIndrxPort.write(0x03);
			crtcDataPort.write(crtcDataPort.read() | 0x80);
			crtcIndrxPort.write(0x11);
			crtcDataPort.write(crtcDataPort.read() & ~0x80);
			registers[0x03] |= 0x80;
			registers[0x11] &= ~0x80;


			for ( uint8_t var = 0;  var < 25; ++ var)
			{
				crtcIndrxPort.write(var);
				crtcDataPort.write(*(registers++));
			}

			for ( uint8_t var = 0;  var < 9; ++ var)
			{
				graphicsControllerIndexPort.write(var);
				graphicsControllerDataPort.write(*(registers++));
			}

			for ( uint8_t var = 0;  var < 21; ++ var)
			{
				attributeControllerResetPort.read();
				attributeControllerIndexPort.write(var);
				attributeControllerWritePort.write(*(registers++));
			}




		}


		VideoGraphicsArray::VideoGraphicsArray():
												miscPort(0x3C2),
												crtcIndrxPort(0x3D4),
												crtcDataPort(0x3D5),
												sequenceIndexPort(0x3C4),
												sequenceDataPort(0x3C5),
												graphicsControllerIndexPort(0x3CE),
												graphicsControllerDataPort(0x3CF),
												attributeControllerIndexPort(0x3C0),
												attributeControllerReadPort(0x3C1),
												attributeControllerWritePort(0x3C0),
												attributeControllerResetPort(0x3DA)
		{

		}
		VideoGraphicsArray::~VideoGraphicsArray()
		{

		}

		bool VideoGraphicsArray::setMode(uint32_t width,uint32_t height,uint32_t colorDepth)
		{

			if(!supportsMode(width,height,colorDepth))
			{
				return false;
			}

			unsigned char g_320x200x256[] =
			{
			/* MISC */
				0x63,
			/* SEQ */
				0x03, 0x01, 0x0F, 0x00, 0x0E,
			/* CRTC */
				0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F,
				0x00, 0x41, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x9C, 0x0E, 0x8F, 0x28,	0x40, 0x96, 0xB9, 0xA3,
				0xFF,
			/* GC */
				0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F,
				0xFF,
			/* AC */
				0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
				0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
				0x41, 0x00, 0x0F, 0x00,	0x00
			};
			writeRegisters(g_320x200x256);
		}

		bool VideoGraphicsArray::supportsMode(uint32_t width,uint32_t height,uint32_t colorDepth)
		{
			return width == 320 && height == 200 && colorDepth == 8;
		}

		void VideoGraphicsArray::putPixel(uint32_t x,uint32_t y,uint8_t red,uint8_t green,uint8_t blue)
		{
             putPixel(x, y, getColorIndex(red, green, blue));
		}

		uint8_t* VideoGraphicsArray::GetFrameBufferSegment()
		{
			graphicsControllerIndexPort.write(0x06);
			uint8_t segment = ((graphicsControllerDataPort.read() >> 2 ) & 0x03);
			switch(segment)
			{


				case 0:
					return (uint8_t*)0x00000;
				case 1:
					return (uint8_t*)0xA0000;
				case 2:
					return (uint8_t*)0xB0000;
				case 3:
					return (uint8_t*)0xB8000;
				default:
					return (uint8_t*)0x00000;

			}
		}

		void VideoGraphicsArray::putPixel(uint32_t x,uint32_t y,uint8_t colorIndex)
		{
			uint8_t* pixelAddr = GetFrameBufferSegment() + (y * 320) + x;
			*pixelAddr = colorIndex;
		}
		uint8_t VideoGraphicsArray::getColorIndex(uint8_t red,uint8_t green,uint8_t blue)
		{
			if(red == 0x00,green == 0x00,blue == 0xA8)
				return 0x01;
		}

	}
}
