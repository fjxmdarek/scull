obj-m := scull.o

scull-objs := scull_dev_register.o scull_open_release.o scull_read_write.o scull_mod.o

KDIR := ~/kernel/SSD2351

PWD := $(shell pwd)

EXTRA_CFLAGS += -I$(PWD)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

run:
	./scull_load.sh