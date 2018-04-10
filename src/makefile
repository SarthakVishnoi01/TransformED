CC=g++
IDIR = ../include
CFLAGS= -I$(IDIR)

ODIR = obj
LDIR = ../lib

LIBS          = -lGL -lGLU -lglut -lm
GTKFLAGS = `pkg-config gtkmm-3.0 --cflags --libs`

_DEPS = common.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = toplevel.o 3D_view.o 2D_view.o 2D_to_3D.o 3D_to_2D.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(GTKFLAGS) $(LIBS)

program: $(OBJ)
	g++ -o $@ $^ $(CFLAGS) $(GTKFLAGS) $(LIBS)

clean:
	rm -f $(ODIR)/*.o *~ core $(INCDIR)/*~ 