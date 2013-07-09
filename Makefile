CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =	Messages.o LogManager.o InterfaceConnection.o Gems.o

LIBS = -ldl -lrt

TARGET =	Gems

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
