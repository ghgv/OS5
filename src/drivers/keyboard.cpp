#include <drivers/keyboard.h>

char asccode[58][2] =       /* Array containing ascii codes for
			       appropriate scan codes */
     {
       {   0,0   } ,
       {  1 , 1  } , /*ESC*/
       { '1','!' } ,
       { '2','"' } ,
       { '3','#' } ,
       { '4','$' } ,
       { '5','%' } ,
       { '6','^' } ,
       { '7','&' } ,
       { '8','*' } ,
       { '9','(' } ,
       { '0',')' } ,
       { '`','_' } ,
       { '?','+' } ,
       {   8,8   } ,
       {   9,9   } ,
       { 'q','Q' } ,
       { 'w','W' } ,
       { 'e','E' } ,
       { 'r','R' } ,
       { 't','T' } ,
       { 'y','Y' } ,
       { 'u','U' } ,
       { 'i','I' } ,
       { 'o','O' } ,
       { 'p','P' } ,
       { '`','{' } ,
       { '+','}' } ,
       {  13,13  } ,
       {   0,0   } ,
       { 'a','A' } ,
       { 's','S' } ,
       { 'd','D' } ,
       { 'f','F' } ,
       { 'g','G' } ,
       { 'h','H' } ,
       { 'j','J' } ,
       { 'k','K' } ,
       { 'l','L' } ,
       { ';',':' } ,
       {  39,34  } ,
       { '`','~' } ,
       {   0,0   } ,
       { '\\','|'} ,
       { 'z','Z' } ,
       { 'x','X' } ,
       { 'c','C' } ,
       { 'v','V' } ,
       { 'b','B' } ,
       { 'n','N' } ,
       { 'm','M' } ,
       { ',','<' } ,
       { '.','>' } ,
       { '-','?' } ,
       {   0,0   } ,
       {   0,0   } ,
       {   0,0   } ,
       { ' ',' ' } ,
   };



static void keyboard_callback(registers_t regs)
{
     
   unsigned char scancode;
   scancode = inb(0x60);
   
   if (scancode & 0x80)
   {
    //printf("%c",asccode[scancode][1]);   
    }
   else{
    if(asccode[scancode][0]!='\r')   
        printf("%c",asccode[scancode][0]);
    else
        printf("\n");
   }
   
}

void init_keyboard(uint32_t frequency)
{
   printf("Initializing keyboard..\n");
   register_interrupt_handler(IRQ1, &keyboard_callback);
   printf("Keyboard registered.\n");
   //(manager, 0x21),
   //dataport(0x60),
    //commandport(0x64)
   
}
