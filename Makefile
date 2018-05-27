
.PHONE: all testdir

all: testdir
testdir:
	$(MAKE) -C test
