#include <reg52.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
# define uchar unsigned char
# define uint unsigned int
    
uchar *receiveData, receive_flag;
sbit led1=P0^1;

uchar haystack[] = "123";
uchar needle[] = "123";

//--------------------------------------------------
unsigned char ReceBuf[128]={0};
int head = 0;
int tail = 0;
unsigned char temp = 0;

//--------------------------------------------------


void Uart1Sends(uchar *str);
void Uart1Send(uchar c);
void DelaySec(uint sec);
uint hand(uchar *str);
uint hand_and_send_message(uchar *str);
void send_message(uchar *content);


void init()
{
    SCON=0X50;          //???????1
    TMOD=0X20;          //?????????2
    PCON=0X80;          //?????
    TH1=0XF3;               //????????,??????4800?
    TL1=0XF3;
    TR1=1;                  //?????  
    
    SM0=0;//??????????,(10?????,?????,????1??????)
    SM1=1;//(??)?????,????????????????
    REN=1;//???????(????sm0sm1??????)
    
    ES=1;                       //??????
    EA=1;                       //?????
    
}






void main()
{
	unsigned char data = 0;
    init();     
    DelaySec(3);
    
        while(1)
        {
             if(head != tail)
			 {
				 data = ReceBuf[tail];
				 tail+=1;
				 if(tail>128)
				 {
					 tail =0;
				 }
				 Uart1Send(data);
				 
				 
			 }
        }


}

void Usart() interrupt 4
{ 
    temp=SBUF;//????????
    RI = 0;//????????? 
	
	ReceBuf[head] = temp ;
	head +=1;
	if(head>128)
	{
		head=0;
	}
    
    
}

void Uart1Send(uchar c)
{
    ES = 0;
    SBUF=c;
    while(!TI);//????????(TI=1)??
    TI=0;  
    ES = 1;
}

void Uart1Sends(uchar *str)
{
    ES = 0;
    while(*str!='\0')
    {
        SBUF=*str;
        while(!TI);//????????(TI=1)??
        TI=0;
        str++;
    }
    ES = 1;
}

void DelaySec(uint sec)
{
    uint i , j= 0;

    for(i=0; i<sec; i++)
    {
        for(j=0; j<65535; j++);
    }        
}

uint hand(uchar *content)
{
        while(1)
        {
                if(receive_flag == 1)
                {                                       
                                receive_flag = 0;
                                Uart1Sends(receiveData);
                                Uart1Send(0x0d);                                        
                                return 1;
                }
        }   

        return 0;
}

uint hand_and_send_message(uchar *str)
{
        while(1)
        {
                        if(receive_flag == 1)
                        {                                       
                                        receive_flag = 0;
                                        send_message(receiveData);
                                        return 1;
                        }
        }   

        return 0;
}

void send_message(uchar *content)
{
        Uart1Sends("AT");           
        Uart1Send(0x0d);              //?????
        Uart1Send(0x0a);  
        DelaySec(2);
        while(!hand("456"));

        Uart1Sends("AT+CMGF=1");            
        Uart1Send(0x0d);              //?????
        Uart1Send(0x0a);  
        DelaySec(2);
        while(!hand("456"));

        Uart1Sends("AT+CSMP?");         
        Uart1Send(0x0d);              //?????
        Uart1Send(0x0a);  
        DelaySec(2);
        while(!hand("456"));

        Uart1Sends("AT+CSCS=\"GSM\"");          
        Uart1Send(0x0d);              //?????
        Uart1Send(0x0a);  
        DelaySec(2);
        while(!hand("456"));

        Uart1Sends("AT+CSMP=17,167,0,250");         
        Uart1Send(0x0d);              //?????
        Uart1Send(0x0a);  
        DelaySec(2);
        while(!hand("456"));

        Uart1Sends("AT+CMGS=\"+8618888888888\"");
        Uart1Send(0x0d);              //?????
        Uart1Send(0x0a);  
        DelaySec(2);
        while(!hand("456"));

        Uart1Sends(*content);
        Uart1Send(0x1A);
        DelaySec(2);
        while(!hand("456"));

        Uart1Sends("over");
        Uart1Send(0x0d);              //?????
        Uart1Send(0x0a);  
        DelaySec(2);
}