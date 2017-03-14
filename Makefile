SOURCES_CPP := 	xmain.cpp \
		audio.cpp \
		basegame.cpp \
		bsp.cpp \
		common.cpp \
		decal.cpp \
		edge.cpp \
		engine.cpp \
		entity.cpp \
		frame.cpp \
		graph.cpp \
		graphics.cpp \
		hashtable.cpp \
		heap.cpp \
		junzip.cpp \
		light.cpp \
		matrix.cpp \
		md5.cpp \
		md5model.cpp \
		model.cpp \
		menu.cpp \
		net.cpp \
		parse.cpp \
		plane.cpp \
		player.cpp \
		quake3.cpp \
		quaternion.cpp \
		rigidbody.cpp \
		shader.cpp \
		sin_table.cpp \
		speaker.cpp \
		trigger.cpp \
		vector.cpp \
		vehicle.cpp \

SOURCES_CC :=	md5sum.c \


OBJS_CPP  := $(SOURCES_CPP:%.cpp=%.o)
OBJDIR_CPP := $(SOURCES_CPP:%.cpp=obj/%.o)
OBJS_C  := $(SOURCES_CC:%.c=%.o)
OBJDIR_C := $(SOURCES_CC:%.c=obj/%.o)

%.o: src/%.cpp
	$(CPP) $(CFLAGS) -c $(INCLUDES) -o obj/$@ $<

%.o: src/%.c
	$(CC) $(CFLAGS) -c $(INCLUDES) -o obj/$@ $<


INCLUDES = -I./include/ -I/usr/local/opt/openal-soft/include -I/usr/X11R6/include -I/opt/X11/include 
#CPP := g++
#CC := gcc
CPP := clang++
CC := clang

#coverity stuff, OSX has g++ point to clang, so must use linux for coverity run
#cov-configure --comptype gcc --compiler [path to compiler]
#cov-build --dir cov-int make

#AddressSanitizer
#-O1 -g -fsanitize=address -fno-omit-frame-pointer

# clang memory sanitizer -- ie the slow one
#-fsanitize=memory -fno-omit-frame-pointer
#-fsanitize-memory-track-origins
#-fsanitize-memory-use-after-dtor
#-fsanitize=safe-stack

altEngine: CFLAGS := -DGL_GLEXT_PROTOTYPES -DOPENGL -Wno-write-strings -Wall -O3 -march=native #-fsanitize=address -fno-omit-frame-pointer
altEngine_dedicated: CFLAGS := -DGL_GLEXT_PROTOTYPES -DDEDICATED -DOPENGL -Wno-write-strings -Wall -O3 -march=native
LFLAGS_OSX := -lX11 -lGL -lc -framework OpenAL
LFLAGS := -lX11 -lGL -lopenal
LIBS := -L/usr/X11R6/lib/ 

all: altEngine

altEngine: $(OBJS_CPP) $(OBJS_C)
	$(CPP) $(CFLAGS) -o altEngine $(OBJDIR_CPP) $(OBJDIR_C) $(LIBS) $(LFLAGS)

altEngine_dedicated: $(OBJS_CPP) $(OBJS_C)
	$(CPP) $(CFLAGS) -o altEngine_dedicated $(OBJDIR_CPP) $(OBJDIR_C) $(LIBS) $(LFLAGS)

clean:
	rm -f ./obj/*.o
	rm -f ./altEngine

