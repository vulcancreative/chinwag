C=gcc

CFLAGS=-Wall -O3
DEBUG=-g -DCW_DEBUG
DEEPD=-DCW_DEEP_DEBUG

A=ar
AFLAGS=rvs

D=utilities/compile_dict

X=xxd
XFLAGS=-i

DEPTS=rep.o args.o tokenize.o generator.o dict.o chinwag.o utility.o \
config.o seuss.o latin.o
LIBDP=args.o tokenize.o generator.o dict.o chinwag.o utility.o config.o \
seuss.o latin.o
DICTS=include/dicts/latin.h include/dicts/seuss.h

LOREM_LIB=build/lib/libchinwag.a

all: build/ lib $(DICTS) $(DEPTS)
	$(C) $(CFLAGS) rep.o $(LOREM_LIB) -o build/chinwag
	@-rm -rf *.o *.h

chinwag: build/ lib $(DICTS) $(DEPTS)
	$(C) $(CFLAGS) rep.o $(LOREM_LIB) -o build/chinwag
	@-rm -rf *.o *.h
	@-rm -rf build/lib

lib: build/ build/lib/ $(DICTS) $(LIBDP)
	$(A) $(AFLAGS) $(LOREM_LIB) $(LIBDP)
	@-rm -rf *.o *.h

test: build/ lib $(DICTS) $(DEPTS)
	@-$(C) $(CFLAGS) tests/chinwag_test.c $(LOREM_LIB) -o tests/chinwag_test
	@-tests/chinwag_test
	@-rm -rf *.o *.h

install: build/chinwag
	@-chmod +x build/chinwag
	@-cp build/chinwag /usr/local/bin/

debug: build/ $(DICTS) $(DEPTS)
	@-echo "making special *.o builds for debugging..."
	@-echo "incorporating debug operatives..."
	$(C) -c $(CFLAGS) $(DEBUG) src/rep.c
	$(C) -c $(CFLAGS) $(DEBUG) src/dict.c
	$(C) -c $(CFLAGS) $(DEBUG) src/args.c
	$(C) -c $(CFLAGS) $(DEBUG) src/seuss.c
	$(C) -c $(CFLAGS) $(DEBUG) src/latin.c
	$(C) -c $(CFLAGS) $(DEBUG) src/config.c
	$(C) -c $(CFLAGS) $(DEBUG) src/tokenize.c
	$(C) -c $(CFLAGS) $(DEBUG) src/generator.c
	$(C) -c $(CFLAGS) $(DEBUG) src/chinwag.c
	$(C) $(CFLAGS) $(DEBUG) $(DEPTS) -o build/chinwag
	rm -rf *.o *.h

dicts:
	@-test -d include/dicts || mkdir include/dicts
	@-$(D) dict/seuss
	@-$(D) dict/latin
	@-mv *.c src/
	@-mv *.h include/dicts/

build/:
	@-mkdir build

build/lib/: build/
	@-mkdir build/lib

src/seuss.c: dict/seuss
	@-make dicts

src/latin.c: dict/latin
	@-make dicts

rep.o: src/rep.c
	@-$(C) -c $(CFLAGS) src/rep.c

args.o: include/chinwag/args.h src/args.c
	@-$(C) -c $(CFLAGS) src/args.c

dict.o: include/chinwag/dict.h src/dict.c
	@-$(C) -c $(CFLAGS) src/dict.c

seuss.o: include/dicts/seuss.h src/seuss.c
	@-$(C) -c $(CFLAGS) src/seuss.c

latin.o: include/dicts/latin.h src/latin.c
	@-$(C) -c $(CFLAGS) src/latin.c

config.o: include/chinwag/config.h src/config.c
	@-$(C) -c $(CFLAGS) src/config.c

utility.o: include/chinwag/utility.h src/utility.c
	@-$(C) -c $(CFLAGS) src/utility.c

tokenize.o: include/chinwag/tokenize.h src/tokenize.c
	@-$(C) -c $(CFLAGS) src/tokenize.c

generator.o: include/chinwag/generator.h src/generator.c
	@-$(C) -c $(CFLAGS) src/generator.c

chinwag.o: include/chinwag.h src/chinwag.c
	@-$(C) -c $(CFLAGS) src/chinwag.c

include/dicts/seuss.h: dict/seuss
	@-make dicts

include/dicts/latin.h: dict/latin
	@-make dicts

clean:
	if [ -a "include/dicts/seuss.h" ]; then rm include/dicts/seuss.h; fi
	if [ -a "include/dicts/latin.h" ]; then rm include/dicts/latin.h; fi
	if [ -a "tests/chinwag_test" ]; then rm tests/chinwag_test; fi
	if [ -a "src/seuss.c" ]; then rm src/seuss.c; fi
	if [ -a "src/latin.c" ]; then rm src/latin.c; fi
	rm -rf build *.o *.h
