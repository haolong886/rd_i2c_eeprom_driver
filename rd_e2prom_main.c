
#include "rd_e2prom_main.h"
#define DAC081_ADDR		0
#define I2C_REG_SIZE	5
#define INT_34XX_SYS_NIRQ 7

typedef struct
{
	struct kobject *kobj;
	struct i2c_client *client;
}eeprom_data;
eeprom_data *eeprom;

#define FORMAT_I2C	0
#define FORMAT_ARM  1
//static void cvt_data_format(char const *data, int format)
//{
//	char *d = data;
//	char tmp = 0;
//	if(format == FORMAT_I2C)
//	{
//		tmp = *(d+1);
//		*(d+1) = *d;
//		*d = tmp;
//	}
//	if(format == FORMAT_ARM)
//	{
//		tmp = *(d);
//		*d = *(d+1);
//		*(d+1) = tmp;
//	}
//}
#define EEPROM_SIZE	128
static ssize_t eeprom_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	char *s=buf;
	struct i2c_client *client;
	unsigned int addr=0;
	int err = 0;
	client = eeprom->client;
//	err = sscanf(buf, "%x", addr);
//	ISERR(err,"[eeprom show]:sscanf addr err\n");
	for(; addr<EEPROM_SIZE; addr++)
	{
		err = i2c_smbus_read_byte_data(client, addr);
		if(err < 0)
		{
			BRIGHT_ERR("receive from eeprom error, err value = %d\n", err);
			return err;
		}
		BRIGHT_DBG("read from i2c addr = %d, value = %x\n", addr, err);
//		sprintf(s+addr, "%02x", (unsigned int)err);
//		BRIGHT_DBG("sprintf addr = %d, value = %s\n", (unsigned int)(s+addr), s+addr);
	}
	return EEPROM_SIZE;//byte
}

static ssize_t eeprom_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t n)
{
	unsigned int device_code, addr, value;
	int err = 0;
	struct i2c_client *client;
	client = eeprom->client;
	err = convert_e2prom_args(buf, &device_code, &addr, &value);
	ISERR(err, "convert e2prom args error\n");
	BRIGHT_DBG("addr = %x, value = %x\n", addr, value);
	err = i2c_smbus_write_byte_data(client, addr, value);
	ISERR(err, "send to e2prom error\n");
	return n;
}

static struct kobj_attribute kobj_attr_eeprom =
{
	.attr =
	{
		.name = __stringify(eeprom_ctl),
		.mode = 0666,
	},
	.show = eeprom_show,
	.store = eeprom_store,
};

static struct attribute *attr_g[] =
{
	&kobj_attr_eeprom.attr,
	NULL,
};

static struct attribute_group grp_attr =
{
	.attrs = attr_g,
};

static int eeprom_probe(struct i2c_client *client,
			const struct i2c_device_id *dev_id)
{
	struct kobject *kobj;
	int err = 0;
	if (!(err = i2c_check_functionality(client->adapter, I2C_FUNC_SMBUS_WORD_DATA)))
		goto exit;
	
	dev_info(&client->dev, "chip found\n");
	eeprom = kzalloc(sizeof(eeprom_data), GFP_KERNEL);
	kobj = kobject_create_and_add("eeprom", NULL);
	if(!kobj)
	{
		BRIGHT_ERR("eeprom kobj didn't creat\n");
		err = (int)kobj;
		goto exit_kobj;
	}
	eeprom->kobj = kobj;
	err = sysfs_create_link(kobj, &client->dev.kobj, "eeprom_link");
	if(err != 0)
		goto exit_link;
	err = sysfs_create_group(&client->dev.kobj, &grp_attr);
	if(err)
		goto exit_sysfs;
	eeprom->client = client;
	
	
	return 0;
	
	exit_sysfs:
		sysfs_remove_link(kobj, "eeprom_link");
	exit_link:
		//delete kobject
		kobject_del(kobj);
		kobject_put(kobj);
	exit_kobj:
		kfree(eeprom);
	exit:
		return err;
}

static int __devexit eeprom_remove(struct i2c_client *client)
{
	sysfs_remove_group(&client->dev.kobj, &grp_attr);
	sysfs_remove_link(eeprom->kobj, "eeprom_link");
	kobject_del(eeprom->kobj);
	kobject_put(eeprom->kobj);
	kfree(eeprom);
	return 0;
}


static const struct i2c_device_id eeprom_id[] = {
	{ "eeprom", 0 },
	{}
};

static struct i2c_driver eeprom_driver =
{
	.driver = 
	{
		.name = "eeprom",
	},
	.probe = eeprom_probe,
	.remove = __devexit_p(eeprom_remove),
	.id_table = eeprom_id,
};

static int __init eeprom_init(void)
{
	BRIGHT_DBG("eeprom init\n");
	return i2c_add_driver(&eeprom_driver);
}

module_init(eeprom_init);

static void __exit eeprom_exit(void)
{
	return i2c_del_driver(&eeprom_driver);
}

module_exit(eeprom_exit);
MODULE_LICENSE("GPL");

