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

void outw(uint16_t _port, uint32_t _data)
{
                        __asm__ volatile("out %%eax,    %%dx\n\t"
                                :: "a" (_data), "d" (_port)
                        );
}

uint32_t inw(uint16_t _port)
{
                        uint32_t result;
                        __asm__ volatile("in    %%dx, %%eax\n\t"
                        : "=a" (result)
                        : "d" (_port)
                        );
                        return result;
                        
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
            changer(0,24);
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
                changer(0,24);
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
                changer(0,24);
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
                changer(0,24);
            }
            x++;
        }
   update_cursor(x, y);         
}


void putchar(unsigned char str){
  if(str == 128){
      y--;
      goto salir;
  }
  if(str == 129){
      y++;
      if(y>24){
          changer(x,24);
        }
      goto salir;
  }
  if(str == 130){
      x--;
      goto salir;
  }
  if(str == 131){
      x++;
      goto salir;
  }
  if(str=='\n'){
      y++;
      x=0;   
  }
  else{
  videomemory[y*VGA_WIDTH+x]=(videomemory[y*VGA_WIDTH+x] & 0xFF00)| str;

   if(x>(VGA_WIDTH-1)){
          y++;
          x=0;
      }
    if(y>(VGA_HEIGHT-1)){
        changer(0,24);
        x--;
    }
   x++; 
   }
   salir:
   update_cursor(x, y);
}

void changer(int X, int Y){
    y=Y;
    x=X;
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
    char X[4],Y[4];
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
    
    itoa(x+1,X,10);
    itoa(y+1,Y,10);
    videomemory[0]=(videomemory[0] & 0xFF00) | X[0];
    videomemory[1]=(videomemory[1] & 0xFF00) | X[1];
    videomemory[2]=(videomemory[2] & 0xFF00) | ':';
    videomemory[3]=(videomemory[3] & 0xFF00) | Y[0];
    videomemory[4]=(videomemory[4] & 0xFF00) | Y[1];
    
}

unsigned long getticks(void)
{
     unsigned long  ret;
     __asm__ __volatile__("rdtsc": "=A" (ret));
     return ret;
}
// function to reverse buffer[i..j]
char* reverse2(char *buffer, int i, int j)
{
    while (i < j)
        swap(&buffer[i++], &buffer[j--]);
 
    return buffer;
}

// Iterative function to implement itoa() function in C
char* itoa(int value, char* buffer, int base)
{
    // invalid input
    if (base < 2 || base > 32)
        return buffer;
 
    // consider absolute value of number
    int n = value;
 
    int i = 0;
    while (n)
    {
        int r = n % base;
 
        if (r >= 10) 
            buffer[i++] = 65 + (r - 10);
        else
            buffer[i++] = 48 + r;
 
        n = n / base;
    }
 
    // if number is 0
    if (i == 0)
        buffer[i++] = '0';
 
    // If base is 10 and value is negative, the resulting string 
    // is preceded with a minus sign (-)
    // With any other base, value is always considered unsigned
    if (value < 0 && base == 10)
        buffer[i++] = '-';
 
    buffer[i] = '\0'; // null terminate string
 
    // reverse the string and return it
    return reverse2(buffer, 0, i - 1);
}
