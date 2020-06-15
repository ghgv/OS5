GPPARAMS = -g -Iinclude -m32 -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti -fno-exceptions -fno-leading-underscore
ASPARAMS = --32
NASMPARAMS = -f elf
LDPARAMS = -melf_i386
objects = 	obj/interrupt.o \
		obj/test.o \
		obj/paging.o \
		obj/timer.o \
		obj/syscall.o \
		obj/drivers/keyboard.o \
		arch/um/tty.o \
		obj/isr.o \
		obj/multitask.o \
		obj/loader.o \
		obj/kheap.o \
		obj/gdt_s.o \
		obj/gdt.o \
		obj/mt.o \
		obj/common.o \
		obj/stdio.o \
		obj/kernel.o
		

		
obj/%.o: src/%.asm
	nasm $(NASMPARAMS) -o $@ $<
	
arch/%.o: arch//%.cpp
	gcc $(GPPARAMS) -o $@ -c $<
	
obj/%.o: src/%.cpp
	gcc $(GPPARAMS) -o $@ -c $<

obj/%.o: src/%.s
	as $(ASPARAMS) -o $@ $<

mykernel.bin: linker.ld $(objects)
	ld $(LDPARAMS) -T $< -o $@ $(objects)
	
install:mykernel.bin
	sudo cp $< /boot/mykernel.bin
clean: 
	rm -r obj/*.o
	rm -r *.iso
	rm -r *.bin
	
mykernel.iso: mykernel.bin
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp mykernel.bin iso/boot/mykernel.bin
	echo 'set timeout=1'                      > iso/boot/grub/grub.cfg
	echo 'set default=1'                     >> iso/boot/grub/grub.cfg
	echo ''                                  >> iso/boot/grub/grub.cfg
	echo 'menuentry "OS5" {' >> iso/boot/grub/grub.cfg
	echo '  multiboot /boot/mykernel.bin'    >> iso/boot/grub/grub.cfg
	echo '  boot'                            >> iso/boot/grub/grub.cfg
	echo '}'                                 >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=mykernel.iso iso
	rm -rf iso
	
mykernel.img: mykernel.bin
	mkdir mnt2 
	sudo losetup /dev/loop10 hda.img
	mount /dev/loop0 mnt2
	mkdir mnt2/boot
	mkdir mnt2/boot/grub
	cp mykernel.bin mnt2/boot/mykernel.bin
	echo 'set timeout=10'                   >> mnt2/boot/grub/grub.cfg
	echo 'set default=10'                   >> mnt2/boot/grub/grub.cfg
	echo ''                                 >> mnt2/boot/grub/grub.cfg
	echo 'menuentry "OS5" {' 		>> mnt2/boot/grub/grub.cfg
	echo '  multiboot /boot/mykernel.bin'   >> mnt2/boot/grub/grub.cfg
	echo '  boot'                           >> mnt2/boot/grub/grub.cfg
	echo '}'                                >> mnt2/boot/grub/grub.cfg
	umount /dev/loop0
	losetup -d /dev/loop10
	rm -rf mnt2
	
