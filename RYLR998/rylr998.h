#ifndef RYLR998_H_
#define RYLR998_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "usart.h"

#define RECV_MAX_SIZE				(270)

enum Rylr_Error
{
	OK=0,
	NO_ENTER=1,
	NO_AT=2,
	UNKNOWN_COMMAND=4,
	DATA_LEN_MISMATCH=5,
	TX_OVER_TIMES=10,
	CRC_ERROR=12,
	TX_DATA_OVERFLOW=13,
	FLASH_WRITE_ERROR=14,
	UNKNOWN_ERROR=15,
	LAST_TX_NOT_COMPLETED=17,
	PREAMBLE_VALUE_INVALID=18,
	RX_FAILED=19,
	SMART_RECEIVE_ERROR=20
};

enum Rylr_Error rylr998_tx_init(void);
enum Rylr_Error rylr998_rx_init(void);
enum Rylr_Error rylr998_send(char *data);
enum Rylr_Error rylr998_receive(char *data,uint8_t num);
enum Rylr_Error rylr998_get_response(void);

#endif /* RYLR998_H_ */
