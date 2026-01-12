#include "rylr998.h"

#define FREQ						(915000000)
#define BAUD_RATE					(115200)
#define TXRX_ADDRESS				(6)
#define RESPONSE_MAX_SIZE			(10)
#define SEND_MAX_SIZE				(240)
#define COMMAND_SIZE				(20)

enum Rylr_Error rylr998_tx_init(void)
{
	char buffer[SEND_MAX_SIZE]={0};
	enum Rylr_Error error=0;

	usart1_tx_init(BAUD_RATE);
	usart1_rx_init(BAUD_RATE);

	snprintf(buffer,SEND_MAX_SIZE,"AT+BAND=%d\r\n",FREQ);
	usart1_write_string(buffer);
	error=rylr998_get_response();

	if(error!=OK) return error;

	snprintf(buffer,SEND_MAX_SIZE,"AT+PARAMETER=5,9,1,4\r\n");
	usart1_write_string(buffer);
	error=rylr998_get_response();

	return error;
}

enum Rylr_Error rylr998_rx_init(void)
{
	char buffer[SEND_MAX_SIZE]={0};
	enum Rylr_Error error;

	usart1_tx_init(BAUD_RATE);
	usart1_rx_init(BAUD_RATE);

	snprintf(buffer,SEND_MAX_SIZE,"AT+BAND=%d\r\n",FREQ);
	usart1_write_string(buffer);
	error=rylr998_get_response();

	if(error!=OK) return error;

	snprintf(buffer,SEND_MAX_SIZE,"AT+PARAMETER=5,9,1,4\r\n");
	usart1_write_string(buffer);
	error=rylr998_get_response();

	if(error!=OK) return error;

	snprintf(buffer,SEND_MAX_SIZE,"AT+ADDRESS=%d\r\n",TXRX_ADDRESS);
	usart1_write_string(buffer);
	error=rylr998_get_response();

	return error;
}

enum Rylr_Error rylr998_send(char *data)
{
	uint8_t data_len=strlen(data);
	char buffer[SEND_MAX_SIZE+COMMAND_SIZE]={0};
	enum Rylr_Error error;

	if(data_len>SEND_MAX_SIZE)
	{
		return TX_DATA_OVERFLOW;
	}

	snprintf(buffer,SEND_MAX_SIZE+COMMAND_SIZE,
			"AT+SEND=%d,%d,%s\r\n",
			TXRX_ADDRESS,data_len,data);
	usart1_write_string(buffer);
	error=rylr998_get_response();
	return error;
}

// Not Tested, may contain bugs
enum Rylr_Error rylr998_receive(char *data,uint8_t num)
{
	char buffer[RECV_MAX_SIZE]={0};
	uint8_t ch;
	uint16_t num_actual_recv=0;
	uint8_t num_of_comma=0;

	for(volatile uint16_t i=0;i<RECV_MAX_SIZE;i++)
	{
		ch=usart1_read();
		if(ch=='\r')
		{
			buffer[i]=0x00;
		}
		else if(ch=='\n' || i==(num-1))
		{
			buffer[i]=0x00;
			break;
		}
		else
		{
			buffer[i]=ch;
		}
	}

	if(strstr(buffer,"+RCV")==NULL)
	{
		return UNKNOWN_ERROR;
	}

	for(volatile uint16_t i=0;i<num_actual_recv;i++)
	{
		if(num_of_comma==2 && i<num)
		{
			data[i]=buffer[i];
		}

		if(buffer[i]==',')
		{
			num_of_comma++;
		}

		if(num_of_comma==3 || i==(num-1))
		{
			data[i]=0x00;
			break;
		}
	}

	return OK;
}

enum Rylr_Error rylr998_get_response(void)
{
	char buffer[RESPONSE_MAX_SIZE]={0};
	uint16_t buffer_pos=0;
	uint8_t ch=0;

	while(buffer_pos<RESPONSE_MAX_SIZE)
	{
		// Read ch from usart
		ch=usart1_read();

		if(ch=='\r'|| ch=='\n' || buffer_pos==(RESPONSE_MAX_SIZE-1))
		{
			buffer[buffer_pos]=0x00;
		}
		else
		{
			buffer[buffer_pos]=ch;
		}

		buffer_pos++;
		if(ch=='\n') break;
	}

	if(strstr(buffer,"OK")!=NULL)
	{
		return OK;
	}
	else if (strstr(buffer,"ERR")!=NULL)
	{
		char *num_str=strstr(buffer,"=");
		int num;

		num_str++;
		num=atoi(num_str);

		return num;
	}
	else
	{
		return UNKNOWN_ERROR;
	}
}
