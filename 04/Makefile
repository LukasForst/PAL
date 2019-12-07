VALGRIND := "0"
SHELL := /bin/bash
CFLAGS+= -std=c++11 -O3
TESTS := $(shell ls data | grep .in | grep -Po '\d+' | sed ':a;N;$!ba;s/\n/\" \"/g' ; )
objects := $(patsubst %.c,%.o,$(wildcard *.cpp))
TESTOUTPUT = "tests/"

program: $(objects)
	g++ -o program $(objects)
#	${CC} ${CFLAGS} -o program $(objects)

run: program
	cat data/pub${N}.in | ./program && cat data/pub${N}.out

exec: program
	./program

zip:
	@if [ -f "`which zip`" ]; then \
	 zip -R hw.zip *.cpp *.h; \
	fi;

test: FORCE
	@if ! [ -d "$TESTOUTPUT" ] ; then\
 mkdir -p ${TESTOUTPUT} ; \
 fi;
	@$(foreach var,$(TESTS), ./program < data/pub${var}.in 1> ${TESTOUTPUT}TEST_${var} 2> /dev/null; if [ "`hexdump ${TESTOUTPUT}TEST_${var}`" = "`hexdump data/pub${var}.out`" ] ; then \
printf "TEST ${var} OUT [\033[0;32m OK \033[0m] "; \
else \
printf "\n ----- EXPECTED RESULT ----- \n\n" >> ${TESTOUTPUT}TEST_${var}; \
cat data/pub${var}.out >> ${TESTOUTPUT}TEST_${var}; \
printf "\n--- HEX ---\n" >> ${TESTOUTPUT}TEST_${var}; \
 ./program <data/pub${var}.in 2>/dev/null| hexdump -C >> ${TESTOUTPUT}TEST_${var} ; \
printf "\n--- EXPECTED HEX ---\n" >> ${TESTOUTPUT}TEST_${var}; \
hexdump -C data/pub${var}.out >> ${TESTOUTPUT}TEST_${var}; \
printf "TEST ${var} OUT [\033[0;31mFAIL\033[0m] "; \
fi;\
if  test -f data/pub${var}.err  ; then \
printf  " ------- [ STDERR ] ------\n"  >>${TESTOUTPUT}TEST_${var};\
 if [ "`./program  < data/pub$(var).in 2>&1 >/dev/null | tee -a ${TESTOUTPUT}TEST_${var} | hexdump`" = "`hexdump data/pub${var}.err`" ] ; then \
printf " ERR [ \033[0;32mOK \033[0m]";    \
else  \
printf "\n ----- EXPECTED RESULT ----- \n\n" >> ${TESTOUTPUT}TEST_${var}; \
cat data/pub${var}.err >> ${TESTOUTPUT}TEST_${var}; \
printf "\n--- HEX ---\n" >> ${TESTOUTPUT}TEST_${var}; \
./program < data/pub${var}.in 2>&1 | hexdump -C >> ${TESTOUTPUT}TEST_${var} ; \
printf "\n--- EXPECTED HEX ---\n" >> ${TESTOUTPUT}TEST_${var}; \
hexdump -C data/pub${var}.err >> ${TESTOUTPUT}TEST_${var};\
printf " ERR [\033[0;31mFAIL\033[0m]"; \
 fi; \
 fi; \
 if [ "${VALGRIND}" = "1" ]; then\
 valgrind --leak-check=full --error-exitcode=125 ./program < data/pub${var}.in >/dev/null 2>&1; \
RETURN=$$?;\
printf " VALGRIND";  \
if [ $$RETURN = 125 ] ; then\
 printf  " [\033[0;31mFAIL\033[0m]"; \
 else \
 printf  " [ \033[0;32mOK \033[0m]";  \
 fi;\
 fi; \
./program < data/pub${var}.in >/dev/null 2>&1;\
 RETURN=$$?;\
printf " PROGRAM RETURNED $$RETURN";  \
if  test -f data/pub${var}.code  ; then \
 if [ RETURN = "`cat data/pub${var}.code`" ] ; then\
 printf  " [ \033[0;32mOK \033[0m]";  \
 else \
 printf  " [\033[0;31mFAIL\033[0m]"; \
 fi;\
fi; \
printf "\n" ;)


clean:
	rm -f *.o
	rm -f program
	rm -f hw.zip
	rm -Rf ${TESTOUTPUT}

all: clean program test zip

help:
	@echo "Pouziti: make prikaz"
	@echo "prikaz:  help    - this help text"
	@echo "         test    - run tests"
	@echo "         clean   - clean binary"
	@echo "         program - build"
	@echo "         zip     - archive all .cpp and .h files"
	@echo "         all     - clean_boolean_vector - program - test - zip"

FORCE: