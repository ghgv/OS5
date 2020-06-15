#include <stdio.h>
#include <stdarg.h>
#include <common.h>
#include <unistd.h>

void printf(char* str,...)
{
  unsigned int a;
  unsigned long b;
  char s;
  va_list ap;
  va_start(ap, str);
  int i=0;
  while(str[i]!='\0'){
      while(str[i]!='%' && str[i]!='\0' )
      {
        
        putchar(str[i++]);
      }
      if(str[i]=='%' && str[i+1]=='c')
      {
        i++;i++;
        s = va_arg(ap, char);
        putchar(s);
      }
      else if(str[i]=='%' && str[i+1]=='d')
      {
        i++;  i++;
        a = va_arg(ap, int);
        putint(a,10);
      }
      else if(str[i]=='%' && str[i+1]=='i')
      {
        i++;  i++;
        a = va_arg(ap, int);
        putint(a,10);
      }
      else if(str[i]=='%' && str[i+1]=='x')
      {
        i++;  i++;
        a = va_arg(ap, int);
        putint(a,16);
      }
      else if(str[i]=='%' && str[i+1]=='X')
      {
        i++;  i++;
        b = va_arg(ap, unsigned long);
        putlong(b,16);
      }
      else if(str[i]=='%' && str[i+1]=='l')
      {
        i++;  i++;
        b = va_arg(ap, unsigned long);
        putlong(b,10);
      }
  }
  va_end(ap);
}


void exit(int code){
    
}

int getchar() {

    char buf[1];
    int n = read(0, buf, 1);

    while(n < 1)
    {
        n = read(0, buf, 1);
    }
    return buf[0];
}

ssize_t read(int fd, void *buf, size_t count){
    return -1;
}
