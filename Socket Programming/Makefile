all: clean buildserver buildclient run

dev: clean buildserver buildclient

buildclient:
	gcc ./client.c -o ./client.sh -lm

buildserver:
	gcc ./server.c -o ./server.sh -lm

clean:
	rm -rf ./client.sh ./server.sh

run:
	./server.sh && ./client.sh