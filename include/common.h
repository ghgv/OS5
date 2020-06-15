#ifndef _COMMON_H
#define _COMMON_H
#include <types.h>
#include <stdarg.h>
#define asmlinkage CPP_ASMLINKAGE __attribute__((regparm(0)))

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
static unsigned short* videomemory  =(unsigned short*)0xb8000;
static int x=0,y=0;


void putchar(char str);
void putint(int  num, int base);
void putlong(unsigned long  num, int base);
void swap(char *a, char *b);
void reverse(char *str[], int length);
uint8_t inb(uint16_t _port);
void outb(uint16_t _port, uint8_t _data);
void memset(uint8_t *dest, uint8_t val, uint32_t len);
void changer();
void update_cursor(int x, int y);
unsigned long getticks(void);


#define PS2 0x60

struct multiboot
{
	uintptr_t flags;
	uintptr_t mem_lower;
	uintptr_t mem_upper;
	uintptr_t boot_device;
	uintptr_t cmdline;
	uintptr_t mods_count;
	uintptr_t mods_addr;
	uintptr_t num;
	uintptr_t size;
	uintptr_t addr;
	uintptr_t shndx;
	uintptr_t mmap_length;
	uintptr_t mmap_addr;
	uintptr_t drives_length;
	uintptr_t drives_addr;
	uintptr_t config_table;
	uintptr_t boot_loader_name;
	uintptr_t apm_table;
	uintptr_t vbe_control_info;
	uintptr_t vbe_mode_info;
	uintptr_t vbe_mode;
	uintptr_t vbe_interface_seg;
	uintptr_t vbe_interface_off;
	uintptr_t vbe_interface_len;
} __attribute__ ((packed));

#endif
