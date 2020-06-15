#include <stdio.h>
#include <common.h>
#include <string.h>
#include <unistd.h>

uint8_t inb(uint16_t _port)
{
                    uint8_t result;
                    __asm__ volatile("inb %1, %0" : "=a" (result) : "Nd" (_port));
                    return result;
}

void outb(uint16_t _port, uint8_t _data)
{
                    __asm__ volatile("outb %0, %1" : : "a" (_data), "Nd" (_port));
                       //asm volatile ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

void memset(uint8_t *dest, uint8_t val, uint32_t len)
{
    uint8_t *temp = (uint8_t *)dest;
    for ( ; len != 0; len--) *temp++ = val;
    //printf("$");
}

void *memcpy(uint8_t *dest,  uint8_t *src, uint32_t len)
{
    uint32_t i=0;    
    uint8_t *sp = ( uint8_t *)src;
    uint8_t *dp = (uint8_t *)dest;
    for(; len != 0; len--) 
        {
            *dp++ = *sp++;i++;
        }
    return dest;
}

void swap(char *a, char *b)
{
 char  temp = *a;
 *a = *b;
 *b = temp;
}

void reverse(char *str[], int length) 
{ 
    int start = 0; 
    int end = length -1; 
    while (start < end) 
    { 
        swap(*(str+start), *(str+end)); 
        start++; 
        end--; 
    } 
} 

void putint(int  num, int base){
    char s[10];
    char temp;
    int k=0;
    if(num!=0){
        while(num!=0){
            int rem=num%base;
            s[k++] = (rem > 9)? (rem-10) + 'A' : rem + '0'; 
            num = num/base; 
        }
        int j = k - 1; 
        for (int i = 0; i <j; i++) { 
            temp = s[i]; 
            s[i] = s[j]; 
            s[j] = temp; 
            j--; 
        } 
        for(int u=0;u<k;u++){
        videomemory[y*VGA_WIDTH+x]=(videomemory[y*VGA_WIDTH+x] & 0xFF00)| s[u];
        if(x>(VGA_WIDTH-1)){
            y++;
            x=0;
        }
        if(y>VGA_HEIGHT)
            changer();
        x++;
        }
    }
    else
        {
            videomemory[y*VGA_WIDTH+x]=(videomemory[y*VGA_WIDTH+x] & 0xFF00)| '0';
            if(x>(VGA_WIDTH-1)){
                y++;
                x=0;
            }
            if(y>VGA_HEIGHT)
                changer();
            x++;
        }
    update_cursor(x, y);
}

void putlong(unsigned long  num, int base){
    char s[20];
    char temp;
    int k=0;
    if(num!=0){
        while(num!=0){
            unsigned long rem=num%base;
            s[k++] = (rem > 9)? (rem-10) + 'A' : rem + '0'; 
            num = num/base; 
        }
        int j = k - 1; 
        for (int i = 0; i < j; i++) { 
            temp = s[i]; 
            s[i] = s[j]; 
            s[j] = temp; 
            j--; 
        } 
        for(int u=0;u<(k);u++){
            videomemory[y*VGA_WIDTH+x]=(videomemory[y*VGA_WIDTH+x] & 0xFF00)| s[u];
            if(x>(VGA_WIDTH-1)){
                y++;
                x=0;
            }
            if(y>VGA_HEIGHT)
                changer();
            x++;
            }
        }
       else
        {
            videomemory[y*VGA_WIDTH+x]=(videomemory[y*VGA_WIDTH+x] & 0xFF00)| '0';
            if(x>(VGA_WIDTH-1)){
                y++;
                x=0;
            }
            if(y>VGA_HEIGHT){
                changer();
            }
            x++;
        }
   update_cursor(x, y);         
}


void putchar(char str){

  if(str==0xA){
      y++;
      x=0;   
  }
  else{
  videomemory[y*VGA_WIDTH+x]=(videomemory[y*VGA_WIDTH+x] & 0xFF00)| str;

   if(x>(VGA_WIDTH-1)){
          y++;
          x=0;
      }
    if(y>VGA_HEIGHT){
        changer();
        x--;
    }
   x++; 
   }
   update_cursor(x, y);
}

void changer(){
    y=24;
    x=0;
    unsigned short buffer[VGA_WIDTH*VGA_HEIGHT];
    for(int i=0;i<(VGA_WIDTH*VGA_HEIGHT);i++)
        buffer[i]=0;
    for(int i=0;i<(VGA_WIDTH*VGA_HEIGHT);i++)
        buffer[i]=videomemory[i];
    for(int i=0;i<(VGA_WIDTH*24);i++)
        videomemory[i]=buffer[VGA_WIDTH+i];
    for(int i=(VGA_WIDTH*24);i<(VGA_WIDTH*VGA_HEIGHT);i++)
        videomemory[i]=(videomemory[i] & 0xFF00) | ' ';
    update_cursor(x, y);
}

void update_cursor(int x, int y)
{
	uint16_t pos = y * VGA_WIDTH + x;
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

unsigned long getticks(void)
{
     unsigned long  ret;
     __asm__ __volatile__("rdtsc": "=A" (ret));
     return ret;
}
