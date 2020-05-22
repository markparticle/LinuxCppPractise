CC := gcc
Obj = read_proc.o
TAR = read_proc

$(TAR): $(Obj)
	$(CC) $(Obj) -o $(TAR)
%.o: %.c
	$(CC) -c %.c -o %.o

.PHONY: clean
clean:
	rm -rf $(Obj) $(TAR)