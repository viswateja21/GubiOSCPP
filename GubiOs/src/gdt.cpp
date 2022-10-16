/*
 * gdt.cpp
 *
 *  Created on: 01-May-2021
 *      Author: teja
 */
#include <gdt.h>
using namespace gubios;

#define SEGMENT_SIZE	(1024 *1024 * 64)

GlobalDescriptorTable::GlobalDescriptorTable():nullSegmentSelector(0,0,0),unUsedSegmentSelector(0,0,0),
	CodeSegmentSelector(0, SEGMENT_SIZE,0x9A),DataSegmentSelector(0,SEGMENT_SIZE,0x92)
{
	uint32_t lgdtAddr[2];
	lgdtAddr[1] = (uint32_t)this;
	lgdtAddr[0] = sizeof(GlobalDescriptorTable) << 16;
	asm volatile ("lgdt (%0)": : "p"(((uint8_t*)lgdtAddr) + 2));

}
GlobalDescriptorTable::~GlobalDescriptorTable()
{

}

uint16_t GlobalDescriptorTable::getCodeSegmentOffset()
{
	return (uint8_t*)&CodeSegmentSelector - (uint8_t*)this;
}

uint16_t GlobalDescriptorTable::getDataSegmentOffset()
{
	return (uint8_t*)&DataSegmentSelector - (uint8_t*)this;
}


GlobalDescriptorTable::SegmentDescriptor::SegmentDescriptor(uint32_t baseAddr, uint32_t limit, uint8_t accessByte)
{
	uint8_t* target = (uint8_t*)this;
	if(65535 >= limit)
	{
		target[6]  = 0x40;			// only s bit is set in 7 th byte of descriptor without granularity bit
	}
	else
	{
		if((target && 0xFFF) != 0xFFF)
		{
			limit = (limit >> 12) - 1;         // shifted by 12 bits to address upto Gib memory i.e (4kb to 4Gb)
		}
		else
		{
			limit = limit >> 12;
		}
		// setting limit to descritor
		target[6] = 0xC0;
		target[0] =  limit & 0xFF;
		target[1] =  (limit>>8) & 0xFF;
		target[6] |= (limit>>16) & 0xF;
		//setting base addr to descriptor
		target[2] = baseAddr & 0xFF;
		target[3] = (baseAddr>>8) & 0xFF;
		target[4] = (baseAddr >> 16) & 0xFF;
		target[7] = (baseAddr >>24) & 0xFF;
		// setting access byte to descriptor
		target[5] = accessByte;


	}

}
uint32_t GlobalDescriptorTable::SegmentDescriptor::getBaseAddr()
{
	uint8_t* target = (uint8_t*)this;
	uint32_t result = target[7];
	result = (result<<8) + target[4];
	result = (result<<8) + target[3];
	result = (result<<8) + target[2];
	return result;
}

uint32_t GlobalDescriptorTable::SegmentDescriptor::getLimit()
{
	uint8_t* target = (uint8_t*)this;
	uint32_t result = target[6] & 0xF;
	result = (result<<8) + target[1];
	result = (result<<8) + target[0];
	if((target[6] & 0xC0) == 0xC0)
		result = (result << 12) | 0xFFF;

	return result;
}
