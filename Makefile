CC=gcc
CFLAGS=
OUTPUT=build

default:
	$(CC) *.c -o $(OUTPUT) $(CFLAGS)
	sudo chown root $(OUTPUT)
	sudo chmod +s $(OUTPUT)

nosuid:
	$(CC) *.c -o $(OUTPUT) $(CFLAGS)

clean:
	rm -f $(OUTPUT)

