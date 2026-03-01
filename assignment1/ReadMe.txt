The program manages a collection of "Jerries," fictional characters from the Rick and Morty universe, along with their associated planets and physical characteristics. It allows users to interact with the data through a menu-driven interface. The main functionalities include:

Reading Configuration Data:

The program reads information about planets and Jerries from a configuration file, creating dynamic objects for each entity.
Memory Management:

The program dynamically allocates memory for objects and their properties (e.g., IDs, names, attributes) and ensures proper cleanup to avoid memory leaks.
If any memory allocation failure occurs, the program immediately exits after releasing all allocated memory to maintain stability and avoid undefined behavior.
User Interaction via Menu:

Users can perform actions such as:
Print all Jerries or planets.
Add or remove physical characteristics for a specific Jerry.
Search Jerries by planet or physical characteristics.
Exit the program and release all allocated memory.
Data Relationships:

Each Jerry is associated with a planet (its origin).
Planets are shared objects among Jerries to avoid duplication.
Jerries can have multiple dynamic physical characteristics.
Error Handling for Smooth User Experience:

The program prioritizes modularity and readability by breaking down functionalities into separate functions to reduce complexity in the main flow.
Invalid inputs or errors trigger graceful recovery without crashing the menu, allowing "Rick" to interact smoothly with the program.
Consistent Status Handling Conventions:

Memory Allocation Failures:
Functions encountering memory allocation problems return memoryProblem.
These functions ensure that any partially allocated memory is released before returning, avoiding memory leaks.
Logical Errors:
Functions encountering logical issues, such as invalid input or missing dependencies, return failure.
Successful Execution:
Functions that complete their operations successfully return success.
Testing and Validation:

The program was tested using valgrind to ensure no memory leaks or invalid memory accesses.
Edge cases, such as missing data or invalid configuration, were considered to prevent unexpected behavior.
This modular, cleanly implemented program focuses on robust error handling, modularity, and adherence to clear conventions for resource and status management. This ensures a seamless experience for the user while maintaining code clarity and stability.
