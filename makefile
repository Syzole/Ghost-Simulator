FILES = main.o house.o logger.o utils.o

final: ${FILES}
	gcc -g -Wall -Wextra -o final ${FILES}

main.o: main.c defs.h
	gcc -c main.c

house.o: house.c defs.h
	gcc -c house.c

logger.o: logger.c defs.h
	gcc -c logger.c

utils.o: utils.c defs.h
	gcc -c util.c

clean:
	rm -f final ${FILES}
