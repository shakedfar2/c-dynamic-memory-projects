# JerryBoree System Documentation

## System Overview
The **JerryBoree** management system is designed with a modular and efficient architecture to handle Jerry objects, their characteristics, and planets. The system emphasizes clear structure, efficient memory management, and performance optimization.

### Core Data Structures
1. **HashTable**:
   - Maps Jerry IDs to Jerry objects for O(1) lookups.
   - Uses chaining for collision handling and dynamic resizing with a load factor of 0.5.
2. **MultiValueHashTable**:
   - Maps characteristics to lists of Jerries, enabling many-to-one relationships.
   - Built on top of the HashTable.
3. **LinkedList**:
   - Maintains the original insertion order of Jerries for display purposes.

### Key Design Principles
1. **Modularity and Readability**:
   - Complex logic and long code blocks were refactored into smaller functions for:
     - Improved readability and easier debugging.
     - Code reusability across the system.
   - Examples include `readPlanets`, `readJerries`, and `freeAllResources`.
2. **Memory Management**:
   - Clear ownership: `allJerries` is responsible for managing Jerry memory, while other structures use shallow copies and fake free functions.
   - Error handling for allocation failures and resource cleanup on errors.
3. **Optimized Hash Functions**:
   - String hashing for Jerry IDs with prime number sizing for better distribution.
   - Efficient precalculation of hash table sizes based on data.

### System Features
- **Efficient Jerry Management**:
  - Fast lookup by ID using `allJerries`.
  - Characteristic-based searches via `characteristicTable`.
  - Original insertion order maintained in `jerriesByOrder`.
- **Shared Planets**:
  - Planets are shared between Jerries to save memory.
- **Dynamic Initialization**:
  - Pre-read files to calculate required sizes for tables and allocate memory dynamically.

### Special Considerations
- Jerry removal ensures consistent updates across all data structures.
- Activities update Jerries’ happiness levels, adhering to predefined rules.
- Robust error handling in file parsing and memory allocation.

This architecture ensures efficient operations, clear modularity, and robust system management.

