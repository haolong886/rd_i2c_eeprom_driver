#ifndef _RD_E2PROM_MAIN_H
#define _RD_E2PROM_MAIN_H
#include<linux/module.h>
#include<linux/i2c.h>
#include<linux/err.h>
#include<linux/sysfs.h>
#include<linux/string.h>
#include<linux/delay.h>

#include "rd_e2prom_lib.h"

#define BRIGHT_DBG(...)                  (printk(KERN_INFO __VA_ARGS__))
#define BRIGHT_ERR(...)                  (printk(KERN_ERR __VA_ARGS__))
#define ISERR(err, ...)	 do \
							{ if(err < 0) \
								{ BRIGHT_ERR(__VA_ARGS__); \
							  	  return err;} \
							}while(0)
#endif
