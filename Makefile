TARGET1	= server
TARGET2 = client 
TARGET3 = core
OBJS1 = server_main.o server_net.o server_command.o server_system.o
OBJS2 = client.o client_command.o client_net.o client_window.o client_system.o client_se.o
LSDL = -lSDL
GFX = -lSDL_gfx
TTF = -lSDL_ttf
IMAGE = -lSDL_image -lcwiimote
MIX = -lSDL_mixer
LIB = -L/usr/lib
LFLAG = -lm

.c.o:
	$(CC) -c -DNDEBUG $<

all: $(TARGET1) $(TARGET2)

$(TARGET1):	$(OBJS1)
	gcc -o $(TARGET1) $(OBJS1) $(LFLAG) $(LSDL) $(MIX)

$(TARGET2): $(OBJS2)
	$(CC) -o $(TARGET2) $(OBJS2) $(LSDL) $(GFX) $(TTF) $(IMAGE) $(MIX) $(LIB) $(LFLAG)  $(LDFLAGS) 

clean:
	rm *.o $(TARGET1) $(TARGET2) $(TARAGET3)
