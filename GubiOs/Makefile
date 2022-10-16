GPARAMS = -m32 -Iinc -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASPARAMS= --32
LDPARAMS= -melf_i386
objects = obj/kernel.o \
		  obj/gdt.o \
		  obj/loader.o \
		  obj/MemoryManager.o \
		  obj/hardwarecommunication/port.o \
		  obj/hardwarecommunication/interrupts.o \
		  obj/hardwarecommunication/interruptsstubs.o \
		  obj/multitasking.o \
		  obj/syscall.o \
		  obj/drivers/amd_am79C973.o \
		  obj/drivers/keyboard.o \
		  obj/drivers/mouse.o \
		  obj/drivers/Driver.o \
		  obj/drivers/vga.o \
		  obj/net/etherframe.o \
		  obj/net/arp.o \
		  obj/hardwarecommunication/pci.o
obj/%.o : src/%.cpp
	mkdir -p $(@D)
	g++ $(GPARAMS) -o $@ -c $<
obj/%.o : src/%.asm
	mkdir -p $(@D)
	as $(ASPARAMS) -c $< -o $@
obj/%.o : src/%.s
	mkdir -p $(@D)
	as $(ASPARAMS) -c $< -o $@
mykernel.bin : linker.ld $(objects)
	#i686-elf-ld -g -relocatable $^ -o kernelfull.o
	ld $(LDPARAMS) -g -T $< -o $@ $(objects)	
install : mykernel.bin
	sudo cp $< /boot/mykernel.bin
mykernel.iso : mykernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $< iso/boot/
	echo 'set timeout=0' > iso/boot/grub/grub.cfg
	echo 'set default=0' >> iso/boot/grub/grub.cfg
	echo '' >> iso/boot/grub/grub.cfg
	echo 'menuentry "My operating system Gubi os" {' >> iso/boot/grub/grub.cfg
	echo ' multiboot /boot/mykernel.bin' >> iso/boot/grub/grub.cfg
	echo ' boot' >> iso/boot/grub.cfg
	echo '}' >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$@ iso
	
clean:
	rm -rf obj
	rm -rf *iso 
	rm -rf *bin

run: mykernel.iso
	(killall VirtualBox && sleep 1) || true
	virtualboxvm --dbg --startvm New &
	
runQemu: mykernel.iso
	qemu-system-i386 -net user -net nic,model=pcnet -boot d -cdrom $< -m 512
