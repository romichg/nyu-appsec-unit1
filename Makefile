default: prog

get-deps:
	# Assuming Debian or Ubuntu here
	sudo apt-get install -y build-essential check

dictionary.o: dictionary.c
	gcc -Wall -c dictionary.c dictionary.h

spell.o: spell.c
	gcc -Wall -c spell.c

test.o: test_main.c
	gcc -Wall -c test_main.c

main.o: main.c
	gcc -Wall -c main.c

test: dictionary.o spell.o test_main.o
	gcc -Wall -o spell_test test_main.o spell.o dictionary.o -lcheck -lm -lrt -lpthread -lsubunit
	./spell_test

prog: dictionary.o spell.o main.o
	gcc -Wall -o spell_check dictionary.o spell.o main.o

clean:
	rm dictionary.o spell.o main.o test_main.o check_spell.o dictionary.h.gch dictionary.gcno spell.gcno dictionary.gcda spell.gcda test_main.gcno test_main.gcda coverage.info
	
cleanall: clean 
	rm spell_check spell_test

codecov: 
	gcc --coverage -Wall -o spell_test_coverage test_main.c spell.c dictionary.c dictionary.h -lcheck -lm -lrt -lpthread -lsubunit
	./spell_test_coverage
	lcov --capture --directory ./ --output-file coverage.info
	genhtml coverage.info --output-directory docs 
