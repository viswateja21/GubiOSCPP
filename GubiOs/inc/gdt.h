/*
 * gdt.h
 *
 *  Created on: 01-May-2021
 *      Author: teja
 */

#ifndef GUBIOS_GDT_H_
#define GUBIOS_GDT_H_
#include <common/types.h>
using namespace gubios::common;


namespace gubios
{

class GlobalDescriptorTable
{

public:
	class SegmentDescriptor
	{
	private:
		uint16_t limit_low;  		//segment length .first 16 bits i.e 0 to 15 (limit low bits)
		uint16_t baseAddr_low;		//Base Address of segment i.e first 16 bits (0 - 15)
		uint8_t  baseAddr_high; 	//Base Address of segment i.e middle 8 bits (16 - 23)
		uint8_t  accessByte ;    	//Access byte contains the privilages levels of segment
		uint8_t  flags_limit_hi;	//4 bits for flags and 4 bits for segment length
		uint8_t  baseAddr_veryHigh; //Base Address of segment i.e last 8 bits (24 - 31)

	public:
		SegmentDescriptor(uint32_t baseAddr, uint32_t limit, uint8_t accessByte);
		uint32_t getBaseAddr();
		uint32_t getLimit();
	}__attribute__((packed));


	SegmentDescriptor nullSegmentSelector;				// null segment as per IA-32 standard
	SegmentDescriptor unUsedSegmentSelector;			// Unused segment
	SegmentDescriptor CodeSegmentSelector;				// Code segment
	SegmentDescriptor DataSegmentSelector;				// Data segment

	GlobalDescriptorTable();  							//  Constructor
	~GlobalDescriptorTable();							// Destructor

	uint16_t getCodeSegmentOffset();					// returns o
	uint16_t getDataSegmentOffset();

};

}
#endif /* GDT_H_ */
