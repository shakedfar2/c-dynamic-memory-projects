#ifndef JERRY_H
#define JERRY_H
#include "Defs.h"


/**
 * @brief Represents a planet in space.
 */
typedef struct {
    char *name; //pointer to string
    float x, y, z; //coordinates in space
} Planet;


/**
 * @brief Represents the origin of a Jerry, including its planet and dimension.
 */
//defining the origin struct
typedef struct {
    Planet *planet; // pointer to the jerry's planet
    char *dimensionName; // pointer to string
} Origin;


/**
 * @brief Represents a physical characteristic of a Jerry.
 */
//defining the PhysicalCharacteristics struct
typedef struct {
    char *characteristic; // pointer to the characteristic
    float val; // the value of the characteristic

} PhysicalCharacteristics;

/**
 * @brief Represents a Jerry, including its attributes, happiness level, and origin.
 */
typedef struct {
    char *ID; // pointer to string (id)
    int happinessLevel; // the level of happiness
    Origin *origin; // a pointer to the Jerry's origin
    PhysicalCharacteristics **attributes; // pointer to pointer to dynamic array of attributes
    int numOfAttributes; // number of attributes in the array
} Jerry;



//declarations
/**
 * @brief Creates a new Jerry object.
 *
 * @param p_origin Pointer to the origin of the Jerry.
 * @param p_id Pointer to the Jerry's ID.
 * @param happinessLevel Initial happiness level of the Jerry.
 * @return Pointer to the newly created Jerry, or NULL if memory allocation fails.
 */
Jerry* createJerry(Origin* p_origin, char* p_id, int happinessLevel);

/**
 * @brief Creates a new Planet object.
 *
 * @param name Name of the planet.
 * @param x X-coordinate of the planet.
 * @param y Y-coordinate of the planet.
 * @param z Z-coordinate of the planet.
 * @return Pointer to the newly created Planet, or NULL if memory allocation fails.
 */
Planet* createPlanet(char *name,float x,float y, float z);

/**
 * @brief Creates a new physical characteristic.
 *
 * @param p_attributeName Name of the characteristic (e.g., "Height").
 * @param valAttribute Value of the characteristic.
 * @return Pointer to the newly created PhysicalCharacteristics object, or NULL if memory allocation fails.
 */

PhysicalCharacteristics* createAttribute(char* p_attributeName, float valAttribute);


/**
 * @brief Creates a new Origin object.
 *
 * @param p_planet Pointer to the planet of origin.
 * @param p_dimensionName Name of the dimension where the planet resides.
 * @return Pointer to the newly created Origin, or NULL if memory allocation fails.
 */

Origin* createOrigin(Planet* p_planet, char* p_dimensionName);


/**
 * @brief Checks if a Jerry has a specific physical characteristic.
 *
 * @param p_jerry Pointer to the Jerry to check.
 * @param p_attributeName Name of the characteristic to check.
 * @return true if the characteristic exists, false otherwise.
 */

bool isExistAttribute(Jerry* p_jerry, char* p_attributeName);

/**
 * @brief Adds a new physical characteristic to a Jerry.
 *
 * @param p_jerry Pointer to the Jerry.
 * @param p_physical_char Pointer to the PhysicalCharacteristics to add.
 * @return success if the characteristic was added, or memoryProblem if allocation fails.
 */

status addAttribute(Jerry* p_jerry, PhysicalCharacteristics* p_physical_char);


/**
 * @brief Deletes a physical characteristic from a Jerry.
 *
 * @param p_jerry Pointer to the Jerry.
 * @param p_attributeName Name of the characteristic to delete.
 * @return success if the characteristic was deleted, or failure if it doesn't exist.
 */
status deleteAttribute(Jerry* p_jerry, char* p_attributeName);

/**
 * @brief Prints the details of a Jerry.
 *
 * @param p_jerry Pointer to the Jerry to print.
 * @return success if the Jerry was printed, or failure if the Jerry pointer is NULL.
 */

status printJerry(Jerry* p_jerry);

/**
 * @brief Prints the details of a Planet.
 *
 * @param p_planet Pointer to the Planet to print.
 * @return success if the Planet was printed, or failure if the Planet pointer is NULL.
 */

status printPlanet(Planet* p_planet);


/**
 * @brief Frees all memory allocated for a Jerry, including its attributes and origin.
 *
 * @param p_jerry Pointer to the Jerry to destroy.
 * @return success if the memory was freed, or failure if the pointer is NULL.
 */
status destroyJerry(Jerry* p_jerry);

/**
 * @brief Frees all memory allocated for an Origin object.
 *
 * @param origin Pointer to the Origin to destroy.
 * @return success if the memory was freed, or failure if the pointer is NULL.
 */

status destroyOrigin(Origin* origin);

/**
 * @brief Frees all memory allocated for a Planet object.
 *
 * @param p_planet Pointer to the Planet to destroy.
 * @return success if the memory was freed, or failure if the pointer is NULL.
 */

status destroyPlanet(Planet* p_planet);


/**
 * @brief Frees all memory allocated for a PhysicalCharacteristics object.
 *
 * @param p_PhysicalCharacteristics Pointer to the PhysicalCharacteristics to destroy.
 * @return success if the memory was freed, or failure if the pointer is NULL.
 */
status destroyPhysicalCharacteristics(PhysicalCharacteristics* p_PhysicalCharacteristics);





#endif // JERRY_H
