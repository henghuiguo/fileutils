CC = g++
CFLAGS = -Wall -g -Os -std=c++11 
SRCS = *.cpp
OBJS = $(patsubst %.cpp,%.o,$(wildcard $(SRCS)))
TARGET = test

.PHONY: $(TARGET) clean doc

$(TARGET): $(OBJS)
	$(CC) -o $@ $^
	./test

clean:
	rm -rf $(OBJS) $(TARGET) doc

doc:
	doxygen Doxygen

%.o:%.cpp
	$(CC) $(CFLAGS) -c -o $@ $<
