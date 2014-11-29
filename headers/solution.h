#include "cluster_editing.h"

struct s_element{
	int vertex;
	struct s_element * next;
};

typedef struct s_element * element;

struct s_clusterList{
	element headl;
};

typedef struct s_clusterList * clusterList;

struct s_clust{
	clusterList clustVertices;
	int size;
	float score;
	int diameter;
};

typedef struct s_clust *clus;


/*Queue is an integer queue that has five properties.
capacity stands for the maximum number of elements Queue can hold.
Size stands for the current size of the Queue
elements is the array of elements.
front is the index of first element (the index at which we remove the element)
rear is the index of last element (the index at which we insert the element) */
typedef struct Queue{
        int capacity;
        int size;
        int front;
        int rear;
        int *elements;
}Queue;


/* crateQueue function takes argument the maximum number of elements the Queue can hold, creates
   a Queue according to it and returns a pointer to the Queue. */
Queue * CreateQueue(int);

/*dequeue removes the front element from queue*/
void Dequeue(Queue *);

/*front return the int storted in the the front of the queue*/
int Front(Queue *Q);

/*insert an int to the queue*/
void Enqueue(Queue *,int);

/*This function is the main function to find the required statistics*/
void findStatistics();

/*This is an inner function that get
Param1: Sum of weights for edges
Param2: amount of edges:
return: the average weight*/
float Calculate(float,int);

/*This function finds the diameter for each verter in his cluster
param1: the vertex to find the diameter of his cluster*/
void findDiameter(int);

/*This function return the farthest amount of edges need for a spesific vertex.
Param1:The vertex to start the BFS from
param2: an array of int's that represent if the equivalent id-vertex was checked already or not
param3: the cluster to check.
*/
int Bfs(int,int *, int);



void sort(); /*Uses merge sort to merge the array of clusters*/
void updateGraph(); /*Updates the edges of the graph with the new edges*/
int findClusters(); /*Finds clusters in the new graph*/
void setClusters(); /*Sets each vertex to be part of its respective cluster*/
int createResult();

void freeSol();