#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Jerry.h"

#define MAX_LINE_LENGTH 301 //defining the temporary array size for each line from the file


// helper function for fitting each jerry to his planet
// if there is a common planet all Jerries will point to the same planet , no duplicates
/**
 * @brief Finds a planet by name in the array of planets.
 * @param planets Array of pointers to planets.
 * @param num_of_planets Number of planets in the array.
 * @param planetName The name of the planet to find.
 * @return Pointer to the planet if found, NULL otherwise.
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
 * @brief Reads the physical characteristics of a Jerry from the file.
 *        Assumes that the file pointer is at the start of the characteristics section.
 * @param p_file Pointer to the file to read from.
 * @param p_jerry Pointer to the Jerry whose characteristics are being read.
 * @return `success` if successful, or an appropriate error status otherwise.
 */

status readPhysicalCharacteristics(FILE* p_file, Jerry* p_jerry) {

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
        if (characteristic == NULL || addAttribute(p_jerry, characteristic) == failure) {
            destroyPhysicalCharacteristics(characteristic);
            return memoryProblem;
        }
    }
    return success;
}




/**
 * @brief Reads planet data from the file and populates the planets array.
 * @param p_file Pointer to the file to read from.
 * @param p_p_planet Array of pointers to store the planets.
 * @param num_of_planets Number of planets to read.
 * @return `success` if successful, or an appropriate error status otherwise.
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
 * @brief Reads Jerry data from the file and populates the Jerries array.
 * @param p_file Pointer to the file to read from.
 * @param p_p_jerries Array of pointers to store the Jerries.
 * @param p_p_planets Array of pointers to planets (for matching origins).
 * @param num_of_planets Number of planets in the array.
 * @return `success` if successful, or an appropriate error status otherwise.
 */

status readJerries(FILE* p_file,Jerry** p_p_jerries, Planet ** p_p_planets, int num_of_planets) {
    char tempLine[MAX_LINE_LENGTH];
    Jerry* currentJerry =NULL;
    int jerryIndex = 0;

    //skip the word Jerries
    if(fgets(tempLine,sizeof(tempLine),p_file) == NULL) {
        return failure;
    }

    //reading the jerries information
    while (fgets(tempLine, sizeof(tempLine), p_file) !=NULL) {
        if(tempLine[0] == '\t') {
            if ( currentJerry == NULL) {
                printf("Error: Found physical characteristic without a Jerry.\n");
                return failure;
            }
            // Return the file pointer to the current line for the physical characteristics reader
            fseek(p_file, -strlen(tempLine), SEEK_CUR);

            status characteristicStatus = readPhysicalCharacteristics(p_file, currentJerry);
            if (characteristicStatus != success){
                return characteristicStatus;
                    }
            continue;
        }

        //take into parts the details of one Jerry
        char* p_ID = strtok(tempLine,",");
        char* p_dimensionName = strtok(NULL,",");
        char* p_planetName = strtok(NULL, ",");
        char* happinessStr = strtok(NULL, ",");
        if (p_ID == NULL || p_dimensionName == NULL || p_planetName == NULL || happinessStr == NULL) {
            return failure; // Invalid Jerry line format
        }

        // Convert happiness level to integer
        int happiness = atoi(happinessStr);

        // Find the planet object based on the planet name
        Planet* associatedPlanet = findPlanetByName(p_p_planets,num_of_planets,p_planetName);
        if (associatedPlanet == NULL) {
            return failure; // Planet not found
        }

        // Create the origin object
        Origin* p_origin = createOrigin(associatedPlanet,p_dimensionName);
        if (p_origin == NULL) {
            return memoryProblem; // Failed to create origin
        }


        // Create the Jerry object
        currentJerry = createJerry(p_origin, p_ID, happiness);
        if (currentJerry == NULL) {
            destroyOrigin(p_origin);
            return memoryProblem; // Failed to create Jerry
        }
        p_p_jerries[jerryIndex++] = currentJerry;
    }
    return success;
}


/**
 * @brief Converts a string to uppercase by modifying it in place.
 *
 * @param input Pointer to the string to be converted.
 */

void toUpperCase(char* input) {
    for (int i = 0; input[i] != '\0'; i++) {
        if (input[i] >= 'a' && input[i] <= 'z') {
            input[i] = input[i] - ('a' - 'A');
        }
    }
}

/**
 * @brief Prints all planets stored in the given array of Planet pointers.
 *
 * @param p_p_planets Array of pointers to Planet objects.
 * @param num_of_planets Number of planets in the array.
 * @return success if all planets were printed successfully, or failure if any planet fails to print.
 */

status printAllPlanets(Planet** p_p_planets, int num_of_planets) {
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
 * @brief Displays the program's menu with all available actions for the user.
 */

// Function to print the menu
void printMenu() {
    printf("AW JEEZ RICK, what do you want to do now ? \n");
    printf("1 : Print all Jerries \n");
    printf("2 : Print all Planets \n");
    printf("3 : Add physical characteristic to Jerry \n");
    printf("4 : Remove physical characteristic from Jerry \n");
    printf("5 : Print Jerries by a planet \n");
    printf("6 : Print Jerries by a physical characteristic \n");
    printf("7 : Go home \n");
}



/**
 * @brief Prints all Jerries stored in the given array of Jerry pointers.
 *
 * @param p_p_jerries Array of pointers to Jerry objects.
 * @param num_of_jerries Number of Jerries in the array.
 * @return success if all Jerries were printed successfully, or failure if any Jerry fails to print.
 */

status printAllJerries(Jerry** p_p_jerries, int num_of_jerries) {
    if (num_of_jerries <= 0 || p_p_jerries == NULL ) {
        return success; // Handle invalid input
    }

    // iterate the Jerrie's array and print each of them
    for (int i = 0; i < num_of_jerries; i++) {
        if (p_p_jerries[i] != NULL) {
            if (printJerry(p_p_jerries[i]) == failure) {
                return failure; // If any Jerry fails to print
            }
        }
    }

    return success;
}



/**
 * @brief Adds a physical characteristic to a specific Jerry identified by its ID.
 *
 * @param p_p_jerries Array of pointers to Jerry objects.
 * @param num_of_jerries Number of Jerries in the array.
 * @return success if the attribute was added successfully,
 *         memoryProblem if memory allocation failed,
 *         or displayMenu for invalid input.
 */

status Add_attribute_to_Jerry(Jerry **p_p_jerries,int num_of_jerries) {
    // static temp array for user's input
    char buffer[MAX_LINE_LENGTH];
    float bufferValAttribute;


    printf("What is your Jerry's ID ? \n");
    if(scanf("%300s", buffer) != 1) {
        return displayMenu;
    }

    Jerry* p_jerry = NULL;
    for(int i = 0; i < num_of_jerries; i++) {
        if(strcmp(p_p_jerries[i]->ID,buffer) == 0){
            p_jerry = p_p_jerries[i];
            break;
        }
    }

    if(p_jerry == NULL) {
        printf("OH NO! I CAN'T FIND HIM RICK ! \n");
        return displayMenu;
    }

    printf("What physical characteristic can you add to Jerry - %s ? \n",p_jerry->ID);
    if(scanf("%300s",buffer) != 1){
        return displayMenu;
    }

    if (isExistAttribute(p_jerry,buffer)) {
        toUpperCase(buffer);
        printf("RICK I ALREADY KNOW HIS %s ! \n",buffer);
        return displayMenu;
    }



    printf("What is the value of his %s ? \n",buffer);
    if(scanf("%f", &bufferValAttribute) != 1) {
        return displayMenu;
    }

    PhysicalCharacteristics* p_characteristics = createAttribute(buffer,bufferValAttribute);
    if (p_characteristics == NULL ) {
        return memoryProblem;
    }


    status addAttributestatus = addAttribute(p_jerry,p_characteristics);
        if (addAttributestatus == memoryProblem) {
            destroyPhysicalCharacteristics(p_characteristics);
            return addAttributestatus;
        }

    printJerry(p_jerry);
    return success;
}



/**
 * @brief Removes a physical characteristic from a specific Jerry identified by its ID.
 *
 * @param p_p_jerries Array of pointers to Jerry objects.
 * @param num_of_jerries Number of Jerries in the array.
 * @return success if the attribute was removed successfully,
 *         failure if the attribute removal failed,
 *         or displayMenu for invalid input or if the Jerry was not found.
 */

status Remove_attribute_from_Jerry(Jerry** p_p_jerries, int num_of_jerries) {

    char buffer[MAX_LINE_LENGTH];

    printf("What is your Jerry's ID ? \n");
    if (scanf("%300s",buffer) != 1){
        return displayMenu;
    }

    Jerry* p_jerry = NULL;
    for (int i = 0; i < num_of_jerries; i++) {
        if(strcmp(p_p_jerries[i]->ID,buffer) == 0) {
            p_jerry = p_p_jerries[i];
            break;
        }
    }
    if (p_jerry == NULL) {
        printf("OH NO! I CAN'T FIND HIM RICK ! \n");
        return displayMenu;
    }
    printf("What physical characteristic do you want to remove from Jerry - %s ? \n",p_jerry->ID);
    if(scanf("%300s",buffer) != 1) {
        return displayMenu;
    }

    // check if the attribute is exist
    if (isExistAttribute(p_jerry,buffer) == false) {
        toUpperCase(buffer);
        printf("RICK I DON'T KNOW HIS %s ! \n",buffer);
        return displayMenu;
    }
    if (deleteAttribute(p_jerry,buffer) == failure) {
        return failure;
    }
    printJerry(p_jerry);
    return success;
}


/**
 * @brief Prints all Jerries that originate from a specific planet.
 *
 * @param p_p_jerries Array of pointers to Jerry objects.
 * @param p_p_planets Array of pointers to Planet objects.
 * @param num_of_planets Number of planets in the array.
 * @param num_of_jerries Number of Jerries in the array.
 * @return success if all relevant Jerries were printed successfully,
 *         or displayMenu if the planet was not found or user input was invalid.
 */

status Print_Jerries_by_a_planet(Jerry** p_p_jerries, Planet** p_p_planets,int num_of_planets, int num_of_jerries) {
    char inputBuffer[MAX_LINE_LENGTH];
    printf("What planet is your Jerry from ? \n");
    if (scanf("%300s",inputBuffer) != 1) {
        return displayMenu;
    }

    Planet* p_planet = NULL;
    for (int i = 0; i < num_of_planets; i++) {
        if (strcmp(p_p_planets[i]->name,inputBuffer) == 0) {
            p_planet = p_p_planets[i];
            break;
        }
    }



    if (p_planet == NULL) {
        toUpperCase(inputBuffer);
        printf("RICK I NEVER HEARD ABOUT %s ! \n",inputBuffer);
        return displayMenu;
    }

    bool found = false;
    for (int i = 0; i < num_of_jerries; i++) {
        if (strcmp(p_p_jerries[i]->origin->planet->name,p_planet->name) == 0) {
            printJerry(p_p_jerries[i]);
            found = true;
        }
    }
    if (found == false) {
        toUpperCase(inputBuffer);
        printf("OH NO! I DON'T KNOW ANY JERRIES FROM %s ! \n",inputBuffer);
        return displayMenu;
    }
    return success;
}


/**
 * @brief Prints all Jerries that have a specific physical characteristic.
 *
 * @param p_p_jerries Array of pointers to Jerry objects.
 * @param num_of_jerries Number of Jerries in the array.
 * @return success if all relevant Jerries were printed successfully,
 *         or displayMenu if no Jerries with the specified characteristic were found
 *         or if user input was invalid.
 */

status Print_Jerries_by_a_physical_characteristic(Jerry** p_p_jerries, int num_of_jerries) {
    char inputBuffer[MAX_LINE_LENGTH];

    printf("What do you know about your Jerry ? \n");
    if (scanf("%300s",inputBuffer) != 1) {
        return displayMenu;
    }

    bool found = false;
    for (int i = 0; i < num_of_jerries; i++) {
        Jerry* p_jerry = p_p_jerries[i];
        if(p_jerry == NULL) {
            continue;
        }
        if(isExistAttribute(p_jerry,inputBuffer) == true) {
            printJerry(p_jerry);
            found = true;
        }
    }

    if(found == false) {
        toUpperCase(inputBuffer);
        printf("OH NO! I DON'T KNOW ANY JERRY'S %s ! \n",inputBuffer);
        return displayMenu;
    }

    return success;
}

/**
 * @brief Frees all allocated memory for Jerries and Planets, and resets pointers to NULL.
 *
 * @param p_p_jerries Array of pointers to Jerry objects.
 * @param p_p_planets Array of pointers to Planet objects.
 * @param num_of_jerries Number of Jerries in the array.
 * @param num_of_planets Number of Planets in the array.
 */

void freeAllMemory(Jerry** p_p_jerries, Planet** p_p_planets, int num_of_jerries, int num_of_planets) {
    if (p_p_jerries !=NULL) {
        for(int i = 0; i < num_of_jerries; i++) {
            if(p_p_jerries[i] != NULL) {
                destroyJerry(p_p_jerries[i]);
                p_p_jerries[i] = NULL;
            }
        }
    }
    if (p_p_planets != NULL){
        for(int i = 0; i < num_of_planets; i++) {
            if(p_p_planets[i] != NULL) {
                destroyPlanet(p_p_planets[i]);
                p_p_planets[i] = NULL;
            }
        }

        printf("AW JEEZ RICK, ALL THE JERRIES GOT FREE ! \n");
    }
}



/**
 * @brief Main function: Initializes the program, reads input, and manages the main loop with user interactions.
 *
 * This function parses the command-line arguments to retrieve the number of planets, number of Jerries,
 * and the configuration file. It reads data from the file to initialize planets and Jerries, displays a
 * menu for user interaction, and handles memory cleanup upon program termination.
 *
 * @param argc Number of command-line arguments.
 *             Must be 4 (program name + 3 arguments).
 * @param argv Array of command-line argument strings:
 *             - argv[1]: Number of planets (string, converted to integer).
 *             - argv[2]: Number of Jerries (string, converted to integer).
 *             - argv[3]: Name of the configuration file.
 * @return 0 on successful execution, or 1 if an error occurs (e.g., invalid input, memory issues, or file handling errors).
 */

int main(int argc, char *argv[] ) {
    if (argc != 4) {
        return 1;
    }

    // convert the parameters (string) from the command into integer
    int numOfPlanets = atoi(argv[1]);
    int numOfJerries = atoi(argv[2]);
    // file's name
    char* filename = argv[3];

    //open the file
    FILE* p_file = fopen(filename,"r");
    if (p_file == NULL) {
        return 1;
    }


    Planet* allPlanets[numOfPlanets];
    Jerry* allJerries[numOfJerries];

    // Initialize arrays to NULL
    for (int i = 0; i < numOfPlanets; i++) {
        allPlanets[i] = NULL;
    }
    for (int i = 0; i < numOfJerries; i++) {
        allJerries[i] = NULL;
    }

    if (numOfPlanets > 0 ) {
        //read the file
        // return the right messages to the user
        status planetStatus = readPlanets(p_file, allPlanets, numOfPlanets);
        if(planetStatus != success) {
            if(planetStatus == memoryProblem) {
                printf("Memory Problem");
            }
            fclose(p_file);
            freeAllMemory(NULL,allPlanets, numOfJerries,numOfPlanets);
            return 1;
        }
    }

    if (numOfJerries > 0 ) {
        status jerryStatus = readJerries(p_file,allJerries,allPlanets,numOfPlanets);
        if (jerryStatus != success) {
            if (jerryStatus == memoryProblem) {
                printf("Memory Problem");
            }
            fclose(p_file);
            freeAllMemory(allJerries,allPlanets, numOfJerries,numOfPlanets);
            return 1;
        }
    }

    //after successful reading close the file
        fclose(p_file);


    int choice; // choice variable will store the user's choice

    while (1) {
        // Infinite loop to keep showing the menu
        printMenu(); // Display the menu to the user
        if (scanf("%d", &choice) != 1) { // Check if input is not an integer

            printf("RICK WE DON'T HAVE TIME FOR YOUR GAMES ! \n");
            while (getchar() != '\n'); // Clear the input buffer
            continue;
        }


        status actionStatus; // This will store the result of each action

        switch (choice) {
            case 1:
                actionStatus = printAllJerries(allJerries, numOfJerries);
            break;
            case 2:
                actionStatus = printAllPlanets(allPlanets, numOfPlanets);
            break;
            case 3:
                actionStatus = Add_attribute_to_Jerry(allJerries, numOfJerries);
            break;
            case 4:
                actionStatus = Remove_attribute_from_Jerry(allJerries, numOfJerries);
            break;
            case 5:
                actionStatus = Print_Jerries_by_a_planet(allJerries, allPlanets, numOfPlanets, numOfJerries);
            break;
            case 6:
                actionStatus = Print_Jerries_by_a_physical_characteristic(allJerries, numOfJerries);
            break;
            case 7:
                freeAllMemory(allJerries, allPlanets, numOfJerries, numOfPlanets);
            return 0; // Exit the program

            default:
                printf("RICK WE DON'T HAVE TIME FOR YOUR GAMES ! \n");
            break;
        }

        // Handle statuses
        if (actionStatus == memoryProblem) {
            printf("Memory Problem\n");
            freeAllMemory(allJerries, allPlanets, numOfJerries, numOfPlanets);
            return 1; // Exit the program
        } else if (actionStatus == failure) {
            freeAllMemory(allJerries, allPlanets, numOfJerries, numOfPlanets);
            return 1; // Exit the program
        } else if (actionStatus == displayMenu) {
            continue; // Reprint the menu
        } else if (actionStatus == success) {
            // Do nothing, let the loop continue naturally
        }
    }
}
