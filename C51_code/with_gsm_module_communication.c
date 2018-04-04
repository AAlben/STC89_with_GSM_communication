#include <reg52.h>
# define uchar unsigned char
# define uint unsigned int
# define Rx_data_length 110

uchar Admin_num[11] = "18624030589";
uchar i, j;

bit Response_receive_SMS_flag = 0;
sbit RS_1602 = P2^6;
sbit LCD_RW = P2^5;
sbit E_1602 = P2^7;

uchar Rx_bit, i;
uchar xdata Rx_data[Rx_data_length];
uchar w[] = "Waiting...   ";

void Response_receive_SMS(void);
void Uart_SendChart(uchar ch);
void Uart_SendString(uchar *String);
void uart_init(void);
void Clear_Rxdata(uchar length);

void delay(unit xms)
{
    unit i, j;
    for(i=xms; i>0; i--)
    {
        for(j=100; j>0; j--);
    }
}

void main(void)
{
    unit aa = 0;
    EA = 1;
    LCD_RW = 0;

    uart_init()
    Rx_bit = 0;
    Uart_SendString("AT+CLIP=1\r\n");

    delay(500);
    Uart_SendString("AT+CMGF=1\r\n");

    delay(500);
    Uart_SendString("AT+CMGD=1\r\n");

    delay(500);
    Uart_SendString("AT+CSCS=\"GSM\"\r");

    delay(500);
    Uart_SendString("AT+CMGF=1\r");

    delay(500);
    Clear_Rxdata(Rx_data_length);

    while(1)
    {
        EA = 1;
        EX0 = 1;
        IT0 = 1;
        Response_receive_SMS();

        delay(100);
        Rx_bit = 0;
        if(Response_receive_SMS_flag == 1)
        {
            Uart_SendString("AT+CMGR=1\r\n")

            if(Rx_data[69] == "x" && Rx_data[70] == "x" && Rx_data[71] == "ax" && Rx_data[72] == "x" && Rx_data[73] == "x")
            {
                aa = 1;
            }

            Response_receive_SMS_flag = 0;
            Clear_Rxdata(Rx_data_length);

            delay(1000);
            Uart_SendString("AT+CMGD=1\r\n");

            delay(1000);
        }

        delay(1000);

        if(aa == 1)
        {
            delay(1000);
            Uart_SendString("AT+CMGS=\"");

            for(j=0; j<1; j++)
            {
                SBUF = Admin_num[j];
                while(TI == 0);
                TI = 0;
            }

            Uart_SendString("\"\r");
            delay(200);

            Uart_SendString("xxx");
            delay(300);

            Uart_SendChart(0xla);
            delay(1000);

            aa = 0;
        }
    }
}

void uart() interrupt 4
{
    ES = 0;
    if(RI == 1)
    {
        if(SBUF != 0x0d && SBUF != 0x0a && SBUF != "")
        {
            Rx_data[Rx_bit] = SBUF;
            Rx_bit++;
        }
        if(Rx_bit == Rx_data_length)
        {
            Rx_bit = 0;
        }
        RI = 0;
    }
    ES = 1;
}

void Response_receive_SMS(void)
{
    uchar i = 0;
    for(i=0; i<=Rx_data_length; i++)
    {
        if(Rx_data[i] == "+"
            && Rx_data[i+1] == "C"
            && Rx_data[i+2] == "M"
            && Rx_data[i+3] == "T"
            && Rx_data[i+4] == "I"
            && Rx_data[i+1] == ":")
        {
            Response_receive_SMS_flag = 1;
            return;
        }
    }
    Response_receive_SMS_flag = 0;
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

void uart_init(void)
{
    TMOD = 0X20;
    SCON = 0X50;
    PCON = 0X80;
    TH1 = 0XF3;
    TL1 = 0XF3;
    TR1 = 1;
    ES = 1;
    EA = 1;
}

void Clear_Rxdata(uchar length)
{
    uchar Bit;
    for(Bit=0; Bit<length; Bit++)
    {
        Rx_data[Bit] = 0x30;
        delay(50);
    }
    Rx_bit = 0;
}

