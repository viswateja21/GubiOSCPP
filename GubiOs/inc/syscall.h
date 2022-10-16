/*
 * syscall.h
 *
 *  Created on: 20-Apr-2022
 *      Author: teja
 */

#ifndef GUBIOS_SYSCALL_H_
#define GUBIOS_SYSCALL_H_
#include<common/types.h>
#include<hardwarecommunication/interrupts.h>
#include<multitasking.h>

namespace gubios
{
	class SysCallHander : public hardwarecommunication::InterruptHandler
	{
	public:
		SysCallHander(gubios::common::uint8_t interruptNumber, hardwarecommunication::InterruptManager* interruptManager);
		~SysCallHander();
		gubios::common::uint32_t HandleInterrupt(gubios::common::uint32_t esp);
	};
}

#endif /* INC_SYSCALL_H_ */
