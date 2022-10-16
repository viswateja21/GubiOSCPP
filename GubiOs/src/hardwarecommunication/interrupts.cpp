#include <hardwarecommunication/interrupts.h>

using namespace gubios::hardwarecommunication;
using namespace gubios::common;

InterruptManager::GateDescriptor InterruptManager::interruptdesctable[256];

InterruptManager* InterruptManager::activeInterruptManager = 0;

extern void printf(const char* str);
extern void printfHex(uint8_t key);

InterruptHandler::InterruptHandler(uint8_t interruptNumber, InterruptManager* interruptManager)
{
	printf("InterruptHandler()");
	this->InterruptNumber = interruptNumber;
	this->interruptManager = interruptManager;
	interruptManager->handlers[interruptNumber] = this;
}

InterruptHandler::~InterruptHandler()
{
	if(interruptManager->handlers[this->InterruptNumber] == this)
	{
		interruptManager->handlers[this->InterruptNumber] = 0;
	}

}

uint32_t InterruptHandler::HandleInterrupt(uint32_t esp)
{
	return esp;
}


	uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber,uint32_t esp)
{
	 if(0 != activeInterruptManager)
	 {
		 return activeInterruptManager->doHandleInterrupt(interruptNumber, esp);
	 }
	return esp;
}

 uint32_t InterruptManager::doHandleInterrupt(uint8_t interruptNumber,uint32_t esp)
{

	 if(handlers[interruptNumber] != 0)
	 {
		 printf("valid \n");
		 		 	 printfHex(interruptNumber);
		esp = handlers[interruptNumber]->HandleInterrupt(esp);
	 }
	 else if(interruptNumber != 0x20)
	 {
		 	 printf("\n");
		 	 printfHex(interruptNumber);
	 }

	 if(interruptNumber >= 0x20 && interruptNumber < 0x30)
	 {
		 masterCommand.write(0x20);
		 if(interruptNumber >=0x28)
		 {
			 slaveCommand.write(0x20);
		 }
	 }
	return esp;
}

 void InterruptManager::setIDTEntry(
 				uint8_t interruptNumber,
 				uint16_t gdt_table_selector,
 				void (*handle)(),
 				uint8_t DescriptorPrevilage,
 				uint8_t DescriptorType
 				)
 {
	 const uint8_t IDT_DESC_PRESENT = 0x80;
	 interruptdesctable[interruptNumber].handlerAddressLowBits = ((uint32_t)handle & 0xFFFF);
	 interruptdesctable[interruptNumber].handlerAddressHighBits = (((uint32_t)handle >> 16 ) & 0xFFFF);
	 interruptdesctable[interruptNumber].gdt_table_selector = gdt_table_selector;
	 interruptdesctable[interruptNumber].access = IDT_DESC_PRESENT | DescriptorType | ((DescriptorPrevilage & 3)<< 5);
	 interruptdesctable[interruptNumber].reserved = 0;

 }

 uint16_t InterruptManager::getHardwareOffset()
 {
	 return hardwareOffset;
 }
 InterruptManager::InterruptManager(uint16_t hardwareInterruptOffset,GlobalDescriptorTable *gdt,uint16_t HardWareOffset):masterCommand(0x20),
		 	 	 	 	 	 	 	 	 	 	 	 	 	 	masterData(0x21),
																slaveCommand(0xA0),
																slaveData(0xA1)

 {
	 this->hardwareOffset = HardWareOffset;
	 this->hardwareInterruptOffset = hardwareInterruptOffset;
	 uint16_t codeSegment = gdt->getCodeSegmentOffset();

	 const uint8_t IDT_INTERRUPT_GATE = 0xE;
	 for(uint16_t i = 0;i<256;i++)
	 {
		 setIDTEntry(i, codeSegment, &InterruptIgnore, 0, IDT_INTERRUPT_GATE);
		 handlers[i] = 0;

	 }
	    setIDTEntry(0x00, codeSegment, &HandleException0x00, 0, IDT_INTERRUPT_GATE);
	    setIDTEntry(0x01, codeSegment, &HandleException0x01, 0, IDT_INTERRUPT_GATE);
	    setIDTEntry(0x02, codeSegment, &HandleException0x02, 0, IDT_INTERRUPT_GATE);
	    setIDTEntry(0x03, codeSegment, &HandleException0x03, 0, IDT_INTERRUPT_GATE);
	    setIDTEntry(0x04, codeSegment, &HandleException0x04, 0, IDT_INTERRUPT_GATE);
	    setIDTEntry(0x05, codeSegment, &HandleException0x05, 0, IDT_INTERRUPT_GATE);
	    setIDTEntry(0x06, codeSegment, &HandleException0x06, 0, IDT_INTERRUPT_GATE);
	    setIDTEntry(0x07, codeSegment, &HandleException0x07, 0, IDT_INTERRUPT_GATE);
	    setIDTEntry(0x08, codeSegment, &HandleException0x08, 0, IDT_INTERRUPT_GATE);
	    setIDTEntry(0x09, codeSegment, &HandleException0x09, 0, IDT_INTERRUPT_GATE);
	    setIDTEntry(0x0A, codeSegment, &HandleException0x0A, 0, IDT_INTERRUPT_GATE);
	    setIDTEntry(0x0B, codeSegment, &HandleException0x0B, 0, IDT_INTERRUPT_GATE);
	    setIDTEntry(0x0C, codeSegment, &HandleException0x0C, 0, IDT_INTERRUPT_GATE);
	    setIDTEntry(0x0D, codeSegment, &HandleException0x0D, 0, IDT_INTERRUPT_GATE);
	    setIDTEntry(0x0E, codeSegment, &HandleException0x0E, 0, IDT_INTERRUPT_GATE);
	    setIDTEntry(0x0F, codeSegment, &HandleException0x0F, 0, IDT_INTERRUPT_GATE);
	    setIDTEntry(0x10, codeSegment, &HandleException0x10, 0, IDT_INTERRUPT_GATE);
	    setIDTEntry(0x11, codeSegment, &HandleException0x11, 0, IDT_INTERRUPT_GATE);
	    setIDTEntry(0x12, codeSegment, &HandleException0x12, 0, IDT_INTERRUPT_GATE);
	    setIDTEntry(0x13, codeSegment, &HandleException0x13, 0, IDT_INTERRUPT_GATE);

	    	setIDTEntry(hardwareInterruptOffset + 0x00, codeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
	        setIDTEntry(hardwareInterruptOffset + 0x01, codeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
	        setIDTEntry(hardwareInterruptOffset + 0x02, codeSegment, &HandleInterruptRequest0x02, 0, IDT_INTERRUPT_GATE);
	        setIDTEntry(hardwareInterruptOffset + 0x03, codeSegment, &HandleInterruptRequest0x03, 0, IDT_INTERRUPT_GATE);
	        setIDTEntry(hardwareInterruptOffset + 0x04, codeSegment, &HandleInterruptRequest0x04, 0, IDT_INTERRUPT_GATE);
	        setIDTEntry(hardwareInterruptOffset + 0x05, codeSegment, &HandleInterruptRequest0x05, 0, IDT_INTERRUPT_GATE);
	        setIDTEntry(hardwareInterruptOffset + 0x06, codeSegment, &HandleInterruptRequest0x06, 0, IDT_INTERRUPT_GATE);
	        setIDTEntry(hardwareInterruptOffset + 0x07, codeSegment, &HandleInterruptRequest0x07, 0, IDT_INTERRUPT_GATE);
	        setIDTEntry(hardwareInterruptOffset + 0x08, codeSegment, &HandleInterruptRequest0x08, 0, IDT_INTERRUPT_GATE);
	        setIDTEntry(hardwareInterruptOffset + 0x09, codeSegment, &HandleInterruptRequest0x09, 0, IDT_INTERRUPT_GATE);
	        setIDTEntry(hardwareInterruptOffset + 0x0A, codeSegment, &HandleInterruptRequest0x0A, 0, IDT_INTERRUPT_GATE);
	        setIDTEntry(hardwareInterruptOffset + 0x0B, codeSegment, &HandleInterruptRequest0x0B, 0, IDT_INTERRUPT_GATE);
	        setIDTEntry(hardwareInterruptOffset + 0x0C, codeSegment, &HandleInterruptRequest0x0C, 0, IDT_INTERRUPT_GATE);
	        setIDTEntry(hardwareInterruptOffset + 0x0D, codeSegment, &HandleInterruptRequest0x0D, 0, IDT_INTERRUPT_GATE);
	        setIDTEntry(hardwareInterruptOffset + 0x0E, codeSegment, &HandleInterruptRequest0x0E, 0, IDT_INTERRUPT_GATE);
	        setIDTEntry(hardwareInterruptOffset + 0x0F, codeSegment, &HandleInterruptRequest0x0F, 0, IDT_INTERRUPT_GATE);

	        setIDTEntry(0x80, codeSegment, &HandleInterruptRequest0x80, 0, IDT_INTERRUPT_GATE);



	 masterCommand.write(0x11);
	 slaveCommand.write(0x11);

	 masterData.write(0x20);
	 slaveData.write(0x28);

	 masterData.write(0x04);
	 slaveData.write(0x02);

	 masterData.write(0x01);
	 slaveData.write(0x01);

	 masterData.write(0x00);
	 slaveData.write(0x00);

	 IDTPtr idt;
	 idt.size = 256 * sizeof(GateDescriptor)-1;
	 idt.base = (uint32_t)interruptdesctable;
	 asm volatile ("lidt %0"::"m"(idt));
 }

 void InterruptManager::ActivateInterrupts()
 {
	 if(0 != activeInterruptManager)
	 {
		 activeInterruptManager->Deactivate();

	 }
	 activeInterruptManager = this;
	 asm volatile("sti");
 }
 void InterruptManager::Deactivate()
 {
	 if(this == activeInterruptManager)
	 {
		 activeInterruptManager = 0;
		 asm volatile("cli");
	 }

 }
 InterruptManager::~InterruptManager()
 {

 }
/*
 void InterruptManager::HandleInterruptRequest0x00()
 {
 }
 void InterruptManager::HandleInterruptRequest0x01()
 {
 }
 */
 void InterruptManager::InterruptIgnore()
 {
 }

