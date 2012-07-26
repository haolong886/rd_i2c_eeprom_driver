#ifndef _RD_E2PROM_LIB_H
#define _RD_E2PROM_LIB_H
#include "rd_e2prom_main.h"

#define ERR_ARGS		-1
#define	ERR_NO_SYMBLE	-2
extern int find_symble(const char *buf, unsigned int pos);

extern void get_string_from_string(char *data, const char *buf, unsigned int pos, unsigned int num);
extern 
	int convert_e2prom_args(const char *e2prom_args, unsigned int *device_code, 
									unsigned int *addr, unsigned int *value);

#endif
