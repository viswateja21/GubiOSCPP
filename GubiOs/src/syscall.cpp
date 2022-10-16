/*
 * syscall.cpp
 *
 *  Created on: 20-Apr-2022
 *      Author: teja
 */
#include<syscall.h>
using namespace gubios;
using namespace gubios::common;
using namespace gubios::hardwarecommunication;

void printf(const char*);
SysCallHander::SysCallHander(uint8_t interruptNumber, InterruptManager* interruptManager) : InterruptHandler(
		interruptNumber+ interruptManager->getHardwareOffset(),
		interruptManager )
{
	printf("\nSysCallHander::SysCallHander\n");
	}
SysCallHander::~SysCallHander()
{
	}
uint32_t SysCallHander::HandleInterrupt(uint32_t esp)
{
	printf("SysCallHander::HandleInterrupt\n");
	CPUState* cpu = (CPUState*)esp;

	switch(cpu->eax)
	{

	case 4:
		printf((const char*)cpu->ebx);
		break;

	default:
		break;
	}
	return esp;
	}
