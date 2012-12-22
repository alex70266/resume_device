ver.20121220
RE-WRITE AUTHOR: Yu-Yang Lin    alex70266


Recommand environment:Ubuntu with kernel version 2.6 or later than

Opperation Flow: 
	su root (need root password)
	make
	./dev_load
	echo "(your name)" > /dev/resume
	dmesg | tail -5
	cat /dev/resume
	dmesg | tail -5
	make clean
	./dev_unload



Makefile
	make to compile module and create resume.ko..etc
	make clean to clear resume.ko..etc


dev_load 
	./dev_load to insert resume.ko module and create /dev/resume
	
dev_unload
	./dev_load to remove resume module and rm /dev/resume

clear directory
	 make to compile clean_mem.c
	 make clean to clear clean_mem_nonblock 
	
	./clean_mem-nonblock to clear the memory "Message" of resume module


