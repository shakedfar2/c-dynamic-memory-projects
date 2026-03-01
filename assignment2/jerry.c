//
// Created by שקד on 12/1/2024.
//
#include "Jerry.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>


/**
 * @brief Creates a new Jerry object, including its Planet and Origin objects.
 *        Allocates memory dynamically for all components and ensures proper error handling.
 *
 * @param id Pointer to a string representing Jerry's unique ID.
 * @param planetName Pointer to a string representing the name of the Planet.
 * @param x X-coordinate of the Planet.
 * @param y Y-coordinate of the Planet.
 * @param z Z-coordinate of the Planet.
 * @param dimensionName Pointer to a string representing the Dimension's name.
 * @param happinessLevel Integer representing Jerry's happiness level.
 * @return Pointer to the newly created Jerry object on success, or NULL on failure.
 */

Jerry* createJerry(char* id, Planet* p_planet, char* dimensionName, int happinessLevel) {
    // Validate input pointers to ensure they are not NULL
    if (id == NULL || p_planet == NULL || dimensionName == NULL) {
        return NULL;
    }

    Jerry* jerry = (Jerry*) malloc(sizeof(Jerry));
    //check if allocation is failed
    if (jerry==NULL) {
        return NULL;
    }
        // allocate dynamic space to the id pointer
        // add +1 for the /0
        jerry->ID = (char *) malloc(strlen(id)+1);
        //check if allocation is failed
        if (jerry->ID==NULL) {
            // free the memory of jerry
            destroyJerry(jerry);
            return NULL; //return null
        }

        strcpy(jerry->ID, id); // Copy the content of id into the allocated memory

        // Create the Origin object
        Origin* origin = createOrigin(p_planet, dimensionName);
        if (origin == NULL) {
            free(jerry->ID);
            free(jerry);

            return NULL; // Failed to create Origin
        }

        // Initialize Jerry's fields
        jerry->origin = origin;
        jerry->happinessLevel = happinessLevel;
        jerry->attributes = NULL; // No attributes initially
        jerry->numOfAttributes = 0;

        return jerry; // Successfully created Jerry
    }


    /**
     * @brief Creates a new Planet object with the given name and coordinates.
     *        Allocates memory dynamically for the Planet and its name.
     *
     * @param name String representing the planet's name.
     * @param x X-coordinate of the planet.
     * @param y Y-coordinate of the planet.
     * @param z Z-coordinate of the planet.
     * @return Pointer to the newly created Planet object on success, or NULL on memory allocation failure.
     */

    Planet* createPlanet(char *name,float x,float y, float z) {
        if (name == NULL) {
            return NULL;
        }
        // allocation to a planet object
        Planet* newPlanet = (Planet*) malloc(sizeof(Planet));
        //check if allocation is failed
        if (newPlanet == NULL) {
            return NULL;
        }

        newPlanet->name = (char*) malloc(strlen(name)+1);
        if (newPlanet->name ==NULL) {
            destroyPlanet(newPlanet);
            return NULL;
        }
        strcpy(newPlanet->name,name);

        newPlanet->x = x;
        newPlanet->y = y;
        newPlanet->z = z;

        return newPlanet;
    }


    /**
     * @brief Creates a new PhysicalCharacteristics object with the given name and value.
     *        Allocates memory dynamically for the object and its name.
     *
     * @param p_attributeName String representing the name of the physical characteristic.
     * @param valAttribute Float value of the physical characteristic.
     * @return Pointer to the newly created PhysicalCharacteristics object on success, or NULL on memory allocation failure.
     */

    PhysicalCharacteristics* createAttribute(char* p_attributeName, float valAttribute) {
        if (p_attributeName == NULL) {
            return NULL;
        }
        PhysicalCharacteristics* new_physical_cs = (PhysicalCharacteristics*) malloc(sizeof(PhysicalCharacteristics));
        if (new_physical_cs == NULL ) {
            return NULL;
        }
        new_physical_cs->characteristic = (char*) malloc(strlen(p_attributeName)+1);
        if (new_physical_cs->characteristic == NULL) {
            free(new_physical_cs);
            return NULL;
        }

        strcpy(new_physical_cs->characteristic,p_attributeName);
        new_physical_cs->val = valAttribute;
        return new_physical_cs;
    }

    /**
     * @brief Creates a new Origin object with the given planet and dimension name.
     *        Allocates memory dynamically for the Origin and its dimension name.
     *
     * @param p_planet Pointer to the Planet object associated with the Origin.
     * @param p_dimensionName String representing the dimension name.
     * @return Pointer to the newly created Origin object on success, or NULL on memory allocation failure.
     */

Origin* createOrigin(Planet* p_planet, char* p_dimensionName) {
    if (p_planet == NULL || p_dimensionName == NULL) {
        return NULL;
    }

    Origin* new_origin = (Origin*) malloc(sizeof(Origin));
    if (new_origin == NULL) {
        return NULL;
    }
    new_origin->planet = p_planet;
    new_origin->dimensionName = (char*) malloc(strlen(p_dimensionName)+1);
    if (new_origin->dimensionName == NULL) {
        free(new_origin);
        return NULL;
    }
    strcpy(new_origin->dimensionName,p_dimensionName);
    return new_origin;
}


/**
 * @brief Checks if a specific physical characteristic exists in a Jerry object.
 *
 * @param p_jerry Pointer to the Jerry object to check.
 * @param p_attributeName String representing the name of the physical characteristic.
 * @return true if the characteristic exists, false otherwise.
 */

bool isExistAttribute(Jerry* p_jerry, char* p_attributeName){
    if (p_jerry == NULL || p_attributeName == NULL) {
        return false;
    }

    for (int i=0; i < p_jerry->numOfAttributes; i++) {
        if(strcmp(p_jerry->attributes[i]->characteristic,p_attributeName) == 0) {
            return true;
        }
    }
    return false;
    }


/**
 * @brief Adds a new physical characteristic to a Jerry object.
 *        Performs reallocation if the array of attributes needs to grow.
 *
 * @param p_jerry Pointer to the Jerry object.
 * @param p_physical_char Pointer to the PhysicalCharacteristics object to add.
 * @return success on successful addition, failure if the characteristic already exists, or memoryProblem on allocation failure.
 */

status addAttribute(Jerry* p_jerry, PhysicalCharacteristics* p_physical_char) {
    if ( p_physical_char == NULL || p_jerry == NULL) {
        return failure;
    }
    if (isExistAttribute(p_jerry,p_physical_char->characteristic)) {
        return failure;
    }
    PhysicalCharacteristics** new_attributes = (PhysicalCharacteristics**) realloc(p_jerry->attributes,(p_jerry->numOfAttributes +1) * sizeof(PhysicalCharacteristics*));
    if (new_attributes == NULL) {
        return memoryProblem;
    }
    p_jerry->attributes = new_attributes;
    new_attributes[p_jerry->numOfAttributes] = p_physical_char;
    p_jerry->numOfAttributes++;
    return success;
}

/**
 * @brief Deletes a physical characteristic from a Jerry object by its name.
 *        If the array becomes empty, the memory allocated for it is freed.
 *
 * @param p_jerry Pointer to the Jerry object.
 * @param p_characteristicName String representing the name of the characteristic to delete.
 * @return success on successful deletion, failure if the characteristic is not found, or memoryProblem on reallocation failure.
 */

status deleteAttribute(Jerry* p_jerry, char* p_characteristicName) {
    if (p_jerry == NULL || p_characteristicName == NULL) {
        return failure;
    }
    for (int i=0; i< p_jerry->numOfAttributes; i++) {
        if (strcmp(p_jerry->attributes[i]->characteristic, p_characteristicName) == 0) {
           if (destroyPhysicalCharacteristics(p_jerry->attributes[i]) == failure) {
               return failure;
           }

            for (int j = i; j < p_jerry->numOfAttributes - 1; j++) {
                p_jerry->attributes[j] = p_jerry->attributes[j + 1];
            }
            p_jerry->numOfAttributes--;

                if (p_jerry->numOfAttributes == 0) {
                    free(p_jerry->attributes);
                    p_jerry->attributes = NULL;
                } else {
                    PhysicalCharacteristics** updated_attributes = (PhysicalCharacteristics**) realloc(p_jerry->attributes,(p_jerry->numOfAttributes) * sizeof(PhysicalCharacteristics*));
                    if (updated_attributes == NULL){
                        return failure;
                    }
                    // the allocation is good
                    // update the pointer to the new memeory allocation
                    p_jerry->attributes = updated_attributes;
                    }
            return success;
        }
    }
    // attribute was not found
    return failure;
}




/**
 * @brief Prints the details of a Jerry object, including its ID, happiness level, origin, planet, and physical characteristics.
 *
 * @param p_jerry Pointer to the Jerry object to print.
 * @return success on successful print, or failure if the Jerry pointer is NULL.
 */

status printJerry(Jerry* p_jerry) {
    if (p_jerry == NULL) {
        return failure;
    }


    printf("Jerry , ID - %s : \n",p_jerry->ID);
    printf("Happiness level : %d \n",p_jerry->happinessLevel);
    printf("Origin : %s \n",p_jerry->origin->dimensionName);
    printf("Planet : %s ",p_jerry->origin->planet->name);
    printf("(%.2f,%.2f,%.2f)\n",
           p_jerry->origin->planet->x,
           p_jerry->origin->planet->y,
           p_jerry->origin->planet->z);

    // Check if there are any attributes
    if (p_jerry->numOfAttributes > 0) {
        printf("Jerry's physical Characteristics available : \n");
        printf("\t"); // Start with a tab
        // Iterate through all attributes and print them
        for (int i = 0; i < p_jerry->numOfAttributes; i++) {
            // Print the attribute's name and its value
            printf("%s : %.2f",p_jerry->attributes[i]->characteristic,(float)p_jerry->attributes[i]->val);

            // Add a comma and a space after the attribute
            // Only add if it's NOT the last attribute
            if (i < p_jerry->numOfAttributes - 1) {
                printf(" , ");
            }
        }
        printf(" \n");
    }
    return success;
}


/**
 * @brief Prints the details of a Planet object, including its name and coordinates.
 *
 * @param p_planet Pointer to the Planet object to print.
 * @return success on successful print, or failure if the Planet pointer is NULL.
 */

status printPlanet(Planet* p_planet) {
    if (p_planet == NULL) {
        return failure;
    }
    printf("Planet : %s (%.2f,%.2f,%.2f) \n",p_planet->name,p_planet->x,p_planet->y,p_planet->z);
    return success;
}




/**
 * @brief Frees all dynamically allocated memory associated with a Jerry object, including its attributes, ID, and origin.
 *
 * @param p_jerry Pointer to the Jerry object to destroy.
 * @return success on successful destruction, or failure if the Jerry pointer is NULL.
 */

status destroyJerry(Jerry* p_jerry) {
    //make sure jerry exists
    if (p_jerry == NULL) {
        return failure;
    }
    //iterate through the array of attribute and free each pointer ,and it's content
    //at the end free the array itself
    //make sure there is allocation of memory to attributes array
    if (p_jerry->attributes != NULL) {
        for (int i = 0; i < p_jerry->numOfAttributes; i ++) {
            if (p_jerry->attributes[i] != NULL) {
                if(p_jerry->attributes[i]->characteristic != NULL) {
                    free(p_jerry->attributes[i]->characteristic);
                    p_jerry->attributes[i]->characteristic = NULL; //set to null
                }
                free(p_jerry->attributes[i]);
                p_jerry->attributes[i] = NULL; //set to null
            }
        }

    }
    //free the attributes pointer
    free(p_jerry->attributes);
    p_jerry->attributes = NULL; //set to null
    //free ID
    if (p_jerry->ID != NULL) {
        free(p_jerry->ID);
        p_jerry->ID = NULL;
    }

    if (p_jerry->origin != NULL) {
        if(p_jerry->origin->dimensionName != NULL) {
            free(p_jerry->origin->dimensionName);
            p_jerry->origin->dimensionName = NULL; //set to null
        }

        if (p_jerry->origin->planet != NULL) {
            p_jerry->origin->planet = NULL; //set tu null 'dont free now the planet
        }
        free(p_jerry->origin);
    }
    free(p_jerry);
    return success;
}



/**
 * @brief Frees all dynamically allocated memory associated with an Origin object, including its dimension name.
 *
 * @param origin Pointer to the Origin object to destroy.
 * @return success on successful destruction, or failure if the Origin pointer is NULL.
 */

status destroyOrigin(Origin* origin) {
    if (origin == NULL) {
        return failure; // Nothing to destroy
    }

    // Free the dynamically allocated dimensionName
    if (origin->dimensionName != NULL) {
        free(origin->dimensionName);
        origin->dimensionName = NULL;
    }

    // Free the Origin struct itself
    free(origin);

    return success; // Destruction successful
}


/**
 * @brief Frees all dynamically allocated memory associated with a Planet object, including its name.
 *
 * @param p_planet Pointer to the Planet object to destroy.
 * @return success on successful destruction, or failure if the Planet pointer is NULL.
 */

status destroyPlanet(Planet* p_planet) {
    if (p_planet == NULL) {
        return failure;
    }

    if (p_planet->name != NULL) {
        free(p_planet->name);
        p_planet->name = NULL;
    }
    free(p_planet);
    return success;
}

/**
 * @brief Frees all dynamically allocated memory associated with a PhysicalCharacteristics object, including its name.
 *
 * @param p_PhysicalCharacteristics Pointer to the PhysicalCharacteristics object to destroy.
 * @return success on successful destruction, or failure if the PhysicalCharacteristics pointer is NULL.
 */
status destroyPhysicalCharacteristics(PhysicalCharacteristics* p_PhysicalCharacteristics) {
    if (p_PhysicalCharacteristics == NULL) {
        return failure;
    }
    if (p_PhysicalCharacteristics->characteristic != NULL) {
        free(p_PhysicalCharacteristics->characteristic);
        p_PhysicalCharacteristics->characteristic = NULL;
    }
    free(p_PhysicalCharacteristics);
    return success;
}

