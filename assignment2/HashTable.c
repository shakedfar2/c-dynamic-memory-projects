#include "HashTable.h"

#include <bits/posix2_lim.h>

#include "LinkedList.h"
#include "KeyValuePair.h"


struct hashTable_s {
    LinkedList* buckets; // buckets is a pointer to an array of pointers to LinkedLists
    int hashNumber;                // Hash table size
    CopyFunction copyKey;
    FreeFunction freeKey;
    PrintFunction printKey;
    CopyFunction copyValue;
    FreeFunction freeValue;
    PrintFunction printValue;
    EqualFunction equalKey;
    TransformIntoNumberFunction transformIntoNumber;  // Function to transform any generic key into a number
};

/**
 * @brief Destroys a KeyValuePair element.
 * @param elem The KeyValuePair to destroy.
 * @return status indicating success or failure.
 */
status wrappedDestroyKeyValuePair(Element elem) {
    return destroyKeyValuePair((KeyValuePair)elem);
}


/**
 * @brief Prints a KeyValuePair element.
 * @param elem The KeyValuePair to print.
 * @return status indicating success or failure.
 */
status wrappedPrintKeyValuePair(Element elem) {
    return printKeyValuePair((KeyValuePair)elem);
}


/**
 * @brief Checks equality of a KeyValuePair's key with another key.
 * @param elem1 The KeyValuePair element.
 * @param elem2 The key to compare against.
 * @return true if the keys are equal, false otherwise.
 */
bool wrappedIsEqualKey(Element elem1, Element elem2) {
    return isEqualKey((KeyValuePair)elem1, elem2);
}



/**
 * @brief Creates a new HashTable.
 * @param copyKey Function to copy keys.
 * @param freeKey Function to free keys.
 * @param printKey Function to print keys.
 * @param copyValue Function to copy values.
 * @param freeValue Function to free values.
 * @param printValue Function to print values.
 * @param equalKey Function to compare keys.
 * @param transformIntoNumber Function to transform keys into numbers (hash).
 * @param hashNumber The size of the hash table.
 * @return hashTable The created HashTable, or NULL on failure.
 */

hashTable createHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey,
                          CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue,
                            EqualFunction equalKey, TransformIntoNumberFunction transformIntoNumber,
                          int hashNumber) {
    if (!copyKey || !freeKey || !printKey || !copyValue || !freeValue || !printValue ||
        !equalKey || !transformIntoNumber || hashNumber <= 0) {
        return NULL;  // Invalid inputs
        }

    // allocation of memory to the struct hash table
    hashTable hTable = (hashTable)malloc(sizeof(struct hashTable_s));
    if (hTable == NULL) {
        return NULL;
    }

    // allocation for each cell in hash table , each cell is linked list
    hTable->buckets = (LinkedList*)malloc(hashNumber * sizeof(LinkedList));
    if (hTable->buckets == NULL) {
        free(hTable);
        return NULL;
    }

    //initialize the fields
    hTable->hashNumber = hashNumber;
    hTable->copyKey = copyKey;
    hTable->freeKey = freeKey;
    hTable->printKey = printKey;
    hTable->copyValue = copyValue;
    hTable->freeValue = freeValue;
    hTable->printValue = printValue;
    hTable->equalKey = equalKey;
    hTable->transformIntoNumber = transformIntoNumber;

    // Initialize buckets as empty linked lists
    for (int i = 0; i < hashNumber; i++) {
        hTable->buckets[i] = createLinkedList(shallowCopyKeyValuePair, wrappedDestroyKeyValuePair, wrappedPrintKeyValuePair,NULL,wrappedIsEqualKey);
        // if the creation failed
        if (hTable->buckets[i] == NULL) {
            // free the linkedList
            for (int j = 0; j < i; j++) {
                DestroyLinkedList(hTable->buckets[j]);
            }
            // destroy the buckets itself
            free(hTable->buckets);
            // destroy the hash table
            free(hTable);

            return NULL;
        }
    }
    return hTable;
}



/**
 * @brief Destroys a HashTable and frees its memory.
 * @param hash_table The HashTable to destroy.
 * @return status indicating success or failure.
 */
status destroyHashTable(hashTable hash_table) {
    if (hash_table == NULL) {
        return failure;
    }

    //
    for (int i = 0; i < hash_table->hashNumber; i++) {
        if (DestroyLinkedList(hash_table->buckets[i]) == failure) {
            return failure;
        }
    }
    free(hash_table->buckets);
    free(hash_table);
    return success;
}



/**
 * @brief Adds a key-value pair to the HashTable.
 * @param hash_table The HashTable to add to.
 * @param key The key to add.
 * @param value The value to associate with the key.
 * @return status indicating success, failure, or memory problem.
 */


status addToHashTable(hashTable hash_table, Element key,Element value) {
    if (hash_table == NULL || key == NULL || value == NULL) {
        return failure;
    }


    //calculate the cell index
    int cellIndex = abs( hash_table->transformIntoNumber(key) % hash_table->hashNumber);



    // insert linked list to the array
    LinkedList cell = hash_table->buckets[cellIndex];
    // Check if the key already exists
    if (lookupInHashTable(hash_table,key) != NULL) {
            return failure;
        //key exist
        }
    else{
        // Key does not exist, create a new KeyValuePair
        KeyValuePair new_pair = createKeyValuePair(
            key,
            value,
            hash_table->copyKey,
            hash_table->copyValue,
            hash_table->freeKey,
            hash_table->freeValue,
            hash_table->printKey,
            hash_table->printValue,hash_table->equalKey);

        if (new_pair == NULL) {
            hash_table->freeKey(key); // Free the key if KeyValuePair creation fails
            hash_table->freeValue(value); // Free the value if KeyValuePair creation fails
            return memoryProblem; // Failed to create KeyValuePair
        }

        // Add the new KeyValuePair to the linked list in the bucket
        if (appendNode(cell, new_pair) == failure) {
            destroyKeyValuePair(new_pair); // Clean up if appendNode fails
            return failure;
        }

        return success; // Successfully added the new KeyValuePair

    }
}


/**
 * @brief Looks up a key in the HashTable and retrieves the associated value.
 * @param hash_table The HashTable to search.
 * @param key The key to look up.
 * @return The associated value, or NULL if not found.
 */


Element lookupInHashTable(hashTable hash_table, Element key) {
    if (hash_table == NULL || key == NULL) {
        // printf("Error: NULL table or key in lookup\n");
        return NULL;
    }

    // calculate the index of the cell that the key sit there
    int cell_index = hash_table->transformIntoNumber(key) % hash_table->hashNumber;

    if (cell_index < 0 || cell_index >= hash_table->hashNumber) {
        return NULL;
    }

    LinkedList cell_linked_list = hash_table->buckets[cell_index];
    if (cell_linked_list == NULL) {
        return NULL;
    }


    KeyValuePair key_pair = (KeyValuePair) searchByKeyInList(cell_linked_list,key);
    if (key_pair == NULL) {
         // key not found
         return NULL;
     }
    return getValue(key_pair);
}


/**
 * @brief Removes a key-value pair from the HashTable by key.
 * @param hash_table The HashTable to modify.
 * @param key The key to remove.
 * @return status indicating success or failure.
 */

status removeFromHashTable(hashTable hash_table, Element key) {
    if (hash_table == NULL || key == NULL) {
        return failure; // Invalid input
    }


    // Calculate the index of the cell where the key might be stored
    int cell_index = hash_table->transformIntoNumber(key) % hash_table->hashNumber;

    // Get the linked list (bucket) in the hash table
    LinkedList cell_linked_list = hash_table->buckets[cell_index];
    if (cell_linked_list == NULL) {
        return failure;
    }
    // Use searchByKeyInList to find the KeyValuePair in the bucket

    KeyValuePair key_pair = (KeyValuePair)searchByKeyInList(cell_linked_list, key);
    if (key_pair == NULL) {
        return failure; // Key not found
    }
    Element the_key = (Element)getKey(key_pair);


    // Remove the KeyValuePair from the linked list and clean memory
    if (deleteNode(cell_linked_list, the_key) == failure) {

        return failure; // Failed to remove the node from the list
    }
    return success; // Successfully removed the key-value pair
}



/**
 * @brief Displays all elements in the HashTable.
 * @param hash_table The HashTable to display.
 * @return status indicating success or failure.
 */

status displayHashElements(hashTable hash_table) {
    if (hash_table == NULL) {
        return failure; // Invalid input
    }

    // Iterate over all buckets in the hash table
    for (int i = 0; i < hash_table->hashNumber; i++) {
        LinkedList cell_linked_list = hash_table->buckets[i];
        if (cell_linked_list != NULL) {
            // Use DisplayList to print all KeyValuePairs in the current bucket
            if (DisplayList(cell_linked_list) == failure) {
                return failure; // Failed to display the list
            }
        }
    }

    return success; // Successfully displayed all elements
}


