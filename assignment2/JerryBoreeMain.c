#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "KeyValuePair.h"
#include "LinkedList.h"
#include "HashTable.h"
#include "MultiValueHashTable.h"
#include "Jerry.h"
#include "Defs.h"
#define MAX_LINE_LENGTH 301 //defining the temporary array size for each line from the file
#define INT_MAX 2147483647
#define FLT_MAX 3.402823e+38


/**
 * @brief Transforms a string key into a numeric hash.
 * @param str The string key to transform.
 * @return The numeric hash of the string, or -1 on failure.
 */
int TransformKey(char * str) {
    if (str == NULL ) {
        return -1;
    }
    int sum = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        sum += (int)str[i];
    }

    return sum;
}


/**
 * @brief Wrapper function to transform an Element key into a numeric hash.
 * @param elem The Element key to transform.
 * @return The numeric hash of the key.
 */

int WrapperTransformKey(Element elem) {
    return TransformKey((char* )elem);
}



/**
 * @brief Compares two Jerry objects for equality.
 * @param jerry1 Pointer to the first Jerry.
 * @param jerry2 Pointer to the second Jerry.
 * @return true if the Jerries are equal, false otherwise.
 */
bool compareJerry(Element jerry1, Element jerry2) {
    if (jerry1 == NULL || jerry2 == NULL) {
        return false;
    }

    Jerry* j1 = (Jerry*)jerry1;
    Jerry* j2 = (Jerry*)jerry2;
    return j1 == j2;
}



/**
 * @brief Compares two strings for equality.
 * @param str1 The first string.
 * @param str2 The second string.
 * @return true if the strings are equal, false otherwise.
 */
bool EqualString(char* str1, char* str2) {
    if (str1 == NULL || str2 == NULL) {
        return false;
    }
    return strcmp(str1, str2) == 0;
}


/**
 * @brief Wrapper function to compare two Element strings for equality.
 * @param elem1 The first Element string.
 * @param elem2 The second Element string.
 * @return true if the strings are equal, false otherwise.
 */
bool WrapperEqualString(Element elem1, Element elem2) {
    return EqualString((char*)elem1, (char*)elem2);
}



/**
 * @brief Copies a Jerry object.
 * @param p_jerry Pointer to the Jerry to copy.
 * @return Pointer to the copied Jerry, or NULL on failure.
 */
Jerry* copyJerry(Jerry* p_jerry) {
    if (p_jerry == NULL) {
        return NULL;
    }
    return p_jerry;
}

/**
 * @brief Wrapper function to copy a Jerry as an Element.
 * @param element The Element to copy.
 * @return A new copy of the Jerry as an Element.
 */
Element WrapperCopyJerry(Element element) {
    return (Element)copyJerry((Jerry* )element);
}


/**
 * @brief Copies a string.
 * @param string The string to copy.
 * @return Pointer to the copied string, or NULL on failure.
 */
char* copyString(char* string) {
    if (string == NULL) {
        return NULL;
    }

    size_t len = strlen(string) + 1;
    char* new_str = (char*)malloc(len);
    if (new_str == NULL) {
        return NULL;
    }
    strncpy(new_str, string, len);
    new_str[len-1] = '\0';  // Ensure null termination
    return new_str;
}

/**
 * @brief Wrapper function to copy a string as an Element.
 * @param element The Element to copy.
 * @return A new copy of the string as an Element.
 */
Element WrapperCopyString(Element element) {
    return (Element)copyString((char*)element);
}


/**
 * @brief Frees a string.
 * @param element The Element string to free.
 * @return status indicating success or failure.
 */
status FreeString(Element element) {
    if (element != NULL) {
        free(element);
    }
    return success;
}


/**
 * @brief Prints a string.
 * @param str The string to print.
 * @return status indicating success or failure.
 */
status PrintString(char * str) {
    if (str == NULL) {
        return failure;
    }
    printf("%s\n", str);
    return success;
}


/**
 * @brief Wrapper function to print a string as an Element.
 * @param element The Element string to print.
 * @return status indicating success or failure.
 */
status WrapperPrintString(Element element) {
    return PrintString((char*) element);
}


/**
 * @brief Prints a Jerry object.
 * @param p_jerry Pointer to the Jerry to print.
 * @return status indicating success or failure.
 */
status PrintJerry(Jerry* p_jerry) {
    if (p_jerry == NULL) {
        return failure;
    }
    printJerry(p_jerry);
    return success;
}

/**
 * @brief Wrapper function to print a Jerry as an Element.
 * @param element The Element Jerry to print.
 * @return status indicating success or failure.
 */
status WrapperPrintJerry(Element element) {
    return PrintJerry((Jerry*) element);
}



/**
 * @brief A no-operation free function.
 * @param element The Element to "free".
 * @return success.
 */
status noOpFree(Element element) {
    return success;
}




/**
 * @brief Frees a Jerry object.
 * @param p_jerry Pointer to the Jerry to free.
 * @return status indicating success or failure.
 */
status freeJerry(Jerry* p_jerry) {
    if (p_jerry == NULL) {
        return failure;
    }
    if (destroyJerry(p_jerry) == failure) {
        return failure;
    }
    return success;
}

/**
 * @brief Wrapper function to free a Jerry as an Element.
 * @param element The Element Jerry to free.
 * @return status indicating success or failure.
 */
status WrapperfreeJerry(Element element) {
    return freeJerry((Jerry*)element);
}



// helper function for fitting each jerry to his planet
// if there is a common planet all Jerries will point to the same planet , no duplicates
/**
 * @brief Finds a planet by name in an array of planets.
 * @param planets Array of Planet pointers.
 * @param num_of_planets Number of planets in the array.
 * @param planetName The name of the planet to find.
 * @return Pointer to the planet if found, or NULL otherwise.
 */

Planet* findPlanetByName(Planet** planets, int num_of_planets, char* planetName) {
    for (int i = 0; i < num_of_planets; i++) {
        if (strcmp(planets[i]->name, planetName) == 0) {
            // returns the pointer to the planet
            return planets[i];
        }
    }
    return NULL; // Planet not found
}

/**
 * @brief Removes a Jerry from all associated characteristics in a MultiValueHashTable.
 * @param characteristicTable The MultiValueHashTable to update.
 * @param jerry Pointer to the Jerry to remove.
 * @return success if successful, failure otherwise.
 */
status removeJerryFromCharacteristics(MultiValueHashTable characteristicTable, Jerry* jerry) {
    if (characteristicTable == NULL || jerry == NULL) {
        return failure; // Invalid input
    }

    // Loop through all attributes of the Jerry
    for (int i = 0; i < jerry->numOfAttributes; i++) {
        char* attributeName = jerry->attributes[i]->characteristic;
        if (removeFromMultiValueHashTable(characteristicTable, (Element)attributeName, (Element)jerry) == failure) {
            return failure; // Stop if any removal fails
        }
    }

    return success; // Successfully removed Jerry from all attributes
}


/**
 * @brief Reads physical characteristics of a Jerry from a file.
 * @param p_file Pointer to the file.
 * @param p_jerry Pointer to the Jerry.
 * @param characteristicTable MultiValueHashTable for characteristics.
 * @return success if successful, failure otherwise.
 */
status readPhysicalCharacteristics(FILE* p_file, Jerry* p_jerry, MultiValueHashTable characteristicTable) {

    char tempLine[MAX_LINE_LENGTH];


    //read the next attributes
    while (fgets(tempLine, sizeof(tempLine), p_file)) {
        if (tempLine[0] != '\t') {
            // Line does not start with a tab - probably a new Jerry
            fseek(p_file, -strlen(tempLine), SEEK_CUR);// take the line one step back
            break;
        }
        // Split the characteristic line into name and value
        char* characteristicName = strtok(tempLine + 1, ":");
        char* val = strtok(NULL, ":");
        if (characteristicName == NULL || val == NULL) {
            return failure; // Invalid format
        }

        // Convert value to float and create the characteristic
        float value = atof(val);
        PhysicalCharacteristics* characteristic = createAttribute(characteristicName, value);
        if (characteristic == NULL){
            return memoryProblem;
        }
        if (addAttribute(p_jerry, characteristic) == failure) {
            return memoryProblem;
        }
        //add the Jerry to the MultivalueHashTable for this characteristic
        if (addToMultiValueHashTable(characteristicTable,characteristicName,p_jerry) != success) {
            return memoryProblem;
        }
    }
    return success;
}




/**
 * @brief Reads planet data from a file and populates the planets array.
 * @param p_file Pointer to the file.
 * @param p_p_planet Array of Planet pointers.
 * @param num_of_planets Number of planets to read.
 * @return success if successful, failure otherwise.
 */
status readPlanets(FILE* p_file ,Planet** p_p_planet, int num_of_planets) {

    //temporary static array for each line from the file
    char tempLine[MAX_LINE_LENGTH];

    //skip the planets word
    if (fgets(tempLine,sizeof(tempLine),p_file) == NULL) {
        return failure;
        }


    //reading the planet's information from the file
    for (int i = 0; i < num_of_planets ; i++) {
        // store each line into temporary buffer tempLine
        if (fgets(tempLine,sizeof(tempLine),p_file) == NULL) {
            return failure;
        }

        //find the position of the first comma that come after the word planets
        //return pointer to the position
        char* comma_position = strchr(tempLine,',');
        if (comma_position == NULL) {
            return failure;
        }

        *comma_position = '\0';
        char*  name = tempLine;
        float x, y, z;

        if (sscanf(comma_position + 1, "%f,%f,%f", &x, &y, &z) != 3) {
            return failure;
        }

        p_p_planet[i] = createPlanet(name, x, y, z);
        if (p_p_planet[i] == NULL) {
            return memoryProblem;
        }
    }
    return success;
    }


/**
 * @brief Reads Jerry data from a file and populates the system.
 * @param p_file Pointer to the file.
 * @param jerries_table HashTable for Jerries.
 * @param characteristicTable MultiValueHashTable for characteristics.
 * @param p_p_planets Array of Planet pointers.
 * @param num_of_planets Number of planets in the array.
 * @param jerriesByOrder LinkedList for ordered Jerries.
 * @return success if successful, failure otherwise.
 */
status readJerries(FILE* p_file, hashTable jerries_table, MultiValueHashTable characteristicTable, Planet** p_p_planets, int num_of_planets, LinkedList jerriesByOrder) {
    char tempLine[MAX_LINE_LENGTH];
    Jerry* currentJerry = NULL;

    // Skip the word "Jerries"
    if (fgets(tempLine, sizeof(tempLine), p_file) == NULL) {
        return failure;
    }

    // Reading the Jerries information
    while (fgets(tempLine, sizeof(tempLine), p_file) != NULL) {
        // Handle physical characteristics
        if (tempLine[0] == '\t') {
            fseek(p_file, -strlen(tempLine), SEEK_CUR); // Return the file pointer to the current line
            status characteristicStatus = readPhysicalCharacteristics(p_file, currentJerry, characteristicTable);
            if (characteristicStatus != success) {
                return characteristicStatus;
            }
            continue;
        }

        // Parse the details of one Jerry
        char* p_ID = strtok(tempLine, ",");
        char* p_dimensionName = strtok(NULL, ",");
        char* p_planetName = strtok(NULL, ",");
        char* happinessStr = strtok(NULL, ",");
        if (p_ID == NULL || p_dimensionName == NULL || p_planetName == NULL || happinessStr == NULL) {
            continue; // Skip to the next line instead of failing completely
        }

        // Convert happiness level to integer
        int happiness = atoi(happinessStr);

        // Find the planet object based on the planet name
        Planet* associatedPlanet = findPlanetByName(p_p_planets, num_of_planets, p_planetName);
        if (associatedPlanet == NULL) {
            continue;
        }

        // Create the Jerry object
        currentJerry = createJerry(p_ID, associatedPlanet, p_dimensionName, happiness);
        if (currentJerry == NULL) {
            return memoryProblem;
        }

        if (addToHashTable(jerries_table, p_ID, currentJerry) != success) {
            destroyJerry(currentJerry);
            continue; // Skip to the next line
        }

        // Add the Jerry to the order list
        if (appendNode(jerriesByOrder, (Element)currentJerry) == failure) {
            removeFromHashTable(jerries_table, p_ID);
            return memoryProblem;
            continue; // Skip to the next line

        }

    }

    return success;
}



/**
 * @brief Prints the main menu for the user.
 */
// Function to print the menu
void printMenu() {
    printf("Welcome Rick, what are your Jerry's needs today ? \n");
    printf("1 : Take this Jerry away from me \n");
    printf("2 : I think I remember something about my Jerry \n");
    printf("3 : Oh wait. That can't be right \n");
    printf("4 : I guess I will take back my Jerry now \n");
    printf("5 : I can't find my Jerry. Just give me a similar one \n");
    printf("6 : I lost a bet. Give me your saddest Jerry \n");
    printf("7 : Show me what you got \n");
    printf("8 : Let the Jerries play \n");
    printf("9 : I had enough. Close this place \n");
}


/**
 * @brief Handles adding a Jerry to the daycare.
 * @param allJerries HashTable of all Jerries.
 * @param planets Array of Planet pointers.
 * @param numOfPlanets Number of planets.
 * @param jerries_by_order LinkedList of ordered Jerries.
 * @param char_table MultiValueHashTable for characteristics.
 * @return success if successful, failure otherwise.
 */
status TakeThisJerryAwayFromMe(hashTable allJerries, Planet** planets, int numOfPlanets,LinkedList jerries_by_order, MultiValueHashTable char_table) {
    if (allJerries == NULL || planets == NULL || numOfPlanets <= 0 || jerries_by_order == NULL || char_table == NULL) {
        return failure; // Invalid input
    }

    // Jerry's ID
    char jerryID[MAX_LINE_LENGTH];
    printf("What is your Jerry's ID ? \n");
    if (scanf("%300s", jerryID) != 1) {
        printf("check invalid input");
        return displayMenu;
    }

    // Check if the Jerry ID already exists in the system
    if (lookupInHashTable(allJerries, jerryID) != NULL) {
        printf("Rick did you forgot ? you already left him here ! \n");
        return displayMenu; // ID already exists
    }

    //the jerry doesn't exist in daycare
    // ask for the Jerry's planet
    char planetName[MAX_LINE_LENGTH];
    printf("What planet is your Jerry from ? \n");
    if (scanf("%300s", planetName) != 1) {
        return displayMenu;
    }

    // Check if the planet exists in the system
    Planet* planet = findPlanetByName(planets, numOfPlanets, planetName);
    if (planet == NULL) {
        printf("%s is not a known planet ! \n",planetName);
        return displayMenu; // Planet does not exist
    }

    // Ask for the dimension and happiness level
    char dimension[MAX_LINE_LENGTH];
    printf("What is your Jerry's dimension ? \n");
    if (scanf("%300s", dimension) != 1) {
        return displayMenu;
    }

    int happinessLevel;
    printf("How happy is your Jerry now ? \n");
    if (scanf("%d", &happinessLevel) != 1) {
        return displayMenu;
    }

    // Create a new Jerry object
    Jerry* newJerry = createJerry(jerryID, planet, dimension, happinessLevel);
    if (newJerry == NULL) {
        return memoryProblem;
    }


    // Add the new Jerry to the hash table
    if (addToHashTable(allJerries, newJerry->ID, (Element)newJerry) != success) {
        destroyJerry(newJerry); // Free the Jerry object
        return memoryProblem;
    }

    // Add the Jerry to the order list
    if (appendNode(jerries_by_order, (Element)newJerry) != success) {
        destroyJerry(newJerry); // Free the Jerry object
        return memoryProblem;
    }



    // Print the Jerry details and return to the main menu
    PrintJerry(newJerry);
    return success;
}



/**
 * @brief Prints all Jerries associated with a specific physical characteristic.
 *
 * @param characteristicTable MultiValueHashTable containing physical characteristics.
 * @param characteristicName Name of the physical characteristic to search for.
 * @return success if printing succeeded, otherwise failure.
 */

status PrintJerriesByKnownCharacteristic(MultiValueHashTable characteristicTable, const char* characteristicName) {
    // Validate input parameters
    if (characteristicTable == NULL || characteristicName == NULL) {
        return failure;
    }

    // Look up the list of Jerries associated with the given characteristic
    LinkedList JerriesList = lookupInMultiValueHashTable(characteristicTable, (Element)characteristicName);

    // Check if no list exists or if the list is empty
    if (JerriesList == NULL || getLengthList(JerriesList) == 0) {
        return failure; // No Jerries to display
    }

    // Print the characteristic name
    printf("%s : \n", characteristicName);

    // Print all Jerries in the list
    if (DisplayList(JerriesList) == failure) {
        return failure;
    }

    return success; // Printing completed successfully
}


//helper function for sub menu option 2

/**
 * @brief Menu function to print Jerries based on a user-selected physical characteristic.
 *
 * @param characteristicTable MultiValueHashTable of physical characteristics.
 * @return success if printing succeeded, otherwise failure or displayMenu in case of invalid input.
 */

status PrintJerriesByCharacteristicMenu(MultiValueHashTable characteristicTable) {
    // Check if the input is NULL
    if (characteristicTable == NULL) {
        return failure;
    }

    char attributeName[MAX_LINE_LENGTH];
    printf("What physical characteristics ? \n");
    if (scanf("%300s",attributeName) != 1) {
        while (getchar() != '\n'); //clean the buffer for the next time
        return displayMenu;
    }
    // Call the existing function to print Jerries
    status result = PrintJerriesByKnownCharacteristic(characteristicTable, attributeName);

    // If no Jerries were found, print a message and return to the main menu
    if (result == failure) {
        printf("Rick we can not help you - we do not know any Jerry's %s ! \n", attributeName);
        return displayMenu;
    }

    return success; // Return success if Jerries were printed successfully
}


/**
 * @brief Adds a new physical characteristic to a Jerry based on its ID.
 *
 * @param allJerries HashTable containing all Jerries.
 * @param characteristicTable MultiValueHashTable of physical characteristics.
 * @return success if the characteristic was added successfully, otherwise failure.
 */

status IthinkIrememberSomethingAboutMyJerry(hashTable allJerries,MultiValueHashTable characteristicTable) {
    if (allJerries == NULL || characteristicTable == NULL) {
        return failure;
    }
    char JerryID[MAX_LINE_LENGTH];
    printf("What is your Jerry's ID ? \n");
    if (scanf("%300s", JerryID) != 1) {
        return displayMenu;
    }

    //check if the Jerry exists
    Jerry* jerry = (Jerry*)lookupInHashTable(allJerries,JerryID);
    if (jerry == NULL) {
        printf("Rick this Jerry is not in the daycare ! \n");
        return displayMenu;
    }

    // Ask for the name of the physical characteristic
    char characteristicName[MAX_LINE_LENGTH];
    printf("What physical characteristic can you add to Jerry - %s ? \n",JerryID);
    if (scanf("%300s", characteristicName) != 1) {
        return displayMenu;
    }
    if (isExistAttribute(jerry,characteristicName) == true) {
        printf("The information about his %s already available to the daycare ! \n",characteristicName);
        return displayMenu;
    }
    //the characteristic is not exist
    float ValCharacteristic;
    printf("What is the value of his %s ? \n",characteristicName);
    if (scanf("%f",&ValCharacteristic) != 1) {
        return displayMenu;
    }
    PhysicalCharacteristics* newAttribute = createAttribute(characteristicName,ValCharacteristic);
    if (newAttribute == NULL) {
        return memoryProblem;
    }
    status addAttributeStatus = addAttribute(jerry, newAttribute);
    if (addAttributeStatus != success) {
        destroyPhysicalCharacteristics(newAttribute);
        return addAttributeStatus; // Return the specific failure reason
    }

    //Add the Jerry to the MultiValue Hash Table for this characteristic
    status addStatus = addToMultiValueHashTable(characteristicTable, (Element)characteristicName, (Element)jerry);
    if (addStatus != success) {
        destroyPhysicalCharacteristics(newAttribute); // Free the allocated memory
        return addStatus; // Return the error (failure or memoryProblem) as is
    }
    if (PrintJerriesByKnownCharacteristic(characteristicTable,characteristicName) == failure) {
        return failure;
    }
    return success;
}



/**
 * @brief Removes a Jerry from the system, including HashTable, linked list, and characteristic tables.
 *
 * @param allJerries HashTable containing all Jerries.
 * @param characteristics MultiValueHashTable of physical characteristics.
 * @param jerries_by_order LinkedList of Jerries.
 * @param jerry Pointer to the Jerry to be removed.
 * @return success if the Jerry was removed successfully, otherwise failure.
 */

status removeJerryFromSystem(hashTable allJerries, MultiValueHashTable characteristics,LinkedList jerries_by_order ,Jerry* jerry) {
    if (allJerries == NULL || characteristics == NULL || jerries_by_order == NULL ||jerry == NULL) {
        return failure;
    }

    if (removeJerryFromCharacteristics(characteristics,jerry) == failure) {
        return failure;
    }

    //  Remove Jerry from the linked list
    if (deleteNode(jerries_by_order,(Element)jerry) == failure) {
        return failure;
    }

    // remove Jerry from the main HashTable
    if (removeFromHashTable(allJerries, (Element)jerry->ID) == failure) {
        return failure; // If Jerry is not in the main HashTable
    }
    return success; // Successfully removed Jerry from the system
}



/**
 * @brief Returns a Jerry to its owner by identifying it and removing it from the system.
 *
 * @param allJerries HashTable of all Jerries.
 * @param charTable MultiValueHashTable of physical characteristics.
 * @param jerries_by_order LinkedList of Jerries.
 * @return success if the removal succeeded, otherwise displayMenu.
 */

status IguessIwillTakeBackMyJerryNow(hashTable allJerries, MultiValueHashTable charTable, LinkedList jerries_by_order) {
    if (allJerries == NULL) {
        return failure;
    }
    printf("What is your Jerry's ID ? \n");
    char JerryId[MAX_LINE_LENGTH];
    if (scanf("%300s",JerryId)!= 1) {
        return displayMenu;
    }
    // Check if the Jerry exists in the daycare
    Jerry* jerry =  lookupInHashTable(allJerries,(Element)JerryId);
    if (jerry == NULL) {
        printf("Rick this Jerry is not in the daycare ! \n");
        return displayMenu; // Jerry not found, return to the menu
    }
    if (removeJerryFromSystem(allJerries,charTable,jerries_by_order,jerry)==  failure ) {
        return failure;
    }
    printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");
    return displayMenu;
}



/**
 * @brief Removes a physical characteristic from a specific Jerry based on its ID.
 *
 * @param allJerries HashTable containing all Jerries.
 * @param characteristicTable MultiValueHashTable of physical characteristics.
 * @return success if the removal succeeded, otherwise failure or displayMenu.
 */


status OhWaitThatCantBeRight(hashTable allJerries,MultiValueHashTable characteristicTable) {
    if (allJerries == NULL || characteristicTable == NULL) {
        return failure;
    }
    char IdBuffer[MAX_LINE_LENGTH];
    printf("What is your Jerry's ID ? \n");
    if (scanf("%300s",IdBuffer) != 1) {
        return displayMenu;
    }
    //check if the jerry is existing in daycare
    Jerry* jerry = lookupInHashTable(allJerries,(Element)IdBuffer);
    if (jerry == NULL) {
        printf("Rick this Jerry is not in the daycare ! \n");
        return displayMenu;
    }
    printf("What physical characteristic do you want to remove from Jerry - %s ? \n",IdBuffer);
    char attributeBuffer[MAX_LINE_LENGTH];
    if (scanf("%300s",attributeBuffer)!= 1) {
        return displayMenu;
    }
    //check if the attribute is existing to jerry
    if(isExistAttribute(jerry,attributeBuffer) == false){
        printf("The information about his %s not available to the daycare ! \n",attributeBuffer);
        return displayMenu;
    }
    if (deleteAttribute(jerry,attributeBuffer) == failure) {
        return failure;
    }

    status removeStatus = removeFromMultiValueHashTable(characteristicTable, (Element)attributeBuffer, (Element)jerry);
    if (removeStatus == failure) {
        return failure;
    }
    printJerry(jerry);

    return success;
}


/**
 * @brief Retrieves the value of a specific physical characteristic for a Jerry.
 *
 * @param p_jerry Pointer to the Jerry.
 * @param attributeName Name of the physical characteristic.
 * @return The value of the characteristic, or -1 if not found.
 */

//helper function in order to find the value of attribute of spesific Jerry
float getAttributeValue(Jerry* p_jerry, char* attributeName) {
    if (p_jerry == NULL || attributeName == NULL) {
        return -1; // Error value
    }

    // Loop through the array of attributes
    for (int i = 0; i < p_jerry->numOfAttributes; i++) {
        if (p_jerry->attributes[i] != NULL &&
            strcmp(p_jerry->attributes[i]->characteristic, attributeName) == 0) {
            return p_jerry->attributes[i]->val; // Return the value
            }
    }

    return -1; // Attribute not found
}



/**
 * @brief Finds the most similar Jerry based on a given physical characteristic and value.
 *
 * @param allJerries HashTable containing all Jerries.
 * @param charactheristicTable MultiValueHashTable of physical characteristics.
 * @param jerries_by_order LinkedList of Jerries.
 * @return success if a similar Jerry was found and returned, otherwise displayMenu.
 */
status IcantFindMyJerryJustGiveMeAsimilarOne(hashTable allJerries,MultiValueHashTable charactheristicTable, LinkedList jerries_by_order) {
    if (allJerries == NULL || charactheristicTable == NULL) {
        return failure;
    }
    char attribute[MAX_LINE_LENGTH];
    printf("What do you remember about your Jerry ? \n");
    if (scanf("%300s",attribute) != 1) {
        return displayMenu;
    }
    LinkedList jerriesList = (LinkedList)lookupInMultiValueHashTable(charactheristicTable,attribute);
    if (jerriesList == NULL) {
        printf("Rick we can not help you - we do not know any Jerry's %s ! \n",attribute);
        return displayMenu;
    }
    float valueAttribute;
    printf("What do you remember about the value of his %s ? \n",attribute);
    if (scanf("%f",&valueAttribute)!=1) {
        return displayMenu;
    }
    Jerry* closestJerry = NULL;
    float closestDiff = FLT_MAX; //max value of float in c language

    // look for the mose similar Jerry
    int listLength = getLengthList(jerriesList); // get the length of the linked list
    // printf("Debug: Number of Jerries in list: %d\n", listLength);
    for (int i = 1; i <= listLength; i++) {
        Jerry* currentJerry = (Jerry*)getDataByIndex(jerriesList, i);
        if (currentJerry == NULL) {
            continue; // ignore empty node
        }


        // find the value of the current Jerry
        float currentValue = getAttributeValue(currentJerry,attribute);

        if (currentValue != -1) {
            float diff = fabs(currentValue - valueAttribute);
            if (diff < closestDiff) {
                closestDiff = diff;
                closestJerry = currentJerry;
            }
        }
    }
    if (closestJerry == NULL) {
        printf("Rick we couldn't find a Jerry with similar %s.\n", attribute);
        DisplayList(jerriesList);
        return displayMenu;
    }
    printf("Rick this is the most suitable Jerry we found : \n");
    if (printJerry(closestJerry) != success) {
        return failure;
    }
    if (removeJerryFromSystem(allJerries,charactheristicTable,jerries_by_order,closestJerry)== failure){
        return failure;
    }
    printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");
    return displayMenu;
}



/**
 * @brief Finds the saddest Jerry in the daycare and returns it to the user.
 *
 * @param allJerries HashTable containing all Jerries.
 * @param characteristicisTable MultiValueHashTable of physical characteristics.
 * @param jerriesByorder LinkedList of Jerries.
 * @return success if the saddest Jerry was found and returned, otherwise failure.
 */

status IlostAbetGiveMeYourSaddestJerry(hashTable allJerries, MultiValueHashTable characteristicisTable,LinkedList jerriesByorder) {
    if (allJerries == NULL || characteristicisTable == NULL || jerriesByorder == NULL) {
        return failure;
    }
    if (getLengthList(jerriesByorder) == 0) {
       printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
       return displayMenu;
   }
    Jerry* saddestJerry = NULL;
    int lowestHappiness =INT_MAX;
    int listLen = getLengthList(jerriesByorder);
    for (int i = 1; i <= listLen; i ++) {
        Jerry* currJerry = (Jerry*)getDataByIndex(jerriesByorder,i);
        if (currJerry == NULL) {
            continue;
        }
        int currHappiness = currJerry->happinessLevel;
        if (currHappiness < lowestHappiness) {
            lowestHappiness = currHappiness;
            saddestJerry = currJerry;
        }
    }
    printf("Rick this is the most suitable Jerry we found : \n");
    printJerry(saddestJerry);
    if (removeJerryFromSystem(allJerries,characteristicisTable,jerriesByorder,saddestJerry) == failure) {
        return failure;
    }
    printf("Rick thank you for using our daycare service ! Your Jerry awaits ! \n");
    return displayMenu;
}

/**
 * @brief Frees all allocated memory for planets and resets their pointers to NULL.
 *
 * @param p_p_planets Array of pointers to Planet objects.
 * @param num_of_planets Number of planets in the array.
 * @return success if all planets were freed successfully, otherwise failure.
 */

status freePlantes(Planet** p_p_planets, int num_of_planets) {
    if (p_p_planets != NULL) {
        for(int i = 0; i < num_of_planets; i++) {
            if(p_p_planets[i] != NULL) {
                destroyPlanet(p_p_planets[i]);
                p_p_planets[i] = NULL;
            }
        }
    }
    return success;
}




/**
 * @brief Frees all allocated resources in the system.
 *
 * @param allJerries HashTable of all Jerries.
 * @param characteristicTable MultiValueHashTable of physical characteristics.
 * @param allPlanets Array of pointers to Planet objects.
 * @param numOfPlanets Number of planets in the array.
 * @param jerriesByOrder LinkedList of ordered Jerries.
 * @return success if all resources were freed successfully, otherwise failure.
 */
status freeAllResources(hashTable allJerries, MultiValueHashTable characteristicTable, Planet** allPlanets, int numOfPlanets, LinkedList jerriesByOrder) {
    if (allJerries != NULL) {
        if (destroyHashTable(allJerries) == failure) {
            return failure;
        }
    }
    if (characteristicTable != NULL) {
        if (destroyMultiValueHashTable(characteristicTable) == failure) {
            return failure;
        }
    }
    if (allPlanets != NULL) {
        if (freePlantes(allPlanets, numOfPlanets) == failure) {
            return failure;
        }
    }
    if (jerriesByOrder != NULL) {
        if (DestroyLinkedList(jerriesByOrder) == failure) {
            return failure;
        }
    }
    return success;
}


/**
 * @brief Counts the number of unique physical characteristics in a file.
 *
 * @param p_file Pointer to the input file.
 * @return Number of unique characteristics found.
 */
int countUniqueCharacteristics(FILE* p_file) {
    char tempLine[MAX_LINE_LENGTH];
    char* uniqueCharacteristics[MAX_LINE_LENGTH];
    int Counter = 0;

    // Create a temporary array for unique characteristics
    for (int i = 0; i < MAX_LINE_LENGTH; i++) {
        uniqueCharacteristics[i] = NULL;
    }

    while (fgets(tempLine, sizeof(tempLine), p_file) != NULL) {
        if (tempLine[0] == '\t') {
            // Extract the characteristic name
            char* characteristicName = strtok(tempLine + 1, ":");
            if (characteristicName == NULL) {
                continue; // Skip invalid lines
            }

            // Check if the characteristic is already in the unique list
            bool isUnique = true;
            for (int i = 0; i < Counter; i++) {
                if (strcmp(uniqueCharacteristics[i], characteristicName) == 0) {
                    isUnique = false;
                    break;
                }
            }

            // If the characteristic is unique, add it to the list
            if (isUnique) {
                uniqueCharacteristics[Counter] = strdup(characteristicName);
                Counter++;
            }
        }
    }

    // Free memory used for temporary strings
    for (int i = 0; i < Counter; i++) {
        free(uniqueCharacteristics[i]);
    }

    // Rewind the file for further reading
    rewind(p_file);

    return Counter;
}



/**
 * @brief Checks if a given number is a prime number.
 *
 * @param num The number to check.
 * @return true if the number is prime, otherwise false.
 */

//function to check if number is a prime number
bool isPrime(int num) {
    if (num < 2) {
        return false;
    }
    for (int i = 2; i * i <= num; i++) {
        if (num % i == 0) return false;
    }
    return true;
}


/**
 * @brief Finds the next prime number greater than or equal to the given number.
 *
 * @param num The starting number.
 * @return The next prime number.
 */

int findNextPrime(int num) {
    while (!isPrime(num)) {
        num++;
    }
    return num;
}


/**
 * @brief Counts the number of Jerries listed in the input file.
 *
 * @param p_file Pointer to the input file.
 * @return Number of Jerries found.
 */

//helper function that counts the number of jerries before insert them
int countJerries(FILE* p_file) {
    char tempLine[MAX_LINE_LENGTH];
    int count = 0;
    bool isReadingJerries = false;

    while (fgets(tempLine, sizeof(tempLine), p_file) != NULL) {
        if (strncmp(tempLine, "Jerries", 7) == 0) {
            isReadingJerries = true;
            continue;
        }

        if (isReadingJerries && tempLine[0] != '\t' && strlen(tempLine) > 1) {
            count++;
        }
    }

    rewind(p_file);
    return count;
}




/**
 * @brief Calculates the optimal hash table size based on the number of elements and load factor.
 *
 * @param numOfElements Number of elements to store.
 * @param loadFactor Desired load factor (0 < loadFactor <= 1).
 * @return The size of the hash table, rounded up to the next prime number.
 */

int calculateHashTableSize(int numOfElements, float loadFactor) {
    if (loadFactor <= 0 || loadFactor > 1) {
        // Load factor must be between 0 and 1
        return -1; // Return error
    }

    // Calculate the initial size based on the load factor
    int tableSize = (int)(numOfElements / loadFactor);

    // Find the next prime number greater than or equal to the calculated size
    return findNextPrime(tableSize);
}

/**
 * @brief Prints all planets stored in the given array of Planet pointers.
 *
 * @param p_p_planets Array of pointers to Planet objects.
 * @param num_of_planets Number of planets in the array.
 * @return success if all planets were printed successfully, otherwise failure.
 */

status Allknownplanets(Planet** p_p_planets, int num_of_planets) {
    if (num_of_planets <= 0 || p_p_planets == NULL ) {
        return success;// Handle invalid input
    }

    // iterate the planet's array and print each of them
    for (int i = 0; i < num_of_planets; i++) {
        if (p_p_planets[i] != NULL) {
            if (printPlanet(p_p_planets[i]) == failure) {
                return failure; // If any planet fails to print
            }
        }
    }

    return success;
}




/**
 * @brief Prints all Jerries in the daycare.
 *
 * @param jerries_by_order LinkedList of Jerries.
 * @return success if all Jerries were printed successfully, otherwise failure.
 */

status AllJerries(LinkedList jerries_by_order) {
    if (jerries_by_order == NULL) {
        return failure;
    }
    if (DisplayList(jerries_by_order) == failure) {
        return failure;
    }
    return success;

}


void printSubMenu() {
    printf("What information do you want to know ? \n");
    printf("1 : All Jerries \n");
    printf("2 : All Jerries by physical characteristics \n");
    printf("3 : All known planets \n");
}


/**
 * @brief Displays information about Jerries or planets based on the user's selection.
 *
 * @param allJerries HashTable of all Jerries.
 * @param characteristicTable MultiValueHashTable of physical characteristics.
 * @param allPlanets Array of pointers to Planet objects.
 * @param jerries_by_order LinkedList of ordered Jerries.
 * @param numOfPlanets Number of planets in the array.
 * @return success if the operation succeeded, otherwise failure or displayMenu.
 */


status ShowMeWhatYouGot(hashTable allJerries, MultiValueHashTable characteristicTable,Planet** allPlanets, LinkedList jerries_by_order, int numOfPlanets) {
    if (allJerries == NULL || characteristicTable == NULL || allPlanets == NULL) {
        return failure;
    }
    status action_status;
    int choice;
    printSubMenu();
    if (scanf("%d",&choice) != 1) {
        printf("Rick, this option is not known to the daycare ! \n");
        while (getchar() != '\n');
        return displayMenu;
    }
    switch (choice) {
        case 1:
            action_status = AllJerries(jerries_by_order);
        break;
        case 2:
            action_status = PrintJerriesByCharacteristicMenu(characteristicTable);
        break;
        case 3:
            action_status = Allknownplanets(allPlanets,numOfPlanets);
        break;
        default:
            printf("Rick, this option is not known to the daycare ! \n");
        return displayMenu;
    }
    // Handle statuses
    if (action_status == failure) {
        return failure; // Pass the failure status to the main menu
    } else if (action_status == success) {
        return success; // Indicate success
    }
}


/**
 * @brief Displays a menu of activities for Jerries to participate in.
 *
 * This function prints the available activities:
 * 1. Interact with fake Beth.
 * 2. Play golf.
 * 3. Adjust the picture settings on the TV.
 */

void printActivityMenu() {
    printf("What activity do you want the Jerries to partake in ? \n");
    printf("1 : Interact with fake Beth \n");
    printf("2 : Play golf \n");
    printf("3 : Adjust the picture settings on the TV \n");
}



/**
 * @brief Updates the happiness level of a given Jerry by a specified amount.
 *
 * @param jerry Pointer to the Jerry whose happiness level is to be updated.
 * @param update The value to add (positive or negative) to the happiness level.
 *
 * Ensures that the happiness level remains within the bounds of [0, 100].
 */

void updateHappinessLevel(Jerry* jerry, int update) {
    if (jerry == NULL) return;

    jerry->happinessLevel += update;

    // Ensure happiness level is within the bounds [0, 100]
    if (jerry->happinessLevel > 100) {
        jerry->happinessLevel = 100;
    } else if (jerry->happinessLevel < 0) {
        jerry->happinessLevel = 0;
    }
}


/**
 * @brief Allows Jerries to interact with fake Beth, updating their happiness levels.
 *
 * - Jerries with happiness level >= 20 gain 15 happiness points.
 * - Jerries with happiness level < 20 lose 5 happiness points.
 *
 * @param jerriesByOrder LinkedList of Jerries to interact with fake Beth.
 * @return success if the activity was completed successfully, otherwise failure.
 *
 * Prints the updated list of Jerries after the activity.
 */

status InteractWithFakeBeth(LinkedList jerriesByOrder) {
    if (jerriesByOrder == NULL) {
        return failure;
    }

    int listLen = getLengthList(jerriesByOrder);
    for (int i = 1; i <= listLen; i++) {
        Jerry* currentJerry = (Jerry*)getDataByIndex(jerriesByOrder, i);
        if (currentJerry == NULL) continue;

        // Update happiness level based on current happiness
        if (currentJerry->happinessLevel >= 20) {
            updateHappinessLevel(currentJerry, 15);
        } else {
            updateHappinessLevel(currentJerry, -5);
        }
    }

    printf("The activity is now over ! \n");
    DisplayList(jerriesByOrder); // Print updated Jerries
    return success;
}



/**
 * @brief Allows Jerries to play golf, updating their happiness levels.
 *
 * - Jerries with happiness level >= 50 gain 10 happiness points.
 * - Jerries with happiness level < 50 lose 10 happiness points.
 *
 * @param jerriesByOrder LinkedList of Jerries to participate in the golf activity.
 * @return success if the activity was completed successfully, otherwise failure.
 *
 * Prints the updated list of Jerries after the activity.
 */

status PlayGolf(LinkedList jerriesByOrder) {
    if (jerriesByOrder == NULL) {
        return failure;
    }

    int ListLen = getLengthList(jerriesByOrder);
    for (int i = 1; i <= ListLen; i++) {
        Jerry* currJerry = (Jerry*)getDataByIndex(jerriesByOrder, i);
        if (currJerry == NULL) continue;

        // Update happiness level based on current happiness
        if (currJerry->happinessLevel >= 50) {
            updateHappinessLevel(currJerry, 10);
        } else {
            updateHappinessLevel(currJerry, -10);
        }
    }

    printf("The activity is now over ! \n");
    DisplayList(jerriesByOrder); // Print updated Jerries
    return success;
}


/**
 * @brief Allows Jerries to adjust picture settings on the TV, increasing their happiness levels.
 *
 * - All Jerries gain 20 happiness points regardless of their current happiness level.
 *
 * @param jerriesByOrder LinkedList of Jerries to participate in the activity.
 * @return success if the activity was completed successfully, otherwise failure.
 *
 * Prints the updated list of Jerries after the activity.
 */

status AdjustPictureSettings(LinkedList jerriesByOrder) {
    if (jerriesByOrder == NULL) {
        return failure;
    }

    int listLen = getLengthList(jerriesByOrder);
    for (int i = 1; i <= listLen; i++) {
        Jerry* currentJerry = (Jerry*)getDataByIndex(jerriesByOrder, i);
        if (currentJerry == NULL) continue;

        // Increase happiness by 20 for all Jerries
        updateHappinessLevel(currentJerry, 20);
    }

    printf("The activity is now over ! \n");
    DisplayList(jerriesByOrder); // Print updated Jerries
    return success;
}




/**
 * @brief Allows Jerries to participate in selected activities and updates their happiness levels.
 *
 * @param jerries_by_order LinkedList of Jerries.
 * @return success if the operation succeeded, otherwise displayMenu.
 */

status LetTheJerriesPlay(LinkedList jerries_by_order) {
    if (getLengthList(jerries_by_order) == 0 || jerries_by_order == NULL) {
        printf("Rick we can not help you - we currently have no Jerries in the daycare ! \n");
        return displayMenu;
    }

    // Print the activity menu
    printActivityMenu();

    int activityChoice;
    if (scanf("%d", &activityChoice) != 1) {
        while (getchar() != '\n'); // Clear the input buffer
        printf("Rick this option is not known to the daycare ! \n");
        return displayMenu;
    }

    status actionStatus;
    switch (activityChoice) {
        case 1:
            actionStatus = InteractWithFakeBeth(jerries_by_order);
        break;
        case 2:
            actionStatus = PlayGolf(jerries_by_order);
        break;
        case 3:
            actionStatus = AdjustPictureSettings(jerries_by_order);
        break;
        default:
            printf("Rick this option is not known to the daycare ! \n");
        return displayMenu;
    }
    if (actionStatus == success) {
        return success;
    } else if (actionStatus == failure) {
        return failure;
    }
}



/**
 * @brief Closes the Jerry Daycare by freeing all allocated resources.
 *
 * This function calls `freeAllResources` to:
 * - Destroy the `allJerries` hash table.
 * - Destroy the `characteristicTable` MultiValueHashTable.
 * - Free memory allocated for the array of planets.
 * - Destroy the linked list of Jerries (`jerriesByOrder`).
 *
 * @param allJerries HashTable containing all Jerries.
 * @param characteristicTable MultiValueHashTable of physical characteristics.
 * @param jerriesByOrder LinkedList of Jerries in the daycare.
 * @param planets Array of pointers to Planet objects.
 * @param numOfPlanets Number of planets in the array.
 * @return success if all resources were freed successfully, otherwise failure.
 */

status CloseTheDaycare(hashTable allJerries, MultiValueHashTable characteristicTable, LinkedList jerriesByOrder, Planet** planets, int numOfPlanets) {
    // Call the existing function to free all resources
    status result = freeAllResources(allJerries, characteristicTable, planets, numOfPlanets, jerriesByOrder);
    if (result != success) {
        return result;
    }
    return success;
}





/**
 * @brief Main function of the Jerry Daycare system.
 *
 * This function initializes the daycare system by:
 * 1. Parsing command-line arguments for the number of planets and input file name.
 * 2. Reading the file to populate Jerries, planets, and physical characteristics.
 * 3. Creating the data structures required for the system:
 *    - `allJerries`: A hash table to store Jerries by their ID.
 *    - `characteristicTable`: A MultiValueHashTable to map physical characteristics to Jerries.
 *    - `jerriesByOrder`: A linked list of Jerries, in order of insertion.
 * 4. Displaying an interactive menu to perform various operations on Jerries:
 *    - Adding, removing, or updating Jerry characteristics.
 *    - Viewing Jerries or planets.
 *    - Engaging Jerries in activities.
 *    - Returning or removing Jerries from the daycare.
 * 5. Handling user input and executing corresponding actions using a `switch` statement.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments:
 *        - argv[1]: Number of planets (integer).
 *        - argv[2]: File name of the input data (string).
 * @return Exit status:
 *         - Returns 0 if the program terminates successfully.
 *         - Returns 1 if an error occurs during initialization or resource management.
 *
 * @note The program will display appropriate error messages for memory allocation issues
 *       or invalid input and will clean up all resources before exiting.
 */

int main(int argc, char *argv[] ) {
    if (argc != 3) {
        return 1;
    }

    // convert the parameters (string) from the command into integer
    int numOfPlanets = atoi(argv[1]);
    // file's name
    char* filename = argv[2];


    //open the file
    FILE* p_file = fopen(filename,"r");
    if (p_file == NULL) {
        return 1;
    }


    //pre-reading for counting the jerries
    int numOfJerries = countJerries(p_file);
    if (numOfJerries <= 0) {
        fclose(p_file);
        return 1;
    }

    int HashNumber = calculateHashTableSize(numOfJerries,0.5);

    //pre-reading for counting the jerrie's characteristics
    int num0fCharacteristic = countUniqueCharacteristics(p_file);
    int HashMultiNumber = calculateHashTableSize(num0fCharacteristic,0.5);
    // const array to planets
    Planet* allPlanets[numOfPlanets];

    // Initialize array to NULL
    for (int i = 0; i < numOfPlanets; i++) {
        allPlanets[i] = NULL;
    }

    if (numOfPlanets > 0 ) {
        //read the file
        // return the right messages to the user
        status planetStatus = readPlanets(p_file, allPlanets, numOfPlanets);
        if(planetStatus != success) {
            if(planetStatus == memoryProblem) {
                printf("Memory Problem \n");
            }
            fclose(p_file);
            freePlantes(allPlanets,numOfPlanets);
            return 1;
        }
    }




    hashTable allJerries = createHashTable(WrapperCopyString,FreeString,WrapperPrintString,WrapperCopyJerry,WrapperfreeJerry,WrapperPrintJerry,WrapperEqualString,WrapperTransformKey,HashNumber);
    if (allJerries == NULL) {
        printf("Memory Problem \n");
        fclose(p_file);
        freePlantes(allPlanets,numOfPlanets);
        return 1;;
    }
    MultiValueHashTable characteristicTable = createMultiValueHashTable(WrapperCopyString,FreeString,WrapperPrintString,WrapperCopyJerry,noOpFree,WrapperPrintJerry,compareJerry,WrapperEqualString,WrapperTransformKey,HashMultiNumber);
    if (characteristicTable == NULL) {
        printf("Memory Problem \n");
        fclose(p_file);
        destroyHashTable(allJerries);
        freePlantes(allPlanets,numOfPlanets);
        return 1;
    }

    LinkedList jerriesByOrder = createLinkedList(WrapperCopyJerry, noOpFree, WrapperPrintJerry, NULL, compareJerry);
    if (jerriesByOrder == NULL) {
        printf("Memory Problem \n");
        fclose(p_file);
        freeAllResources(allJerries,characteristicTable,allPlanets,numOfPlanets,NULL);
        return 1;
    }


    //read the jerries
    status status_reading = readJerries(p_file,allJerries,characteristicTable,allPlanets,numOfPlanets,jerriesByOrder);
    fclose(p_file);
    p_file = NULL;
    if (status_reading != success) {
        if (status_reading == memoryProblem) {
            printf("Memory Problem \n");
        }
        freeAllResources(allJerries,characteristicTable,allPlanets,numOfPlanets,jerriesByOrder);
            return 1;
        }

    int choice; // choice variable will store the user's choice

    while (1) {
        // Infinite loop to keep showing the menu
        printMenu(); // Display the menu to the user
        if (scanf("%d", &choice) != 1) { // Check if input is not an integer
            printf("Rick this option is not known to the daycare ! \n");
            while (getchar() != '\n'); // Clear the input buffer
            continue;
        }


        status actionStatus; // This will store the result of each action

        switch (choice) {
            case 1:
                actionStatus = TakeThisJerryAwayFromMe(allJerries, allPlanets, numOfPlanets,jerriesByOrder,characteristicTable);
            break;
            case 2:
                actionStatus = IthinkIrememberSomethingAboutMyJerry(allJerries,characteristicTable);
            break;
            case 3:
                actionStatus = OhWaitThatCantBeRight(allJerries,characteristicTable);
            break;
            case 4:
                actionStatus = IguessIwillTakeBackMyJerryNow(allJerries,characteristicTable,jerriesByOrder);
            break;
            case 5:
                actionStatus = IcantFindMyJerryJustGiveMeAsimilarOne(allJerries,characteristicTable, jerriesByOrder);
            break;
            case 6:
                actionStatus = IlostAbetGiveMeYourSaddestJerry(allJerries, characteristicTable,jerriesByOrder);
            break;

            case 7:
                actionStatus = ShowMeWhatYouGot(allJerries,characteristicTable,allPlanets,jerriesByOrder,numOfPlanets);
            break;
            case 8:
                actionStatus = LetTheJerriesPlay(jerriesByOrder);
            break;
            case 9:
                actionStatus =  CloseTheDaycare(allJerries, characteristicTable, jerriesByOrder, allPlanets, numOfPlanets);
                printf("The daycare is now clean and close ! \n");
                return 0;
            default:
            printf("Rick this option is not known to the daycare ! \n");
            continue;
        }

        // Handle statuses
        if (actionStatus == memoryProblem) {
            printf("Memory Problem \n");
            freeAllResources(allJerries, characteristicTable,allPlanets, numOfPlanets,jerriesByOrder);
            return 1; // Exit the program
        } else if (actionStatus == failure) {
            freeAllResources(allJerries, characteristicTable,allPlanets, numOfPlanets,jerriesByOrder);
            return 1; // Exit the program
        } else if (actionStatus == displayMenu) {
            continue; // Reprint the menu
        } else if (actionStatus == success) {
            // Do nothing, let the loop continue naturally
        }
    }
}
