TARGET = cube
OBJS = cube.o factory.o data.o controllers.o logo.o common/callbacks.o common/vram.o
PSPLIBSDIR = $(PSPSDK)/..

INCDIR =
CFLAGS = -G0 -Wall
CXXFLAGS = $(CFLAGS) -fno-exceptions -fno-rtti
ASFLAGS = $(CFLAGS)

LIBDIR =
LDFLAGS =
LIBS= -lpspgum -lpspgu -lm -lpspaudio -lpspmp3

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = War

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak

logo.o: logo.raw
	bin2o -i logo.raw logo.o logo
