#include "LinkedList.h"
#include <stdlib.h>
struct Node {
    Element data;
    Node next;
};


struct LinkedList {
    Node head_node;
    int size;
    CopyFunction copy_func;
    FreeFunction free_func;
    PrintFunction print_func;
    EqualFunction fullComparisonFunc;
    EqualFunction partialComparisonFunc;
};




/**
 * @brief Creates a new LinkedList.
 * @param cf Function to copy elements.
 * @param free_f Function to free elements.
 * @param pf Function to print elements.
 * @param full_comp_f Function for full comparison of elements.
 * @param partial_comp_func Function for partial comparison of elements.
 * @return LinkedList The created LinkedList, or NULL on failure.
 */
LinkedList createLinkedList(CopyFunction cf,FreeFunction free_f, PrintFunction pf, EqualFunction full_comp_f, EqualFunction partial_comp_func) {
    if (cf == NULL || free_f == NULL || pf == NULL ) {
        return NULL;
    }



    // Ensure at least one comparison function is provided
    if (full_comp_f == NULL && partial_comp_func == NULL) {
        return NULL;
    }


    LinkedList list = (LinkedList) malloc(sizeof(struct LinkedList));
    if (list == NULL) {
        return NULL;
    }

        list->head_node = NULL;
        list->size = 0;
        list->copy_func = cf;
        list->free_func = free_f;
        list->print_func = pf;
        list->fullComparisonFunc = full_comp_f;
        list->partialComparisonFunc = partial_comp_func;
        return list;
    }


/**
 * @brief Destroys a LinkedList and frees its memory.
 * @param list The LinkedList to destroy.
 * @return status indicating success or failure.
 */

status DestroyLinkedList(LinkedList list) {
    if (list == NULL) {
        return failure;
    }

    Node current = list->head_node;
    while (current != NULL) {
        Node next = current->next;
        if (list->free_func != NULL) {
            if (list->free_func(current->data) != success) {
                return failure;
            }
        }
        free(current);
        current = next;
    }
    free(list);
    return success;
}


/**
 * @brief Appends a new node with the given element to the LinkedList.
 * @param linked_list The LinkedList to append to.
 * @param elem The element to append.
 * @return status indicating success, failure, or memory problem.
 */
status appendNode(LinkedList linked_list, Element elem) {
    if (linked_list == NULL || elem == NULL) {
        return failure; // Check if the input is valid
    }

    // Create a new node
    Node new_node = (Node)malloc(sizeof(struct Node));
    if (new_node == NULL) {
        return memoryProblem; // Memory allocation failed
    }

    // Initialize the new node
    new_node->data = elem;   // Store the element in the node
    new_node->next = NULL;   // The new node points to NULL (end of the list)

    // If the list is empty
    if (linked_list->head_node == NULL) {
        linked_list->head_node = new_node; // The new node becomes the head of the list
    } else {
        // Traverse to the end of the list
        Node curr = linked_list->head_node;
        while (curr->next != NULL) {
            curr = curr->next; // Move to the next node
        }
        curr->next = new_node; // Add the new node at the end of the list
    }

    linked_list->size++; // Update the size of the list
    return success;
}


/**
 * @brief Deletes a node containing the specified element from the LinkedList.
 * @param linked_list The LinkedList to modify.
 * @param elem The element to delete.
 * @return status indicating success or failure.
 */

status deleteNode(LinkedList linked_list, Element elem) {
    if (linked_list == NULL || elem == NULL) {
        return failure; // Invalid input
    }

    if (linked_list->head_node == NULL) {
        return failure; // List is empty
    }

    Node curr = linked_list->head_node;
    Node prev = NULL;

    // Traverse the list to find the node with the given element
    while (curr != NULL) {
        if (linked_list->partialComparisonFunc(curr->data, elem)) {
            // Node found
            if (prev == NULL) {
                // The node to delete is the head node
                linked_list->head_node = curr->next; // Update the head to the next node
            } else {
                // The node to delete is in the middle or end
                prev->next = curr->next;
            }

            // Free the node's data
            if (linked_list->free_func(curr->data) == failure) {
                return failure; // Failed to free the node's data
            }



            free(curr); // Free the node itself
            linked_list->size--; // Update the list size
            return success;
        }

        prev = curr; // Move to the next node
        curr = curr->next;
    }

    // Element not found in the list
    return failure;
}




/**
 * @brief Displays all elements in the LinkedList.
 * @param linked_list The LinkedList to display.
 * @return status indicating success or failure.
 */
status DisplayList(LinkedList linked_list) {
    if (linked_list == NULL || linked_list->print_func == NULL) {
        return failure;
    }
    Node curr = linked_list->head_node;
    while (curr) {
        if (linked_list->print_func(curr->data) == failure) {
            return failure;
        }
        curr = curr->next;
    }
    return success;
}


/**
 * @brief Retrieves the data at a specified index in the LinkedList.
 * @param linked_list The LinkedList to retrieve from.
 * @param index The index of the element (1-based).
 * @return The data at the specified index, or NULL if invalid.
 */
Element getDataByIndex(LinkedList linked_list, int index) {
    if (linked_list == NULL || index < 1 || index > linked_list->size) {
        return NULL;
    }
    Node curr = linked_list->head_node;
    int curr_idx = 1;
    while (curr && curr_idx < index) {
        curr = curr->next;
        curr_idx++;
    }
    // curr is not null
    if (curr) {
        return linked_list->copy_func(curr->data);
    }else {
        return NULL;
    }
}

/**
 * @brief Gets the length of the LinkedList.
 * @param linked_list The LinkedList to query.
 * @return The number of elements in the LinkedList, or -1 if invalid.
 */

int getLengthList (LinkedList linked_list) {
    if(linked_list == NULL) {
        return -1;
    }
    return linked_list->size;
}


/**
 * @brief Searches for an element in the LinkedList by partial comparison.
 * @param linked_list The LinkedList to search.
 * @param part_of_elem The element to compare against.
 * @return The first matching element, or NULL if not found.
 */
Element searchByKeyInList(LinkedList linked_list, Element part_of_elem) {
    if (linked_list == NULL || part_of_elem == NULL || linked_list->partialComparisonFunc == NULL) {
        return NULL;
    }
    Node curr = linked_list->head_node;
    while (curr) {
        if (linked_list->partialComparisonFunc(curr->data,part_of_elem)) {
            return curr->data;
        }
        curr = curr->next;
    }
    return NULL;
}



/**
 * @brief Performs a shallow copy of a LinkedList.
 * @param original_list The LinkedList to copy.
 * @return The original LinkedList (shallow copy).
 */
LinkedList shallowCopyLinkedList(LinkedList original_list) {
    return original_list;
}


