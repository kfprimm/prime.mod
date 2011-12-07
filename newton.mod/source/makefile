

ALL_SRC_FILES = $(DG_SRCS) $(DG_PHYSICS_SRCS) $(DG_NEWTON_SRCS)
DG_OBJ_FILES = $(ALL_SRC_FILES:.cpp=.o)

PLATFORM = linux32
COMPILER = gcc

# Linux options	 gcc 4.2.2
#CPU_FLAGS = -fPIC  -O0 -g 
#CPU_FLAGS = -O2 -fsingle-precision-constant
CPU_FLAGS = -O2 -g -fpic -msse -msse2 -ffloat-store -ffast-math -freciprocal-math -funsafe-math-optimizations -fsingle-precision-constant 
	
#FLAGS = -c -Wall -Wno-strict-aliasing -D_LINUX_VER -ffloat-store -ffast-math -fsingle-precision-constant $(CPU_FLAGS) -I$(DG_INCLUDED_PATH) -I$(DG_INCLUDED_PHYSICS_PATH)
FLAGS = -c -Wall -Wno-strict-aliasing -D_LINUX_VER  $(CPU_FLAGS) -I$(DG_INCLUDED_PATH) -I$(DG_INCLUDED_PHYSICS_PATH)


.SUFFIXES : .o .cpp
.cpp.o :
	$(COMPILER) $(FLAGS) -o $@ $<	

# main target
engine : libNewton.a


# clean all objects target
clean :
	rm $(DG_OBJ_FILES)
	touch $(ALL_SRC_FILES)
#	rm -*.a

# libraries targets
libNewton.a : $(DG_OBJ_FILES)
	ar r $@ $?
#	strip -g -S -d -v libNewton.a -olibNewton.a
	cp libNewton.a ../../../packages/$(PLATFORM)/libNewton.a
	cp ../../source/newton/Newton.h ../../../packages/$(PLATFORM)/Newton.h
	gcc -shared -Wl,-soname,libNewton.so $? -o libNewton.so 
	cp libNewton.so ../../../packages/$(PLATFORM)/libNewton.so
#	sudo cp libNewton.so /usr/lib

 

	
