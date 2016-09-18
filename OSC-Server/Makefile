SRCDIR = ./src
OBJDIR = ./tmp
BINDIR = ./bin
INCDIR = ./inc

CC= g++
EXEC := $(BINDIR)/render
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)
INCL := -I/usr/local/include -I$(INCDIR) -I$./wosclib-1.0

LIBS+= -L/opt/vc/lib/ -lm -ljpeg -lpng -lz -ltiff -lGLESv2 -lEGL -lopenmaxil -lbcm_host -lvcos -lvchiq_arm -lpthread -lrt -lyaml-cpp -lwosclib
INCL+= -I/opt/vc/include/ -I/opt/vc/include/interface/vcos/pthreads -I/opt/vc/include/interface/vmcs_host/linux -I./ -I../libs/ilclient -I../libs/vgfont

$(EXEC) : $(OBJS)
	mkdir -p $(dir $@)
	$(CC) -o $(EXEC) $(OBJS) $(LIBS)

$(OBJS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp
	mkdir -p $(dir $@)
	$(CC) $(INCL) -DRPI -c $< -o $@
