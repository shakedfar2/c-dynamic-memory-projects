#include "KeyValuePair.h"

#include <stdio.h>
#include <stdlib.h>

#include "LinkedList.h"

struct KeyValuePair {
    Element key;
    Element value;
    CopyFunction copy_key_func;  // Function to copy the key
    CopyFunction copy_value_func;  // Function to copy the value
    FreeFunction free_key_func;  // Function to free the key
    FreeFunction free_value_func;  // Function to free the value
    PrintFunction print_key;
    PrintFunction print_value;
    EqualFunction equal_function;
};


/**
 * @brief Prints a KeyValuePair's key and value.
 *
 * @param key_value_pair The KeyValuePair to print.
 * @return success if both the key and value were printed successfully, otherwise failure.
 */

// Helper function to print a KeyValuePair
status printKeyValuePair(KeyValuePair key_value_pair) {
    if (key_value_pair == NULL) {
        return failure; // Invalid input
    }

    // Print the key followed by the value
    if (displayKey(key_value_pair) == failure || displayValue(key_value_pair) == failure) {
        return failure; // Failed to print key or value
    }

    return success;
}


/**
 * @brief Performs a shallow copy of the given element (returns the same element).
 *
 * @param element The element to copy.
 * @return The same element passed as input, or NULL if the input is NULL.
 */

Element shallowCopyKeyValuePair(Element element) {
    if (element == NULL) {
        return NULL;
    }
    return element;
}



/**
 * @brief Creates a new KeyValuePair with the given key, value, and associated functions.
 *
 * @param key The key element for the KeyValuePair.
 * @param value The value element for the KeyValuePair.
 * @param copy_key Function to copy the key.
 * @param copy_value Function to copy the value.
 * @param free_key Function to free the key.
 * @param free_value Function to free the value.
 * @param print_key Function to print the key.
 * @param print_value Function to print the value.
 * @param equal_func Function to compare keys for equality.
 * @return A pointer to the created KeyValuePair, or NULL if creation failed.
 *
 * @note The key and value are copied using the provided copy functions.
 */

KeyValuePair createKeyValuePair(Element key, Element value,
                                CopyFunction copy_key, CopyFunction copy_value,
                                FreeFunction free_key, FreeFunction free_value,
                                PrintFunction print_key, PrintFunction print_value,
                                EqualFunction equal_func) {
    if (key == NULL || value == NULL || copy_key == NULL || copy_value == NULL ||
        free_key == NULL || free_value == NULL || print_key == NULL || print_value == NULL || equal_func == NULL) {
        return NULL; // Invalid inputs
        }

    KeyValuePair key_value_pair = (KeyValuePair)malloc(sizeof(struct KeyValuePair));
    if (key_value_pair == NULL) {
        return NULL; // Memory allocation failed
    }

    key_value_pair->key = copy_key(key);  // Copy the key
    key_value_pair->value = copy_value(value);  // Copy the value


    if (key_value_pair->key == NULL || key_value_pair->value == NULL) {
        if (key_value_pair->key != NULL) free_key(key_value_pair->key);
        if (key_value_pair->value != NULL) free_value(key_value_pair->value);
        free(key_value_pair);
        return NULL;
    }

    key_value_pair->copy_key_func = copy_key;
    key_value_pair->copy_value_func = copy_value;
    key_value_pair->free_key_func = free_key;
    key_value_pair->free_value_func = free_value;
    key_value_pair->print_key = print_key;
    key_value_pair->print_value = print_value;
    key_value_pair->equal_function = equal_func;

    return key_value_pair;
}


/**
 * @brief Frees all memory associated with a KeyValuePair, including its key and value.
 *
 * @param key_value_pair The KeyValuePair to destroy.
 * @return success if the KeyValuePair was successfully destroyed, otherwise failure.
 *
 * @note If the key or value fails to free, the function returns failure.
 */

status destroyKeyValuePair(KeyValuePair key_value_pair) {
    if (key_value_pair == NULL) {
        return failure;  // Invalid input
    }



    // Free the key and value
    if (key_value_pair->free_key_func(key_value_pair->key) == failure) {
        return failure;
    }

    if (key_value_pair->value == NULL) {
        return success;
    }
    if (key_value_pair->free_value_func(key_value_pair->value) == failure) {
        return failure;
    }

    // Free the KeyValuePair structure
    free(key_value_pair);
    return success;
}



/**
 * @brief Prints the value of a KeyValuePair using the associated print function.
 *
 * @param key_value_pair The KeyValuePair whose value is to be printed.
 * @return success if the value was printed successfully, otherwise failure.
 */

status displayValue(KeyValuePair key_value_pair) {
    if (key_value_pair == NULL || key_value_pair->print_value == NULL) {
        return failure;  // Invalid input
    }

    // Use the print_value function to print the value
    return key_value_pair->print_value(key_value_pair->value);
}



/**
 * @brief Prints the key of a KeyValuePair using the associated print function.
 *
 * @param key_value_pair The KeyValuePair whose key is to be printed.
 * @return success if the key was printed successfully, otherwise failure.
 */

status displayKey(KeyValuePair key_value_pair) {
    if (key_value_pair == NULL || key_value_pair->print_key == NULL) {
        return failure;  // Invalid input
    }

    // Use the print_key function to print the key
    return key_value_pair->print_key(key_value_pair->key);
}


/**
 * @brief Retrieves a copy of the value stored in the KeyValuePair.
 *
 * @param key_value_pair The KeyValuePair to retrieve the value from.
 * @return A copy of the value, or NULL if the input KeyValuePair is invalid.
 *
 * @note The value is copied using the `copy_value_func` provided during creation.
 */


Element getValue(KeyValuePair key_value_pair) {
    if (key_value_pair == NULL) {
        return NULL;  // Invalid input
    }

    return key_value_pair->copy_value_func(key_value_pair->value);
}



/**
 * @brief Retrieves the key stored in the KeyValuePair.
 *
 * @param key_value_pair The KeyValuePair to retrieve the key from.
 * @return The key element, or NULL if the input KeyValuePair is invalid.
 *
 * @note The key is not copied; the original key is returned.
 */

Element getKey(KeyValuePair key_value_pair) {
    if (key_value_pair == NULL) {
        return NULL;  // Invalid input
    }

    return key_value_pair->key;
}



/**
 * @brief Checks if the given key is equal to the key stored in the KeyValuePair.
 *
 * @param key_value_pair The KeyValuePair to compare the key with.
 * @param key The key to compare.
 * @return true if the keys are equal, otherwise false.
 *
 * @note The comparison is performed using the `equal_function` provided during creation.
 */

bool isEqualKey(KeyValuePair key_value_pair, Element key) {
    if (key_value_pair == NULL || key == NULL || key_value_pair->equal_function == NULL) {
        return false;  // Invalid input
    }

    // Use the equal_function to compare keys
    return key_value_pair->equal_function(key_value_pair->key, key);
}



