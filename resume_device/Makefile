obj-m += resume.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	chmod +x dev_load
	chmod +x dev_unload

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	chmod -x dev_load
	chmod -x dev_unload
	
