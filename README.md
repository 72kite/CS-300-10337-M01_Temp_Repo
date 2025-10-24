# CS-300-10337-M01_Temp_Repo
CS-300 Course Planner - Binary Search Tree implementation for managing academic course data with prerequisite tracking. Includes Big O analysis of multiple data structure approaches.

# CS-300 Portfolio - Data Structures and Algorithms

## Project Reflection

### What was the problem you were solving in the projects for this course?

The primary problem I addressed was developing a course management system for ABCU's Computer Science program. Academic advisors needed a tool to quickly access course information, view the complete course catalog in alphanumeric order, and understand prerequisite relationships between courses. The challenge was building a functional system and determining which data structure would provide optimal performance for the specific use cases advisors would encounter daily.

### How did you approach the problem? Consider why data structures are important to understand.

I approached this problem by analyzing three different data structures: vectors, hash tables, and binary search trees. For each structure, I developed complete pseudocode implementations and conducted runtime analysis to understand their performance characteristics. Understanding data structures was necessary because the choice directly impacts user experience. Through my analysis, I discovered that vectors had O(n²) loading time due to nested prerequisite validation loops, hash tables achieved O(n) loading with O(1) average search time, and BSTs provided O(n log n) loading with naturally sorted data. This understanding allowed me to make an informed recommendation (hash table for frequent lookups) while implementing the BST in Project Two for its natural ordering advantage.

### How did you overcome any roadblocks you encountered while going through the activities or project?

Several roadblocks emerged during implementation. First, handling prerequisite validation required multiple passes through the data to ensure all referenced courses existed in the system. I overcame this by implementing a three-pass loading system: reading courses, validating prerequisites, and inserting into the BST. Second, managing memory properly in C++ with dynamic node allocation was challenging. I addressed this by implementing a proper destructor with recursive node deletion to prevent memory leaks. Third, providing case-insensitive course searches required careful string manipulation with uppercase conversion. I also encountered issues with input buffer management when switching between numeric and string inputs, which I resolved using cin.ignore() with numeric_limits. Each roadblock was an opportunity to deepen my understanding of both C++ and algorithm design.

### How has your work on this project expanded your approach to designing software and developing programs?

This project changed how I approach software design by emphasizing the importance of choosing the right tool for the job. Previously, I might have used whatever data structure came to mind first. Now, I understand that design decisions should be driven by analysis of actual use cases. The project taught me to consider multiple dimensions: runtime complexity, space complexity, implementation complexity, and maintainability. I learned that the optimal solution is not always the fastest one. The BST I implemented is not technically the fastest for searches, but its automatic sorting eliminates the need for additional sorting algorithms, making the codebase simpler and more maintainable. This view of software design, balancing performance with code quality, will influence future projects.

### How has your work on this project evolved the way you write programs that are maintainable, readable, and adaptable?

This project changed my coding practices in several ways. First, I embraced comprehensive commenting and clear structural organization. Every major section has header comments explaining its purpose. Second, I implemented modular design with helper functions like trim(), toUpperCase(), and parseLine() that can be reused and tested independently. Third, I added error handling at every potential failure point: file operations, invalid input, missing data, and memory allocation. Fourth, I provided informative user feedback throughout the program, from data loading summaries to helpful tips when courses are not found. The code is adaptable. Adding new features like course deletion or modification would be straightforward given the clean separation of concerns. I learned that readable code is not just about formatting but about making architectural decisions that future developers can understand and build upon. The three-pass validation system, while more complex than a single pass, makes the logic clear and maintainable.

---

## Repository Contents

- **Project One Analysis**: Complete pseudocode implementations and runtime analysis (Big O) for Vector, Hash Table, and Binary Search Tree approaches
- **Project Two Implementation**: Working C++ code implementing a Binary Search Tree for course management with full sorting and search functionality

## Technologies Used

- C++
- Binary Search Tree data structure
- File I/O operations
- Algorithm analysis and optimization
