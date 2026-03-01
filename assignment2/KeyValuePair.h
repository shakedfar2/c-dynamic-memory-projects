#ifndef KEYVALUEPAIR_H
#define KEYVALUEPAIR_H
#include "Defs.h"


typedef struct KeyValuePair* KeyValuePair;
KeyValuePair createKeyValuePair(Element key, Element value, CopyFunction copy_key, CopyFunction copy_value,
                                 FreeFunction free_key, FreeFunction free_value, PrintFunction print_key, PrintFunction print_value, EqualFunction equal_func);

status destroyKeyValuePair(KeyValuePair key_value_pair);
status displayValue(KeyValuePair key_value_pair);
status displayKey(KeyValuePair key_value_pair);
Element getValue(KeyValuePair key_value_pair);
Element getKey(KeyValuePair key_value_pair);
bool isEqualKey(KeyValuePair key_value_pair, Element key);
// helper function to print key value pair
status printKeyValuePair(KeyValuePair key_value_pair);
//copy function for the object key value pair
Element shallowCopyKeyValuePair(Element element);
#endif //KEYVALUEPAIR_H
