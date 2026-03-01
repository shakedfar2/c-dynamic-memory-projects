#ifndef DEFS_H_
#define DEFS_H_
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



/**
 * @brief Boolean type definition for true/false values.
 */
typedef enum e_bool {
    false, /**< Represents a false value (0). */
    true   /**< Represents a true value (1). */
} bool;

/**
 * @brief Status type definition for function return values.
 */
typedef enum e_status {
    success,       /**< Indicates successful operation. */
    failure,       /**< Indicates operation failure. */
    memoryProblem, /**< Indicates memory allocation issues. */
    displayMenu    /**< Indicates a request to display the menu again. */
} status;

/**
 * @brief Generic element type for data abstraction.
 */
typedef void * Element;

/**
 * @brief Function pointer type for copying an element.
 * @param Element The element to copy.
 * @return A new copy of the element, or NULL on failure.
 */
typedef Element(*CopyFunction) (Element);

/**
 * @brief Function pointer type for freeing an element.
 * @param Element The element to free.
 * @return status indicating success or failure.
 */
typedef status(*FreeFunction) (Element);

/**
 * @brief Function pointer type for printing an element.
 * @param Element The element to print.
 * @return status indicating success or failure.
 */
typedef status(*PrintFunction) (Element);


/**
 * @brief Function pointer type for transforming an element into a number.
 * @param Element The element to transform.
 * @return An integer representation of the element.
 */
typedef int(*TransformIntoNumberFunction) (Element);



/**
 * @brief Function pointer type for comparing two elements.
 * @param Element The first element to compare.
 * @param Element The second element to compare.
 * @return true if the elements are equal, false otherwise.
 */
typedef bool(*EqualFunction) (Element, Element);





#endif /* DEFS_H_ */
