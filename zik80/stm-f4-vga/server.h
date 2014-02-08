
#ifndef h_server_h
#define h_server_h

#define SERVER_QUEUE_MAX 10000

extern uint8_t server_queue [ SERVER_QUEUE_MAX ];
extern volatile uint16_t server_itemcount;

void server_setup ( void );
void server_clear ( void );
void server_insert ( uint8_t b );

#endif
