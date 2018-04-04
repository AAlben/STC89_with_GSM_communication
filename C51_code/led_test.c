#include <reg52.h>
typedef unsigned int u16;
typedef unsigned char u8;


sbit LSA=P2^2;
sbit LSB=P2^3;
sbit LSC=P2^4;

void main()
{
    LSA=1;
    LSB=0;
    LSC=0;  //¿ØÖÆ38ÒëÂëÆ÷µÄY0Êä³öµÍµçÆ½
    P0=0x6F;
    while(1);
}