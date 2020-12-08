msrc:
	make -C ./src
msrcb:
	make -C ./srcb
msrcclean:
	make clean -C ./src
makefilecreate:CreateMakefile.c
	gcc CreateMakefile.c -o create_makefile
makefileinstall:create_makefile
	sudo cp create_makefile /usr/bin/

