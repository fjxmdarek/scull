obj-m := skull.o

skull-objs := scull.o scull_dev_register.o scull_open_release.o scull_read_write_seek.o scull_helpers.o

KDIR := ~/kernel/SSD2351

PWD := $(shell pwd)

EXTRA_CFLAGS += -I$(PWD)

default:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

run:
	./scull_load.sh