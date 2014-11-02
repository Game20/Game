EXE = client
SRC = client.c
FUNCS = client_window.c client_system.c
OBJS = client.o client_window.o client_system.o
LSDL = -lSDL
GFX = -lSDL_gfx
TTF = -lSDL_ttf
IMAGE = -lSDL_image
LIB = -L/usr/lib
LFLAG = -lm

.c.o:
	$(CC) -c $<

$(EXE): $(OBJS)
	$(CC) -o $(EXE) $(OBJS) $(LSDL) $(GFX) $(TTF) $(IMAGE) $(LIB) $(LFLAG)