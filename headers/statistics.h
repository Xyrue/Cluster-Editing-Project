#include "proj.h"

#define IS_VALUE_1(x) ((1-x)<0.00001)


/*This function is the main function to find the required statistics*/
void findStatistics();

/*This is an inner function that get
Param1: Sum of weights for edges
Param2: amount of edges:
return: the average weight*/
double Calculate(double,int);

/*This function finds the diameter for each verter in his cluster
param1: the vertex to find the diameter of his cluster*/
void findDiameter(int);

/*This function return the farthest amount of edges need for a spesific vertex.
Param1:The vertex to start the BFS from
param2: an array of int's that represent if the equivalent id-vertex was checked already or not
param3: the cluster to check.
*/
int Bfs(int,int *, int);



