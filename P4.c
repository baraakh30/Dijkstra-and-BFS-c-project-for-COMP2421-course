//Name : Baraa Khanfar
// Id : 1210640
//section No : 5
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


#define MAX_CITIES 100
#define MAX_DISTANCE 2000

struct City
{
    char name[20];
    int distance;
    struct City* next;
};

struct Graph
{
    struct City* adjacency_list[MAX_CITIES];
};

struct Graph graph;
int getIndex(char* cityName)
{
    // loop through the cities in the adjacency list
    for (int i = 0; i < MAX_CITIES; i++)
    {
        // check if the current city is not NULL and its name matches the given cityName
        if (graph.adjacency_list[i] != NULL && strcmp(graph.adjacency_list[i]->name, cityName) == 0)
        {
            return i;
        }
    }
    return -1;
}

void addCity(char* city1, char* city2, int distance)
{
    // create a new city node and allocate memory
    struct City* newCity = (struct City*)malloc(sizeof(struct City));
    strncpy(newCity->name, city2, sizeof(newCity->name));
    newCity->distance = distance;
    newCity->next = NULL;

    int index = getIndex(city1);

    // if the city1 is not found in the adjacency list, add it to the list
    if (index == -1)
    {
        // find the first available slot in the adjacency list
        int i = 0;
        while (graph.adjacency_list[i] != NULL)
        {
            i++;
        }

        // create a new city node for city1 and add the newCity as its next node
        graph.adjacency_list[i] = (struct City*)malloc(sizeof(struct City));
        strncpy(graph.adjacency_list[i]->name, city1, sizeof(graph.adjacency_list[i]->name));
        graph.adjacency_list[i]->next = newCity;
    }
    else
    {
        // Find the last node of city1 in the adjacency list and add the newCity as its next node
        struct City* cityNode = graph.adjacency_list[index];
        while (cityNode->next != NULL)
        {
            cityNode = cityNode->next;
        }
        cityNode->next = newCity;
    }
}

void loadCities()
{
    FILE* file = fopen("cities.txt", "r");
    if (file == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    char city1[20], city2[20], distanceStr[20];

    // read the city1, city2, and distance from the file until the end of the file (EOF)
    while (fscanf(file, "%s\t%s\t%s", city1, city2, distanceStr) != EOF)
    {
        int distance = atoi(distanceStr);
        addCity(city1, city2, distance);
    }

    fclose(file);

    printf("Cities loaded successfully.\n");
}

void printPath(int* parent, int dest, char* path, int* totalDistance)
{
    int pathCities[MAX_CITIES];
    int pathDistances[MAX_CITIES];
    int pathLength = 0;

    int currentCity = dest;
    int previousCity = parent[dest];

    // traverse the parent array from the destination to the source
    while (previousCity != -1)
    {
        int distance = 0;
        struct City* city = graph.adjacency_list[previousCity]->next;

        // find the distance between the currentCity and previousCity
        while (city != NULL)
        {
            if (strcmp(city->name, graph.adjacency_list[currentCity]->name)== 0)
            {
                distance = city->distance;
                break;
            }
            city = city->next;
        }

        // add the currentCity and its distance to the path
        pathCities[pathLength] = currentCity;
        pathDistances[pathLength] = distance;
        pathLength++;

        *totalDistance += distance;
        currentCity = previousCity;
        previousCity = parent[currentCity];
    }

    // print the path in  order
    sprintf(path + strlen(path), "%s ", graph.adjacency_list[currentCity]->name);

    for (int i = pathLength - 1; i >= 0; i--)
    {
        sprintf(path + strlen(path), "-> %s (%dkm) ", graph.adjacency_list[pathCities[i]]->name, pathDistances[i]);
    }
}

// Parameters:
// - src: source city index
// - dest: destination city index
// - parent: array to store the parent of each city in the shortest path
// - distance: array to store the distance of each city from the source city
// Returns:
// - char*: string containing the shortest path and total distance using dijkstra
char* dijkstra(int src, int dest, int* parent, int* distance)
{
    int visited[MAX_CITIES];

    // initialize the visited, parent, and distance arrays
    for (int i = 0; i < MAX_CITIES; i++)
    {
        visited[i] = 0;
        parent[i] = -1;
        distance[i] = INT_MAX;
    }

    distance[src] = 0;

    // perform Dijkstra's algorithm
    for (int count = 0; count < MAX_CITIES - 1; count++)
    {
        int minDistance = INT_MAX;
        int u = -1;

        // find the vertex with the minimum distance
        for (int i = 0; i < MAX_CITIES; i++)
        {
            if (!visited[i] && distance[i] < minDistance)
            {
                minDistance = distance[i];
                u = i;
            }
        }

        if (u == -1)
        {
            break;  // no more vertices to visit
        }

        visited[u] = 1;

        struct City* city = graph.adjacency_list[u];
        while (city != NULL)
        {
            int v = getIndex(city->name);
            if (!visited[v] && distance[u] != INT_MAX && distance[u] + city->distance < distance[v])
            {
                distance[v] = distance[u] + city->distance;
                parent[v] = u;
            }
            city = city->next;
        }
    }

    if (distance[dest] != INT_MAX)
    {
        // allocate memory for path string
        char* path = (char*)malloc(sizeof(char) * 1000);
        path[0] = '\0';

        // generate the shortest path string
        printPath(parent, dest, path, distance);

        // allocate memory for the result string and format it
        char* result = (char*)malloc(sizeof(char) * 1000);
        sprintf(result, "Shortest path using Dijkstra: %s\nTotal distance: %d km\n", path, distance[dest]);

        // free memory for path string and return the result
        free(path);
        return result;
    }
    else
    {
        return "No path found using Dijkstra's algorithm.\n";
    }
}
// function to perform Breadth-First Search (BFS) algorithm
// Parameters:
// - src: source city index
// - dest: destination city index
// - parent: array to store the parent of each city in the shortest path
// - distance: array to store the distance of each city from the source city
// Returns:
// - char*: string containing the shortest path and total distance using BFS
char* bfs(int src, int dest, int* parent, int* distance)
{
    int visited[MAX_CITIES];

    // initialize visited, parent, and distance arrays
    for (int i = 0; i < MAX_CITIES; i++)
    {
        visited[i] = 0;
        parent[i] = -1;
        distance[i] = INT_MAX;
    }

    visited[src] = 1;
    distance[src] = 0;

    // create a queue to store the cities to visit
    struct City* queue = (struct City*)malloc(sizeof(struct City));
    strncpy(queue->name, graph.adjacency_list[src]->name, sizeof(queue->name));
    queue->next = NULL;

    struct City* rear = queue;

    // perform BFS
    while (queue != NULL)
    {
        int u = getIndex(queue->name);
        struct City* city = graph.adjacency_list[u];

        // traverse the neighbors of the current city
        while (city != NULL)
        {
            int v = getIndex(city->name);

            if (!visited[v])
            {
                visited[v] = 1;
                distance[v] = distance[u] + city->distance;
                parent[v] = u;

                // enqueue the new city to visit
                struct City* newCity = (struct City*)malloc(sizeof(struct City));
                strncpy(newCity->name, graph.adjacency_list[v]->name, sizeof(newCity->name));
                newCity->next = NULL;
                rear->next = newCity;
                rear = newCity;
            }

            city = city->next;
        }

        // dequeue the visited city
        struct City* temp = queue;
        queue = queue->next;
        free(temp);
    }

    // check if a path exists from source to destination
    if (distance[dest] != INT_MAX)
    {
        // allocate memory for path string
        char* path = (char*)malloc(sizeof(char) * 1000);
        path[0] = '\0';

        // generate the shortest path string
        printPath(parent, dest, path, distance);

        // allocate memory for the result string and format it
        char* result = (char*)malloc(sizeof(char) * 1000);
        sprintf(result, "Shortest path using BFS: %s\nTotal distance: %d km\n", path, distance[dest]);

        // free memory for path string and return the result
        free(path);
        return result;
    }
    else
    {
        return "No path found using BFS.\n";
    }
}

void DisplayMenu()
{
    printf("\033[0;32m"); // sets the color to green
    printf("=========================================================\n");
    printf("|           Welcome To Path Finding System               |\n");
    printf("=========================================================\n");
    printf("|                                                       |\n");
    printf("|            1. Load Cities.                            |\n");
    printf("|            2. Select A Source.                        |\n");
    printf("|            3. Select A Destination and find the path. |\n");
    printf("|            4. Reshow the menu                         |\n");
    printf("|            5. Save The Shortest Path And Exit          |\n");
    printf("|                                                       |\n");
    printf("=========================================================\n");

}

int main()
{


    int choice, sourceIndex = -1, destinationIndex = -1;
    int parent[MAX_CITIES], distance[MAX_CITIES];
    DisplayMenu();
    char * dijkstraPath=NULL;
    char* bfsPath=NULL;

    while (1)
    {


        printf("Enter your choice: ");


        if(scanf("%d", &choice)==0)
        {
            printf("Invalid input. Please enter a valid integer choice.\n");
            scanf("%*[^\n]");  // Clear the input buffer
            continue;
        }

        switch (choice)
        {
        case 1:
            loadCities();

            break;

        case 2:
            if (graph.adjacency_list[0] == NULL)
            {
                printf("No cities loaded. Please load cities first.\n");
            }
            else
            {
                printf("Available cities:\n");
                for (int i = 0; i < MAX_CITIES; i++)
                {
                    if (graph.adjacency_list[i] != NULL)
                    {
                        printf("%d. %s\n", i+1, graph.adjacency_list[i]->name);
                    }
                }
                printf("Enter the number of the source city: ");
                scanf("%d", &sourceIndex);
                if (sourceIndex < 1 || sourceIndex > MAX_CITIES || graph.adjacency_list[sourceIndex-1] == NULL)
                {
                    printf("Invalid source city number.\n");
                    sourceIndex = -1;
                }
                else
                {
                    printf("Source city set to %s.\n", graph.adjacency_list[sourceIndex-1]->name);
                }
            }
            break;

        case 3:
            if (sourceIndex == -1)
            {
                printf("No source city selected. Please select a source city first.\n");
            }
            else
            {
                printf("Available cities:\n");
                for (int i = 0; i < MAX_CITIES; i++)
                {
                    if (graph.adjacency_list[i] != NULL)
                    {
                        printf("%d. %s\n", i+1, graph.adjacency_list[i]->name);
                    }
                }
                printf("Enter the number of the destination city: ");
                scanf("%d", &destinationIndex);
                if (destinationIndex < 1 || destinationIndex > MAX_CITIES || graph.adjacency_list[destinationIndex-1] == NULL)
                {
                    printf("Invalid destination city number.\n");
                    destinationIndex = -1;
                }
                else
                {
                    dijkstraPath=dijkstra(sourceIndex-1, destinationIndex-1, parent, distance);
                    printf("%s",dijkstraPath);
                    bfsPath=bfs(sourceIndex-1, destinationIndex-1, parent, distance);
                    printf("%s",bfsPath);
                    free(bfsPath);
                    free(dijkstraPath);
                }
            }
            break;

        case 4:
            DisplayMenu();
            break;

        case 5:
            if (sourceIndex == -1 || destinationIndex == -1)
            {
                printf("Source or destination not set. Please select a source and destination city.\n");
            }
            else
            {
                FILE* out = fopen("shortest_distance.txt","w");
                if(out==NULL)
                    printf("Cant open the File for Writing.\n");
                else
                {

                    dijkstraPath=dijkstra(sourceIndex-1, destinationIndex-1, parent, distance);
                    bfsPath=bfs(sourceIndex-1, destinationIndex-1, parent, distance);

                    fprintf(out,"%s\n",dijkstraPath);
                    fprintf(out,"%s",bfsPath);

                    free(bfsPath);
                    free(dijkstraPath);

                    fclose(out);
                    printf("Shortest paths saved to 'shortest_distance.txt'\n");

                }
                printf("Exiting the program...\n");
                for(int i=0; i<MAX_CITIES; i++)
                {
                    if(graph.adjacency_list[i]!=NULL)
                    {
                        struct City* current=graph.adjacency_list[i]->next;
                        while(current!=NULL)
                        {
                            struct City* temp=current;
                            current=current->next;
                            free(temp);
                        }

                    }
                }
               return 0;

            }
            break;

        default:
            printf("Invalid choice. Please enter a valid option.\n");
            break;
        }
    }

    return 0;
}
