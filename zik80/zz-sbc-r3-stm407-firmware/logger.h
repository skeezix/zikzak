#ifndef h_logger_h
#define h_logger_h

void log_setup ( void );
void logit ( char *foo ); // append to log queue
char *logpull ( void ); // returns NULL if no more log available
unsigned char logready ( void ); // returns >0 if something is in the log

#endif
