#include <reg52.h>
#include <string.h>
# define uchar unsigned char
# define uint unsigned int

unsigned char ReceBuf[84]={0};
unsigned char message_Buf[1]={0};
int head = 0;
int tail = 0;
int flag = 0;
int enter_index = 0;
int message_content_index = 0;
int message_start_index = 0;
int hand_result = 0;
int while_index = 0;
unsigned char temp = 0;
unsigned char data_str;

void Uart1Send(uchar c);
void Uart1Sends(uchar *str);
void DelaySec(uint sec);
uint hand(void);
void clear_rece_buf(void);
void clear_message_buf(void);
uint receive_message();
void send_message(uchar *content);
uint show_message();
uint exec_AT_command(uchar *command);


void init()
{
    SCON=0X50;
    TMOD=0X20;
    PCON=0X80;
    TH1=0XF3;
    TL1=0XF3;
    TR1=1;
    
    SM0=0;
    SM1=1;
    REN=1;
    
    ES=1;
    EA=1;    
}

void main()
{       
    init();     
    DelaySec(3);

    while(1)
    {
        while(!exec_AT_command("AT"));
        while(!exec_AT_command("AT+CMGF=1"));
        while(!exec_AT_command("AT+CPMS=\"SM\""));

        while(!enter_index)
        {
            while_index += 1;
            Uart1Sends("AT+CMGR=1");
            Uart1Send(0x0d);
            Uart1Send(0x0a);
            DelaySec(3);    
            while(!receive_message());
            if(while_index == 10)
            {
                enter_index = 1;
            }
            clear_rece_buf();
            clear_message_buf();  
        }
        if(while_index == 10)
        {
            enter_index = 0;
            while_index = 0;
            continue;
        }
        enter_index = 0;
        while(!show_message());
        clear_rece_buf();
        clear_message_buf();  

        Uart1Sends("over");
    }
}

void Usart() interrupt 4
{ 
    temp=SBUF;//????????
    RI = 0;//????????? 
    
    ReceBuf[head] = temp;
    head++;
    if(head>84)
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
        for(j=0; j<62535; j++);
    }        
}

uint hand()
{
    while(1)
    {
        if(head != tail)
        {
            data_str = ReceBuf[tail];
            tail++;
            if(tail>84)
            {
                tail = 0;
            }
            if(data_str == 'O' && ReceBuf[tail] == 'K')
            {
                return 1;
            }
        }
        else
        {
            return 0;
        }
    }
}

uint receive_message()
{
    while(1)
    {
        if(head != tail)
        {
            Uart1Sends("[");
            Uart1Sends(ReceBuf);
            Uart1Sends("]");
            Uart1Send(0x0d); 
            // if(data_str == '+')
            // {
            //     // tail += 1;
            //     // data_str = ReceBuf[tail];
            //     enter_index++;
            // }
            // if(enter_index >= 1)
            // {
            //     if(data_str == 0x0a && ReceBuf[tail] == 'O' && ReceBuf[tail + 1] == 'K')
            //     {
            //         return 1;
            //     }
            //     message_Buf[message_content_index] = data_str;
            //     message_content_index++;
            // }           
        }
        else
        {
            return 1;
        }
    }    
}

uint show_message()
{    
    while(1)
    {
        if(message_content_index != message_start_index)
        {
            // send_message(message_Buf);
            Uart1Sends(message_Buf);
            return 1;
        }
        else
        {
            return 1;
        }
    }
}


void clear_rece_buf()
{
    for(flag=0; flag<84; flag++)
    {
        ReceBuf[flag] = 0;
    }
    head = 0;
    tail = 0;
}

void clear_message_buf()
{
    for(flag=0; flag<32; flag++)
    {
        message_Buf[flag] = 0;
    }
    message_start_index = 0;
    message_content_index = 0;
}

void send_message(uchar *content)
{
    Uart1Sends("AT");
    Uart1Send(0x0d); 
    Uart1Send(0x0a);  
    DelaySec(2);
    while(!hand());
    clear_rece_buf();

    Uart1Sends("AT+CMGF=1");
    Uart1Send(0x0d); 
    Uart1Send(0x0a);  
    DelaySec(2);
    while(!hand());
    clear_rece_buf();

    Uart1Sends("AT+CSMP?");
    Uart1Send(0x0d); 
    Uart1Send(0x0a);  
    DelaySec(2);
    while(!hand());
    clear_rece_buf();

    Uart1Sends("AT+CSCS=\"GSM\"");
    Uart1Send(0x0d); 
    Uart1Send(0x0a);  
    DelaySec(2);
    while(!hand());
    clear_rece_buf();

    Uart1Sends("AT+CSMP=17,167,0,250");
    Uart1Send(0x0d); 
    Uart1Send(0x0a);  
    DelaySec(2);
    while(!hand());
    clear_rece_buf();

    Uart1Sends("AT+CMGS=\"+8618888888888\"");
    Uart1Send(0x0d); 
    Uart1Send(0x0a);  
    DelaySec(2);
    while(!hand());
    clear_rece_buf();

    Uart1Sends(content);
    Uart1Send(0x1A); 
    DelaySec(2);
    while(!hand());
    clear_rece_buf();

    Uart1Sends("over");
    Uart1Send(0x0d); 
    Uart1Send(0x0a); 
    DelaySec(2);
    while(!hand());
    clear_rece_buf();
}

uint exec_AT_command(uchar *command)
{
    Uart1Sends(command);
    Uart1Send(0x0d);
    Uart1Send(0x0a);  
    DelaySec(2);
    hand_result = hand();
    clear_rece_buf(); 

    return hand_result;
}