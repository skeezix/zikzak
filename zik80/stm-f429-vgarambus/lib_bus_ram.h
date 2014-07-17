
#ifndef h_lib_bus_ram_h
#define h_lib_bus_ram_h

void bus_setup ( void ); // sets thigns up, and release bus
void bus_grab_and_wait ( void ); // neither read nor write
void bus_release ( void );

uint8_t bus_perform_read ( uint32_t address );

uint8_t bus_check_cs1 ( void );

#endif
