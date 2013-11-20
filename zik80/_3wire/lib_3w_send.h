
#ifndef h_3w_send_h
#define h_3w_send_h

// goal:
// - queue up a 9bit datum
// - repeatedly call a sender function
// - check for queue empty

// only a single 3w is supported for now; adding a void* handle is doable, but
// then we also need setter-functions to specify port# and so on..

// at least at first, this is a VERY SLOW protocol, since its waiting on an ACK from receiver
// for every bit

unsigned char 3w_append ( unsigned char d ); // append d to queue; returns >0 on success

unsigned char 3w_queuesize ( void ); // return size of queue (ie: >0 means data to be sent still)

void 3w_send ( void ); // if somethign in the queue, try sending it

#endif
