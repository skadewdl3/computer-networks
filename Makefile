all: clean build run

dev: clean build

build:
	gcc ./CRC/crc.c -o ./CRC/crc.sh -lm
clean:
	rm -rf ./CRC/crc.sh
run:
	./CRC/crc.sh