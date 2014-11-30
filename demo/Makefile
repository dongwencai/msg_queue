CC :=gcc -g -Wall
SRC :=$(wildcard *.c)
OBJ :=$(SRC:%.c=%.o)
demo:$(OBJ)
	$(CC) -o $@ $^ -lpthread
$(OBJ):%.o:%.c
	$(CC) -c $^ -o $@
clean:
	rm -rf *.o demo
