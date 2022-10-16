/*
 * MemoryManager.h
 *
 *  Created on: 14-Feb-2022
 *      Author: teja
 */

#ifndef INC_MEMORYMANAGER_H_
#define INC_MEMORYMANAGER_H_

#include<common/types.h>
using namespace gubios::common;
namespace gubios
{
	struct MemoryChunk
	{
		MemoryChunk* next;
		MemoryChunk* prev;
		bool allocated;
		size_t size;
	};
	class MemoryManager
	{
	protected:

		MemoryChunk* first;

	public:
		static MemoryManager *activeMemoryManager;

		MemoryManager(gubios::common::size_t start,gubios::common::size_t size);
		~MemoryManager();

		void *malloc(gubios::common::size_t size);
		void free(void* ptr);

	};
}

void* operator new(unsigned size);
void* operator new[](unsigned size);

void* operator new(unsigned size,void *ptr);
void* operator new[](unsigned size,void *ptr);

void operator delete(void  *ptr);
void operator delete[](void  *ptr);
#endif /* INC_MEMORYMANAGER_H_ */
