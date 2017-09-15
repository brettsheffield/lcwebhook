CC=gcc
CCOPTS=-Wall -Werror -g
PROGRAM=lcwebhook
LIBS=-llibrecast
COMMON_OBJECTS=${PROGRAM}.o cJSON.o

${PROGRAM}: ${COMMON_OBJECTS}
	${CC} ${CCOPTS} -o ${PROGRAM} ${COMMON_OBJECTS} ${LIBS}

$(PROGRAM).o: lcwebhook.c
	${CC} ${CCOPTS} -c ${PROGRAM}.c ${LIBS}

cJSON.o: cJSON.c cJSON.h
	${CC} ${CCOPTS} -c cJSON.c

.PHONY: clean

clean:
	rm -f *.o ${PROGRAM}
