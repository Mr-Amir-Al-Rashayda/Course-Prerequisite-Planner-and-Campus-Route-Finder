# Birzeit University - Campus Navigation and Course Planner

## Description
This C program implements a system to assist students at Birzeit University with two key tasks:
1.  **Campus Navigation:** Finding the shortest route between two buildings on campus using Dijkstra's algorithm.
2.  **Course Planning:** Sorting courses based on their prerequisites using topological sort.

The program reads building distances and course prerequisites from specified input files and provides an interactive menu for the user to perform various operations.

## Features
- Load building distance data from `input_buildings.txt`.
- Load course prerequisite data from `input_courses.txt`.
- Calculate the shortest distance between two specified buildings.
- Print the shortest route and total distance between two specified buildings.
- Sort courses based on prerequisites using topological sort.
- Print the topologically sorted list of courses.
- Interactive menu for selecting operations.
- Basic error handling for file loading and invalid input.

## Input Files
Two input files are required and must be present in the same directory as the executable:

1.  `input_buildings.txt`:
    - Contains pairs of buildings and the direct distance between them.
    - Each line should be in the format: `Building1#Building2#Distance`
    - Example: `Masri#A.Shaheen#70`
    - The file should be populated with all required building connections and distances as per the project description table.

2.  `input_courses.txt`:
    - Contains courses and their prerequisites.
    - Each line should be in the format: `CourseName#Prerequisite1#Prerequisite2#...`
    - Prerequisites are separated by `#`. If a course has no prerequisites, it should just be the `CourseName`.
    - Example: `COMP3340#COMP233#COMP133`
    - The file should be populated with all courses and their prerequisites as per the project description table.

## How to Compile and Run
1.  Save the C code (provided) as a `.c` file (e.g., `P4_1222596_AmirAlRashayda_1.c`).
2.  Ensure the `input_buildings.txt` and `input_courses.txt` files are in the same directory as the `.c` file.
3.  Open a terminal or command prompt.
4.  Navigate to the directory where the files are saved.
5.  Compile the code using a C compiler like GCC:
    ```bash
    gcc P4_1222596_AmirAlRashayda_1.c -o project
    ```
    *(Replace `P4_1222596_AmirAlRashayda_1.c` with your actual filename)*
    *(Replace `project` with your desired executable name)*
6.  Run the compiled executable:
    ```bash
    ./project
    ```
    *(On Windows, you might just type `project.exe`)*
7.  Follow the menu prompts to interact with the application.

## Implementation Details
- **Building Graph:** Represented using an adjacency list built with linked lists.
- **Shortest Path:** Implemented using Dijkstra's algorithm with a Min-Heap for efficient vertex extraction.
- **Course Prerequisites:** Represented as a directed graph where an edge from course A to course B means A is a prerequisite for B.
- **Course Sorting:** Implemented using Topological Sort based on Depth First Search (DFS) and a stack to store the sorted order.

## Author
- Name: Amir Rasmi Al-Rashayda
