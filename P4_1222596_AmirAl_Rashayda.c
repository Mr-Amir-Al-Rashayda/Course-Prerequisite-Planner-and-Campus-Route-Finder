

// Name : Amir Rasmi Al-Rashayda

// ID : 1222596

// Sec : 1


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***************************************** "Global" *****************************************/

#define MAX_NODES 100   // Maximum number of nodes (buildings or courses)
#define INFINITY 999999 // A large number representing infinity
int counterPrintDij = 0;
int counterPrintTop = 0;
int counterLoad = 0;
/***************************************** "Structs Implementations" *****************************************/

// Struct to represent an edge in the graph
typedef struct Edge
{
    int Destination;   /// index of the Destination node
    int Weight;        /// Weight of the edge
    struct Edge *Next; /// pointer to the Next edge in the linked list
} Edge;

typedef struct Building
{
    char Name[30]; /// building Name
    Edge *Edges;   /// Linked list of Edges to other buildings
} Building;

typedef struct Course
{
    char CourseName[30];
    int PrereqCount;            // the number of prerequisites
    char Prerequisites[10][30]; // list of prerequisites
} Course;

typedef struct Stack
{
    int items[MAX_NODES];
    int top;
} Stack;

typedef struct MinHeapNode
{
    int vertex;
    int distance;
} MinHeapNode;

typedef struct MinHeap
{
    int size;
    int capacity;
    int *position;
    MinHeapNode **array;
} MinHeap;

// Global arrays to store buildings and courses
Building buildings[MAX_NODES];
int buildingCount = 0;

Course courses[MAX_NODES];
int courseCount = 0;

/***************************************** "Functions prototypes" *****************************************/

void loadFiles();
void dijkstra(char *startBuilding, char *endBuilding);
void printShortestRoute(char *startBuilding, char *endBuilding);
void topologicalSort();
void printSortedCourses();
int getBuildingIndex(char *Name);
int getCourseIndex(char *Name);
void addEdge(Building *building, int Destination, int Weight);
void dfs(int node, int visitedNodes[], Stack *stack, Course *adjList[MAX_NODES]);
void initializeStack(Stack *stack);
int isStackEmpty(Stack *stack);
void push(Stack *stack, int value);
int pop(Stack *stack);
MinHeap *createMinHeap(int capacity);
void swapMinHeapNode(MinHeapNode **a, MinHeapNode **b);
void minHeapify(MinHeap *minHeap, int index);
int isEmpty(MinHeap *minHeap);
MinHeapNode *extractMin(MinHeap *minHeap);
void decreaseKey(MinHeap *minHeap, int vertex, int distance);
int isInMinHeap(MinHeap *minHeap, int vertex);

/***************************************** "_________Main_________" *****************************************/

// Main function
int main()
{
    int userChoice = 0;
    char startBuilding[30], endBuilding[30]; // variables to store building Names

    printf("Hello my friend");
    while (userChoice != 6)
    {
        printf("\n\nWhich operation you want to do next sir :\n");
        printf("1. Load the two files\n");
        printf("2. Calculate the shortest distance between two buildings\n");
        printf("3. Print the shortest route between two buildings and the total distance\n");
        printf("4. Sort the courses using topological sort\n");
        printf("5. Print the sorted courses\n");
        printf("6. Exit the application\n\n");
        printf("Enter The Number Of Operation: ");
        scanf("%d", &userChoice); // Read user's choice

        switch (userChoice)
        {
        case 1:
            if (counterLoad == 0)
            {
                loadFiles(); // Call function to load data from files
                counterLoad++;
            }
            else
                printf("Can't load Twice !!");
            break;
        case 2:
            if (counterLoad)
            {
                printf("Enter start building: ");
                scanf("%s", startBuilding); // Read start building Name
                printf("Enter end building: ");
                scanf("%s", endBuilding);             // Read end building Name
                dijkstra(startBuilding, endBuilding); // Call function to calculate the shortest distance
            }
            else
                printf("Load The Data first !! (Choice number 1)");
            break;
        case 3:
            if (counterLoad)
            {
                counterPrintDij++;
                printf("Enter start building: ");
                scanf("%s", startBuilding); // Read start building Name
                printf("Enter end building: ");
                scanf("%s", endBuilding);                       // Read end building Name
                printShortestRoute(startBuilding, endBuilding); // Call function to print the shortest route
                counterPrintDij--;
            }
            else
                printf("Load The Data first !! (Choice number 1)");
            break;
        case 4:
            if (counterLoad)
                topologicalSort(); // Call function to sort courses using topological sort
            else
                printf("Load The Data first !! (Choice number 1)");
            break;
        case 5:
            if (counterLoad)
            {
                counterPrintTop++;
                printSortedCourses(); // Call function to print sorted courses
                counterPrintTop--;
            }
            else
                printf("Load The Data first !! (Choice number 1)");
            break;
        case 6:
            printf("Good Bye my friend \n");
            printf("Exiting...\n");
            break;
        default:
            printf("ERROR! Enter a valid number from the choices\n");
            continue;
        }
    }

    return 0; // Return 0 to indicate successful execution
}

/***************************************** "Functions" *****************************************/

void loadFiles()
{                           /// load the data from the files "Both Two Files"
    FILE *The_File_Pointer; // pointer for file operations
    char TheLine[256];      // buffer to read each TheLine from the file

    /******************Load Buildings******************/

    The_File_Pointer = fopen("input_buildings.txt", "r");
    if (The_File_Pointer == NULL)
    { // in case didn't open ..
        printf("Couldn't Open input_buildings.txt!! ERROR! \n");
        return;
    }

    while (fgets(TheLine, sizeof(TheLine), The_File_Pointer))
    { /// read each single line one by one

        char building1_Buffer[30], building2_Buffer[30]; /// to store building Names
        int distance;
        sscanf(TheLine, "%[^#]#%[^#]#%d", building1_Buffer, building2_Buffer, &distance);

        int index1 = getBuildingIndex(building1_Buffer); /// the index of the first building // Not found -1
        if (index1 == -1)
        {
            strcpy(buildings[buildingCount].Name, building1_Buffer); /// now if the building was not find then add it to the array
            index1 = buildingCount++;                                /// update the index and increment the building count
        }

        int index2 = getBuildingIndex(building2_Buffer); // same for the second building
        if (index2 == -1)
        {
            strcpy(buildings[buildingCount].Name, building2_Buffer); /// now if the building was not find then add it to the array
            index2 = buildingCount++;                                /// update the index and increment the building count
        }

        // adding edges between the buildings
        addEdge(&buildings[index1], index2, distance);
        addEdge(&buildings[index2], index1, distance);
    }
    fclose(The_File_Pointer);

    /******************Load Courses Now******************/

    The_File_Pointer = fopen("input_courses.txt", "r");
    if (The_File_Pointer == NULL)
    { // in case didn't open ..
        printf("Couldn't Open input_courses.txt!! ERROR! \n");
        return;
    }

    while (fgets(TheLine, sizeof(TheLine), The_File_Pointer))
    {                                         /// go over each single line one by one
        char *token = strtok(TheLine, "#\n"); /// tokenize the The Line using #
        strcpy(courses[courseCount].CourseName, token);

        // go over each prerequisite
        int prereqIndex = 0; // Index for Prerequisites
        while ((token = strtok(NULL, "#\n")) != NULL)
        {
            strcpy(courses[courseCount].Prerequisites[prereqIndex], token); /// now go over it's prereqIndex
            prereqIndex++;
        }
        courses[courseCount].PrereqCount = prereqIndex; // set the number of Prerequisites
        courseCount++;                                  // Increment the course count
    }
    printf("Data loaded successfully\n");
    fclose(The_File_Pointer); // Close the file after reading
}

void addEdge(Building *building, int Destination, int Weight)
{                                                 // to add an edge for the building
    Edge *newEdge = (Edge *)malloc(sizeof(Edge)); // allocate memory for every new edge
    newEdge->Destination = Destination;           // set the destination
    newEdge->Weight = Weight;                     // set the Weight of the edge
    newEdge->Next = building->Edges;              // add the edge to the front of the linked list
    building->Edges = newEdge;                    // Update the head of the linked list
}

void dijkstra(char *startBuilding, char *endBuilding)
{                                                     // implements Dijkstra's algorithm
    int startIndex = getBuildingIndex(startBuilding); // convert the start building to it's own index
    int endIndex = getBuildingIndex(endBuilding);     // convert the end building to it's own index

    if (startIndex == -1 || endIndex == -1)
    { // to check if the buildings are found or not
        if (startIndex == -1 && endIndex == -1)
            printf("ERROR! Both of the start building and end building does NOT Exist! Please try again\n");
        else if (startIndex == -1)
            printf("ERROR! The start building does NOT Exist! Please try again\n");
        else if (endIndex == -1)
            printf("ERROR! The end building does NOT Exist! Please try again\n");
        return; // exit the function in all cases
    }

    int distances[MAX_NODES], previousNodes[MAX_NODES], visitedNodes[MAX_NODES] = {0}; // initialize arrays for distances, previousNodes nodes, and visitedNodes nodes

    // Create a MinHeap and initialize distances
    MinHeap *minHeap = createMinHeap(buildingCount);

    for (int i = 0; i < buildingCount; i++)
    {
        distances[i] = INFINITY; // set the initial distances to infinity
        previousNodes[i] = -1;   // set the initial previousNodes nodes to -1
        minHeap->array[i] = (MinHeapNode *)malloc(sizeof(MinHeapNode));
        minHeap->array[i]->vertex = i;
        minHeap->array[i]->distance = distances[i];
        minHeap->position[i] = i;
    }
    distances[startIndex] = 0; // set distance to the start building to 0
    decreaseKey(minHeap, startIndex, distances[startIndex]);
    minHeap->size = buildingCount;

    while (!isEmpty(minHeap))
    {
        MinHeapNode *minHeapNode = extractMin(minHeap); // Extract the vertex with the minimum distance
        int u = minHeapNode->vertex;

        for (Edge *edge = buildings[u].Edges; edge != NULL; edge = edge->Next)
        {
            int v = edge->Destination;
            if (!visitedNodes[v] && distances[u] + edge->Weight < distances[v])
            {
                distances[v] = distances[u] + edge->Weight;
                previousNodes[v] = u;
                decreaseKey(minHeap, v, distances[v]);
            }
        }
        visitedNodes[u] = 1; // Mark the node as visited
        free(minHeapNode);
    }

    if (distances[endIndex] == INFINITY)
    {                               // Check if there is no route found
        printf("No route found\n"); // Print message if no route is found
        return;                     // Exit the function if no route is found
    }

    // Print the total distance and the route
    printf("Total distance: %d\n", distances[endIndex]);
    if (counterPrintDij)
    {
        printf("Route: ");

        int path[MAX_NODES], pathIndex = 0; // Variables to store the path
        for (int at = endIndex; at != -1; at = previousNodes[at])
        {
            path[pathIndex++] = at;
        }

        // Print the path in reverse order
        for (int i = pathIndex - 1; i > 0; i--)
        {
            printf("%s -> ", buildings[path[i]].Name);
        }
        printf("%s\n", buildings[path[0]].Name);
    }
}

int getBuildingIndex(char *Name)
{
    for (int i = 0; i < buildingCount; i++)
    {
        if (strcasecmp(buildings[i].Name, Name) == 0)
        {
            return i; // return the index if the building is found
        }
    }
    return -1; // Return -1 if the building is not found
}

int getCourseIndex(char *Name)
{
    for (int i = 0; i < courseCount; i++)
    {
        if (strcasecmp(courses[i].CourseName, Name) == 0)
        {
            return i; // Return the index if the course is found
        }
    }
    return -1; // Return -1 if the course is not found
}

// Stack functions
void initializeStack(Stack *stack)
{
    stack->top = -1;
}

int isStackEmpty(Stack *stack)
{
    return stack->top == -1;
}

void push(Stack *stack, int value)
{
    if (stack->top < MAX_NODES - 1)
    {
        stack->items[++(stack->top)] = value;
    }
    else
    {
        printf("Stack overflow\n");
    }
}

int pop(Stack *stack)
{
    if (!isStackEmpty(stack))
    {
        return stack->items[(stack->top)--];
    }
    else
    {
        printf("Stack underflow\n");
        return -1;
    }
}

void topologicalSort()
{
    Course *adjList[MAX_NODES] = {NULL}; // Adjacency list for the graph
    for (int i = 0; i < courseCount; i++)
    {
        adjList[i] = &courses[i];
    }

    int visitedNodes[MAX_NODES] = {0};
    Stack stack;
    Stack stackAc;
    initializeStack(&stack);
    initializeStack(&stackAc);

    for (int i = 0; i < courseCount; i++)
    {
        if (!visitedNodes[i])
        {
            dfs(i, visitedNodes, &stack, adjList); // Perform DFS if the node is not visited
        }
    }
    if (counterPrintTop == 0)
        printf("Sorted Successfully\n");
    if (counterPrintTop)
    {
        printf("Topological Sort: ");
        while (!isStackEmpty(&stack))
        {
            int node = pop(&stack);
            push(&stackAc, node);
        }
        int i = 1;
        while (!isStackEmpty(&stackAc))
        {
            int node = pop(&stackAc);
            printf("\n%d->%s", i++, courses[node].CourseName); // Print the sorted courses
        }
    }
}

// DFS function for topological sort
void dfs(int node, int visitedNodes[], Stack *stack, Course *adjList[MAX_NODES])
{
    visitedNodes[node] = 1; // Mark the node as visited
    for (int i = 0; i < adjList[node]->PrereqCount; i++)
    {
        int prereqIndex = getCourseIndex(adjList[node]->Prerequisites[i]);
        if (prereqIndex != -1 && !visitedNodes[prereqIndex])
        {
            dfs(prereqIndex, visitedNodes, stack, adjList); // Recursively visit adjacent nodes
        }
    }
    push(stack, node); // Push the node to the stack
}

// Function to print the shortest route between two buildings
void printShortestRoute(char *startBuilding, char *endBuilding)
{
    dijkstra(startBuilding, endBuilding); // Call Dijkstra's algorithm to find and print the shortest route
}

// Function to print sorted courses
void printSortedCourses()
{
    topologicalSort(); // Call topological sort function to sort and print the courses
}

/***************************************** "Heap Functions" *****************************************/

// Create a MinHeap
MinHeap *createMinHeap(int capacity)
{
    MinHeap *minHeap = (MinHeap *)malloc(sizeof(MinHeap));                     // Allocate memory for the MinHeap structure
    minHeap->position = (int *)malloc(capacity * sizeof(int));                 // Allocate memory for position array
    minHeap->size = 0;                                                         // Initialize the size of the heap to 0
    minHeap->capacity = capacity;                                              // Set the capacity of the heap
    minHeap->array = (MinHeapNode **)malloc(capacity * sizeof(MinHeapNode *)); // Allocate memory for the array of MinHeapNode pointers
    return minHeap;                                                            // Return the created MinHeap
}

// Swaps two nodes in the heap.
void swapMinHeapNode(MinHeapNode **a, MinHeapNode **b)
{
    MinHeapNode *t = *a; // Temporary variable to hold one of the nodes
    *a = *b;             // Swap the nodes
    *b = t;              // Complete the swap
}

// Maintains the heap property.
void minHeapify(MinHeap *minHeap, int index)
{
    int smallest = index;      // Initialize the smallest as the index
    int left = 2 * index + 1;  // Calculate the left child index
    int right = 2 * index + 2; // Calculate the right child index

    // Check if the left child exists and is smaller than the current smallest
    if (left < minHeap->size && minHeap->array[left]->distance < minHeap->array[smallest]->distance)
    {
        smallest = left;
    }

    // Check if the right child exists and is smaller than the current smallest
    if (right < minHeap->size && minHeap->array[right]->distance < minHeap->array[smallest]->distance)
    {
        smallest = right;
    }

    // If the smallest is not the index, swap and continue heapifying
    if (smallest != index)
    {
        MinHeapNode *smallestNode = minHeap->array[smallest];
        MinHeapNode *indexNode = minHeap->array[index];

        // Swap positions
        minHeap->position[smallestNode->vertex] = index;
        minHeap->position[indexNode->vertex] = smallest;

        // Swap nodes
        swapMinHeapNode(&minHeap->array[smallest], &minHeap->array[index]);

        // Recursively heapify the affected sub-tree
        minHeapify(minHeap, smallest);
    }
}

// Check if the MinHeap is empty
int isEmpty(MinHeap *minHeap)
{
    return minHeap->size == 0; // Return true if the size of the heap is 0
}

// Extract the minimum node from the heap
MinHeapNode *extractMin(MinHeap *minHeap)
{
    if (isEmpty(minHeap))
    {
        return NULL; // Return NULL if the heap is empty
    }

    MinHeapNode *root = minHeap->array[0]; // Store the root node

    MinHeapNode *lastNode = minHeap->array[minHeap->size - 1]; // Get the last node

    // Replace the root with the last node
    minHeap->array[0] = lastNode;

    // Update the position of the last node
    minHeap->position[root->vertex] = minHeap->size - 1;
    minHeap->position[lastNode->vertex] = 0;

    // Reduce the size of the heap and heapify
    --minHeap->size;
    minHeapify(minHeap, 0);

    return root; // Return the extracted root node
}

// Decrease the distance value of a given vertex
void decreaseKey(MinHeap *minHeap, int vertex, int distance)
{
    int i = minHeap->position[vertex]; // Get the index of the vertex in the heap array

    minHeap->array[i]->distance = distance; // Update the distance value

    // Move up the heap until the heap property is restored
    while (i && minHeap->array[i]->distance < minHeap->array[(i - 1) / 2]->distance)
    {
        // Swap the node with its parent
        minHeap->position[minHeap->array[i]->vertex] = (i - 1) / 2;
        minHeap->position[minHeap->array[(i - 1) / 2]->vertex] = i;
        swapMinHeapNode(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);

        // Move to the parent index
        i = (i - 1) / 2;
    }
}

// Check if a given vertex is in the MinHeap
int isInMinHeap(MinHeap *minHeap, int vertex)
{
    if (minHeap->position[vertex] < minHeap->size)
    {
        return 1; // Return true if the vertex is in the heap
    }
    return 0; // Return false otherwise
}
