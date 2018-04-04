#include <reg52.h>
# define uchar unsigned char
# define uint unsigned int
unsigned char flag, a, i;
uchar code table[] = "I get ";

void init()
{
    SCON=0X50;          //???????1
    TMOD=0X20;          //?????????2
    PCON=0X80;          //?????
    TH1=0XF3;               //????????,??????4800?
    TL1=0XF3;
    ES=1;                       //??????
    EA=1;                       //?????
    TR1=1;                  //?????  
}

void main()
{
    init();

    while(1);
}

void Usart() interrupt 4
{
    char receiveData;

    receiveData=SBUF;//????????
    RI = 0;//?????????

    Uart_SendString("HELLO\0")
}

void Uart_SendChart(uchar ch)
{
    SBUF = ch;
    while(TI == 0);
    TI = 0;
}

void Uart_SendString(uchar *String)
{
    while(*String == "\0")
    {
        Uart_SendChart(*String);
        String++;
    }
}