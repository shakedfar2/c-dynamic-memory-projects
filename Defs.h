//
// Created by שקד on 12/1/2024.
//
#ifndef DFS_H
#define DFS_H

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

#endif // DFS_H
