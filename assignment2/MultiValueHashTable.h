#ifndef MULTIVALUEHASHTABLE_H
#define MULTIVALUEHASHTABLE_H

#include "Defs.h"

typedef struct MultiValueHashTable* MultiValueHashTable;


MultiValueHashTable createMultiValueHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey,
                                              CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue,EqualFunction equalval,EqualFunction eqaulKey, TransformIntoNumberFunction transformIntoNumber,
                                              int hashNumber);

// Destroy MultiValueHashTable
status destroyMultiValueHashTable(MultiValueHashTable mvHashTable);

// Add a value to a key
status addToMultiValueHashTable(MultiValueHashTable mvHashTable, Element key, Element value);

// Get values associated with a key
Element lookupInMultiValueHashTable(MultiValueHashTable mvHashTable, Element key);

// Remove a value from a key
status removeFromMultiValueHashTable(MultiValueHashTable mvHashTable, Element key, Element value);

// Display all key-value lists
status displayMultiValueHashElementsByKey(MultiValueHashTable mvHashTable, Element key);

#endif // MULTIVALUEHASHTABLE_H
