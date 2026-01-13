#include "neo6.h"


#define	BAUD_RATE			(9600)
#define FALSE				(0)
#define TRUE				(1)

uint8_t get_time(char *recv_buffer,char *time);
uint8_t get_latitude(char *recv_buffer,char *latitude);
uint8_t get_longitude(char *recv_buffer,char *longitude);

void neo6_init(void)
{
	usart2_rx_init(BAUD_RATE);
}

// Returns TRUE if all fields are found in $GPGGA string
// Returns FALSE if any field is missing
uint8_t neo6_get_data(char *time,char *latitude,char *longitude)
{
	char recv_buffer[NEO6_MAX_BUFFER]={0};
	uint8_t recv_buffer_pos=0;
	char ch;

	do
	{
		ch=usart2_read();
		recv_buffer[recv_buffer_pos]=ch;
		recv_buffer_pos++;
	}while(ch!='\n' && recv_buffer_pos<(NEO6_MAX_BUFFER-1));

	// Check to see if we are in GPGGA string
	if(strstr(recv_buffer,"GPGGA")==NULL) return FALSE;

	if(!get_latitude(recv_buffer,latitude)) return FALSE;
	if(!get_longitude(recv_buffer,longitude)) return FALSE;
	if(!get_time(recv_buffer,time)) return FALSE;

	return TRUE;
}

uint8_t get_time(char *recv_buffer,char *time)
{
	char temp_buffer[TIME_SIZE]={0};
	int8_t time_start_pos=-1;
	int8_t time_stop_pos=-1;

	uint8_t current_pos=0;
	uint8_t num_of_comma=0;
	uint8_t i=0;

	while(recv_buffer[i]!=0)
	{
		if(recv_buffer[i]==',')
		{
			num_of_comma++;
			if(num_of_comma==1)
			{
				time_start_pos=current_pos;
			}

			if(num_of_comma==2)
			{
				time_stop_pos=current_pos;
			}
		}

		i++;
		current_pos++;
	}

	// One or more commas was not found
	if(time_start_pos>time_stop_pos || time_start_pos<0 || time_stop_pos<0)
	{
		return FALSE;
	}

	// If two commas are together then field is empty
	if(time_start_pos==(time_stop_pos-1))
	{
		return FALSE;
	}

	current_pos=++time_start_pos;
	i=0;
	while(current_pos<time_stop_pos)
	{
		temp_buffer[i]=recv_buffer[current_pos];
		current_pos++;
		i++;
	}

	snprintf(time,TIME_SIZE,"%s",temp_buffer);
	return TRUE;
}

uint8_t get_latitude(char *recv_buffer,char *latitude)
{
	char temp_buffer[LATLONG_SIZE]={0};
	int8_t lat_start_pos=-1;
	int8_t lat_stop_pos=-1;
	char lat_direction;

	uint8_t current_pos=0;
	uint8_t num_of_comma=0;
	uint8_t i=0;

	while(recv_buffer[i]!=0)
	{
		if(recv_buffer[i]==',')
		{
			num_of_comma++;
			if(num_of_comma==2)
			{
				lat_start_pos=current_pos;
			}

			if(num_of_comma==3)
			{
				lat_stop_pos=current_pos;
			}

			if(num_of_comma==4)
			{
				lat_direction=recv_buffer[i-1];
				break;
			}
		}

		i++;
		current_pos++;
	}

	// One or more commas was not found
	if(lat_start_pos>lat_stop_pos || lat_start_pos<0 || lat_stop_pos<0)
	{
		return FALSE;
	}

	// If two commas are together then field is empty
	if(lat_start_pos==(lat_stop_pos-1))
	{
		return FALSE;
	}

	// Latitude is positive if North and negative if South
	(lat_direction=='N') ? (lat_direction='+') : (lat_direction='-');

	current_pos=++lat_start_pos;
	i=0;
	while(current_pos<lat_stop_pos)
	{
		temp_buffer[i]=recv_buffer[current_pos];
		current_pos++;
		i++;
	}

	if(strlen(temp_buffer)==10)
	{
		snprintf(latitude,LATLONG_SIZE,"%c0%s",
				 lat_direction,temp_buffer);
	}
	else
	{
		snprintf(latitude,LATLONG_SIZE,"%c%s",
				 lat_direction,temp_buffer);
	}

	return TRUE;
}

uint8_t get_longitude(char *recv_buffer,char *longitude)
{
	char temp_buffer[LATLONG_SIZE]={0};
	int8_t long_start_pos=-1;
	int8_t long_stop_pos=-1;
	char long_direction;

	uint8_t current_pos=0;
	uint8_t num_of_comma=0;
	uint8_t i=0;

	while(recv_buffer[i]!=0)
	{
		if(recv_buffer[i]==',')
		{
			num_of_comma++;
			if(num_of_comma==4)
			{
				long_start_pos=current_pos;
			}

			if(num_of_comma==5)
			{
				long_stop_pos=current_pos;
			}

			if(num_of_comma==6)
			{
				long_direction=recv_buffer[i-1];
				break;
			}
		}

		i++;
		current_pos++;
	}

	// One or more commas was not found
	if(long_start_pos>long_stop_pos || long_start_pos<0 || long_stop_pos<0)
	{
		return FALSE;
	}

	// If two commas are together then field is empty
	if(long_start_pos==(long_stop_pos-1))
	{
		return FALSE;
	}

	// Latitude is positive if East and negative if West
	(long_direction=='E') ? (long_direction='+') : (long_direction='-');

	current_pos=++long_start_pos;
	i=0;
	while(current_pos<long_stop_pos)
	{
		temp_buffer[i]=recv_buffer[current_pos];
		current_pos++;
		i++;
	}

	if(strlen(temp_buffer)==10)
	{
		snprintf(longitude,LATLONG_SIZE,"%c0%s",
				 long_direction,temp_buffer);
	}
	else
	{
		snprintf(longitude,LATLONG_SIZE,"%c%s",
				 long_direction,temp_buffer);
	}

	return TRUE;
}
