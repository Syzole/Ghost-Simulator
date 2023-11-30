FILES = main.o house.o Ghost.o Hunter.o logger.o utils.o

final: ${FILES}
	gcc -g -Wall -Wextra -o final ${FILES}

main.o: main.c defs.h
	gcc -c main.c

house.o: house.c defs.h
	gcc -c house.c

Ghost.o: Ghost.c defs.h
	gcc -c Ghost.c

Hunter.o: Hunter.c defs.h
	gcc -c Hunter.c

logger.o: logger.c defs.h
	gcc -c logger.c

utils.o: utils.c defs.h
	gcc -c utils.c

clean:
	rm -f final ${FILES}
