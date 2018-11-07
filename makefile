CC=gcc
TARGET=sim

$(TARGET): main.o producer.o consumer.o
	$(CC) -o $(TARGET) main.o producer.o consumer.o

main.o: main.c
	$(CC) -c main.c

producer.o: producer.c
	$(CC) -c producer.c

consumer.o: consumer.c
	$(CC) -c consumer.c

clean:
	rm main.o
	rm producer.o
	rm consumer.o
	rm $(TARGET)