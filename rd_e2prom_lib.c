#include "rd_e2prom_lib.h"


int find_symble(const char *buf, unsigned int pos)
{
	int i = pos;
	while(buf[i]!='\0' && buf[i]!=',')
	{
		i++;
	}
	if(buf[i] == ',')
		return i;
	else
		return ERR_NO_SYMBLE;
}

void get_string_from_string(char *to, const char *from, unsigned int pos, unsigned int num)
{
	int n = 0;
	for(; n< num; n++)
	{
		to[n] = from[n+pos];
	}
	to[num] = '\0';
}

int check_args(const char *buf)
{
	if(buf[2] != ',' || (buf[4] !=',' && buf[5] != ',' && buf[6] != ',') || buf[8] != '\n')
		return -1;
	else
		return 0;
}
#define DEVICE_CODE_POS	0
#define DEVICE_CODE_BYTES	2
#define ADDR_POS		3
#define ADDR_BYTES		2
#define VALUE_POS		6
#define VALUE_BYTES		2
int convert_e2prom_args(const char *e2prom_args, unsigned int *device_code, 
								unsigned int *addr, unsigned int *value)
{
	int i=0, err = 0;
	int pos[3]={0};
	int data[3]={0};
	char temp[3];
	for(; i<2; i++)
	{
		pos[i] += 1;
		pos[i+1] = find_symble(e2prom_args, pos[i]);
		ISERR(pos[i+1], "args format error\n");
		get_string_from_string(temp, e2prom_args, pos[i], pos[i+1]-pos[i]);
//		BRIGHT_DBG("temp = %s\n", temp);
		err = sscanf(temp, "%d", data+i);
//		BRIGHT_DBG("data[%d] hex = %x\n", i, data[i]);
	}
	get_string_from_string(temp, e2prom_args,  pos[2]+1, VALUE_BYTES);
//	BRIGHT_DBG("temp = %s\n", temp);
	err = sscanf(temp, "%x", data+2);
//	BRIGHT_DBG("data[%d] hex = %x\n", 2, data[2]);
	*device_code = data[0];
	*addr = data[1];
	*value = data[2];
	return 0;	
}

