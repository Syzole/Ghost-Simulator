CCFLAGS = gcc -g -Wall -Wextra
FILES = main house logger utils

all: ${FILES}

main: main.c
	${CCFLAGS} -o main main.c

house: house.c
	${CCFLAGS} -o house house.c

logger: logger.c
	${CCFLAGS} -o logger logger.c

utils: utils.c
	${CCFLAGS} -o util util.c

clean:
	rm -f ${FILES}
