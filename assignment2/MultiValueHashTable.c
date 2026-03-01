#include "HashTable.h"
#include "MultiValueHashTable.h"

#include "KeyValuePair.h"
#include "LinkedList.h"


struct MultiValueHashTable {
    CopyFunction copy_key;
    CopyFunction copy_val;
    FreeFunction free_key;
    FreeFunction free_val;
    PrintFunction print_key;
    PrintFunction print_val;
    EqualFunction equal_val;
    TransformIntoNumberFunction transform_into_number_func;
    int hashNumber;
    EqualFunction equalKey;
    hashTable innerHashTable;
};


/**
 * @brief Frees a linked list element.
 * @param list_element The linked list element to free.
 * @return `success` if successful, otherwise `failure`.
 */

status freeLinkedListWrapper(Element list_element) {
    if (list_element == NULL) {
        return success;
    }

    LinkedList list = (LinkedList)list_element;


    if (DestroyLinkedList(list) == success) {
        return success;
    }

    return failure;
}


/**
 * @brief Performs a shallow copy of a linked list element.
 * @param original_list The original linked list element to copy.
 * @return A shallow copy of the list, or NULL on failure.
 */
Element shallowCopyLinkedListWrapper(Element original_list) {
    if (original_list == NULL) {
        return NULL;
    }
    return original_list;
}


/**
 * @brief Prints the contents of a linked list.
 * @param list_element The linked list element to print.
 * @return `success` if successful, otherwise `failure`.
 */
status printLinkedListWrapper(Element list_element) {
    if (list_element == NULL) {
        return failure; // Invalid input, nothing to print
    }

    // Cast the generic Element to LinkedList
    LinkedList list = (LinkedList)list_element;

    // Display the linked list using its display function
    if (DisplayList(list) == success) {
        return success; // Successfully printed the linked list
    } else {
        return failure; // Failed to print the linked list
    }
}

/**
 * @brief Creates a MultiValueHashTable.
 * @param copyKey Function to copy keys.
 * @param freeKey Function to free keys.
 * @param printKey Function to print keys.
 * @param copyValue Function to copy values.
 * @param freeValue Function to free values.
 * @param printValue Function to print values.
 * @param equalval Function to compare values.
 * @param equalKey Function to compare keys.
 * @param transformIntoNumber Function to transform keys into numbers for hashing.
 * @param hashNumber Number of hash buckets.
 * @return Pointer to the created MultiValueHashTable, or NULL on failure.
 */
MultiValueHashTable createMultiValueHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey,
CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue,EqualFunction equalval,EqualFunction eqaulKey
, TransformIntoNumberFunction transformIntoNumber,
int hashNumber) {

    if (copyKey == NULL || freeKey == NULL || printKey == NULL || copyValue == NULL || freeValue == NULL ||
        printValue == NULL || transformIntoNumber == NULL || equalval == NULL || hashNumber <= 0) {
        return NULL; // Invalid inputs
        }


    MultiValueHashTable mvHashTable = (MultiValueHashTable)malloc(sizeof(struct MultiValueHashTable));
    if (mvHashTable == NULL) {
        return NULL; // Memory allocation failed
    }
        mvHashTable->copy_key = copyKey;
        mvHashTable->free_key = freeKey;
        mvHashTable->print_key = printKey;
        mvHashTable->copy_val = copyValue;
        mvHashTable->free_val = freeValue;
        mvHashTable->print_val = printValue;
        mvHashTable->transform_into_number_func = transformIntoNumber;
        mvHashTable->hashNumber = hashNumber;
        mvHashTable->equal_val = equalval;
        mvHashTable->equalKey = eqaulKey;


        mvHashTable->innerHashTable = createHashTable(copyKey, freeKey, printKey, shallowCopyLinkedListWrapper
            ,freeLinkedListWrapper,printLinkedListWrapper,
            mvHashTable->equalKey, transformIntoNumber, hashNumber);

        if (mvHashTable->innerHashTable == NULL) {
            free(mvHashTable);
            return NULL; // Failed to create the inner HashTable
        }

        return mvHashTable;
    }



/**
 * @brief Destroys a MultiValueHashTable and frees its memory.
 * @param mvHashTable The MultiValueHashTable to destroy.
 * @return `success` if successful, otherwise `failure`.
 */

    status destroyMultiValueHashTable(MultiValueHashTable mvHashTable) {
        if (mvHashTable == NULL) {
            return success;
        }

        // Destroy the inner HashTable
        if (mvHashTable->innerHashTable != NULL) {
            if (destroyHashTable(mvHashTable->innerHashTable) == failure) {
                return failure; // Failed to destroy the inner HashTable
            }
        }
        // Free the MultiValueHashTable structure itself
        free(mvHashTable);
        return success; // Successfully destroyed the MultiValueHashTable
    }



/**
 * @brief Adds a value to a MultiValueHashTable under a specified key.
 * @param mvHashTable The MultiValueHashTable to add to.
 * @param key The key under which to add the value.
 * @param value The value to add.
 * @return `success` if successful, otherwise `failure`.
 */
status addToMultiValueHashTable(MultiValueHashTable mvHashTable, Element key, Element value) {
    if (mvHashTable == NULL || key == NULL || value == NULL) {
        return failure;
    }

    //check if the list is already exist
    LinkedList valuesList = lookupInHashTable(mvHashTable->innerHashTable, key);

    if (valuesList != NULL) {
        //if the list exist just adding the value to it
        if (appendNode(valuesList, value) == failure) {
            return failure;
        }
        return success;
    }

    // the list does not exist create new one
    LinkedList newList = createLinkedList(
        mvHashTable->copy_val,
        mvHashTable->free_val,
        mvHashTable->print_val,
        NULL,
        mvHashTable->equal_val);

    if (newList == NULL) {
        return memoryProblem;
    }

    //adding the value to the new list
    if (appendNode(newList, value) == failure) {
        DestroyLinkedList(newList);
        return failure;
    }

    //adding the list to the hash table
    if (addToHashTable(mvHashTable->innerHashTable, key, newList) == failure) {
        DestroyLinkedList(newList);
        return failure;
    }

    return success;
}


/**
 * @brief Looks up a key in the MultiValueHashTable and retrieves the associated values.
 * @param mvHashTable The MultiValueHashTable to search.
 * @param key The key to look up.
 * @return The linked list of values associated with the key, or NULL if not found.
 */

Element lookupInMultiValueHashTable(MultiValueHashTable mvHashTable, Element key) {
        if (mvHashTable == NULL || key == NULL) {
            return NULL;
        }

        LinkedList value_list = lookupInHashTable(mvHashTable->innerHashTable,key);
        if (value_list == NULL) {
            return NULL;
        }
        return value_list;
}




/**
 * @brief Removes a specific value from a MultiValueHashTable under a given key.
 * @param mvHashTable The MultiValueHashTable to modify.
 * @param key The key to search for.
 * @param value The value to remove.
 * @return `success` if successful, otherwise `failure`.
 */
status removeFromMultiValueHashTable(MultiValueHashTable mvHashTable, Element key, Element value) {
    if (mvHashTable == NULL || key == NULL || value == NULL) {
        return failure; // Invalid input
    }

    // Check if the key exists in the inner HashTable
    LinkedList value_list = (LinkedList)lookupInHashTable(mvHashTable->innerHashTable, key);
    if (value_list == NULL) {
        return failure; // Key does not exist
    }

    //Delete the value from the LinkedList
    if (deleteNode(value_list, value) == failure) {
        return failure; // Failed to delete the value
    }

    //Check if the LinkedList is now empty
    if (getLengthList(value_list) == 0) {
        // Remove the key from the HashTable
        if (removeFromHashTable(mvHashTable->innerHashTable, key) == failure) {
            return failure; // Failed to remove the key
        }
    }
    return success; // Successfully removed the value (and possibly the key)
}


/**
 * @brief Prints a key and its associated values from the MultiValueHashTable.
 * @param key The key to print.
 * @param mvht The MultiValueHashTable containing the key-value pairs.
 * @return `success` if successful, otherwise `failure`.
 */
//helper function
status printKeyValues(Element key, MultiValueHashTable mvht) {
    if (key == NULL || mvht == NULL) {
        return failure;
    }

    LinkedList value_list = lookupInHashTable(mvht->innerHashTable,key);
    if (value_list == NULL) {
        return failure;
    }

    // Print the key followed by the value
    if (displayKey(key) == failure || DisplayList(value_list) == failure) {
        return failure; // Failed to print key or value
    }

    return success;
}



/**
 * @brief Displays the key and associated values from the MultiValueHashTable for a given key.
 * @param mvHashTable The MultiValueHashTable to search.
 * @param key The key to display.
 * @return `success` if successful, otherwise `failure`.
 */
status displayMultiValueHashElementsByKey(MultiValueHashTable mvHashTable, Element key) {
    if (mvHashTable == NULL || key == NULL) {
        return failure; // Invalid input
    }
    if (printKeyValues(key,mvHashTable) == failure) {
        return failure;
    }

    return success; // Successfully printed the key and values
}
