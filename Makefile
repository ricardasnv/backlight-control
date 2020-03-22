CC=gcc

default:
	$(CC) *.c -o build

suid:
	$(CC) *.c -o build
	sudo chown root build
	sudo chmod +s build
