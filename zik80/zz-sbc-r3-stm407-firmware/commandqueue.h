#ifndef h_commandqueue_h
#define h_commandqueue_h

void queue_setup ( void );
void queueit ( char *foo ); // append to log queue
char *queuepull ( void ); // returns NULL if no more log available
unsigned char queueready ( void ); // returns >0 if something is in the log

#endif
