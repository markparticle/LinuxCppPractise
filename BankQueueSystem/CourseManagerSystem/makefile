CC = g++
CFLAGS = -std=c++11
TARGET = cmsys
OBJS = main.o CmdManager.o Course.o CourseManager.o

all: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)   
$(OBJS): %.o : %.cpp
	$(CC) -c $(CFLAGS) $< -o $@
	
.PHONY: clean
clean:
	rm -rf $(OBJS) $(TARGET)