JerryBoree:Jerry.o JerryBoreeMain.o LinkedList.o KeyValuePair.o HashTable.o MultiValueHashTable.o
	gcc Jerry.o JerryBoreeMain.o LinkedList.o KeyValuePair.o HashTable.o MultiValueHashTable.o -o JerryBoree

Jerry.o:Jerry.c Jerry.h Defs.h
	gcc -c Jerry.c

JerryBoreeMain.o:JerryBoreeMain.c Jerry.h HashTable.h LinkedList.h KeyValuePair.h MultiValueHashTable.h Defs.h
	gcc -c JerryBoreeMain.c

LinkedList.o:LinkedList.c LinkedList.h Defs.h
	gcc -c LinkedList.c

KeyValuePair.o:KeyValuePair.c KeyValuePair.h Defs.h
	gcc -c KeyValuePair.c

HashTable.o:HashTable.c HashTable.h LinkedList.h KeyValuePair.h Defs.h
	gcc -c HashTable.c

MultiValueHashTable.o:MultiValueHashTable.c MultiValueHashTable.h HashTable.h LinkedList.h KeyValuePair.h Defs.h
	gcc -c MultiValueHashTable.c

clean:
	rm -f *.o JerryBoree
