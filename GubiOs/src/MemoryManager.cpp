/*
 * MemoryManager.cpp
 *
 *  Created on: 14-Feb-2022
 *      Author: teja
 */

#include<MemoryManager.h>
void printf(const char*);
void printfHex(uint8_t key);
namespace gubios
{
MemoryManager* MemoryManager::activeMemoryManager=0;

MemoryManager::MemoryManager(common::size_t start,common::size_t size)
{
	activeMemoryManager = this;
	if(size < sizeof(MemoryChunk))
	{
		first = 0;
	}
	else
	{
	first = (MemoryChunk*)start;
	first->allocated = false;
	first->prev = 0;
	first->next = 0;
	first->size = size - sizeof(MemoryChunk);
	}
}

MemoryManager::~MemoryManager()
{
	if(activeMemoryManager = this)
	{
		activeMemoryManager = 0;
	}
	}

void* MemoryManager::malloc(size_t size)
{
//	printf("malloc Enter\n");
	MemoryChunk* result = 0;
	MemoryChunk* chunk = 0;
	for(chunk = first;chunk != 0 && result == 0;chunk = chunk->next)
	{
		if(chunk->size > size && !chunk->allocated)
		{
			result = chunk;
		}
	}
	if(0 == result)
	{
	//	printf("malloc Return 0\n");
		return 0;
	}
//	if(chunk->size < size + sizeof(MemoryChunk) + 1)
//	{
//
//	}

	if(result->size >= size + sizeof(MemoryChunk) + 1)
	{
	//	printf("malloc calculation Enter\n");
		MemoryChunk* ptr = (MemoryChunk*)((size_t)result + sizeof(MemoryChunk) + size);
		ptr->allocated = false;
		ptr->size =result->size - size - sizeof(MemoryChunk);
		ptr->next = result->next;
		ptr->prev = result;
		if(ptr->next != 0)
		{
			ptr->next->prev = ptr;
		}

		result->allocated = true;
		result->next = ptr;
		result->size = size;

	}

	//chunk->allocated = true;
//	printf("malloc Exit\n");
	void* pp = (void*)((size_t)result + sizeof(chunk));
	//	printf("\nAllocated heap: 0x");
		printfHex(((size_t)pp>> 24) & 0xFF);
		printfHex(((size_t)pp>> 16) & 0xFF);
		printfHex(((size_t)pp>> 8) & 0xFF);
		printfHex(((size_t)pp) & 0xFF);
	//	printf("\n");
	return (void*)((size_t)result + sizeof(chunk));

	}

void MemoryManager::free(void* ptr)
{
	MemoryChunk* chunk = (MemoryChunk*)((size_t)ptr - sizeof(MemoryChunk));
	chunk->allocated = false;

	if(chunk->prev != 0 && !chunk->prev->allocated)
	{
		chunk->prev->next = chunk->next;
		chunk->prev->size += chunk->size + sizeof(MemoryChunk);
		if(chunk->next != 0)
		{
			chunk->next->prev = chunk->prev;
		}
		chunk = chunk->prev;
	}
	if(chunk->prev != 0 && !chunk->next->allocated)
	{
		chunk->size += chunk->next->size + sizeof(MemoryChunk);
		chunk->next = chunk->next->next;
		if(chunk->next!=0)
		{
			chunk->next->prev = chunk;
		}
	}
}


}


void* operator new(unsigned size)
{
	if(gubios::MemoryManager::activeMemoryManager == 0)
		return 0;
	return gubios::MemoryManager::activeMemoryManager->malloc(size);
}

void* operator new[](unsigned size)
{
	if(gubios::MemoryManager::activeMemoryManager == 0)
		return 0;
	return gubios::MemoryManager::activeMemoryManager->malloc(size);
}

void operator delete(void  *ptr)
{
	if(gubios::MemoryManager::activeMemoryManager != 0)
		 gubios::MemoryManager::activeMemoryManager->free(ptr);

}

void operator delete[](void  *ptr)
{
	if(gubios::MemoryManager::activeMemoryManager != 0)
		 gubios::MemoryManager::activeMemoryManager->free(ptr);

}

void* operator new(unsigned size,void *ptr)
{
	return ptr;
}
void* operator new[](unsigned size,void *ptr)
{
	return ptr;
	}

