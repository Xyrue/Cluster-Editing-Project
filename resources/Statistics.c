#include "Statistics.h"
#include "valid.h"
#include "Queue.h"

/*only for now... need to delete this variables from here****************/
extern unsigned int arrSize; /*size of arrSize */
extern vertex *vertices; /*graph*/
extern int vertexNum=0;


double avgBetweenClusters=0; /*Average of all the edges that are between clusters*/
double avgInClusters=0; /*Average of all the edges that are in clusters*/

/*an array of the clusters where each cell in the array
is the cluster score of the equivalent cluster*/
double *ClusterScore;

/*an array of the clusters where each cell in the array
is the cluster diameter of the equivalent cluster*/
int *ClusterDiameter; 


void findStatistics(){
	double sumInCluster=0, sumBetweenCluster=0, 
	int countInCluster=0,countBetweenCluster=0, temp=0;
	edge TempEdge;
	ClusterScore = (double *) calloc(amountOfClusters,sizeof(double)); /*the cluster score*/
		if(ClusterScore==NULL){
		allFail(1);
	}
	ClusterDiameter= (int *) calloc(amountOfClusters,sizeof(int)); /*the cluster Diameter*/
		if(ClusterDiameter==NULL){
		allFail(1);
	}
	while(temp<(vertexNum)){ /*iterating on all the vertices*/
	findDiameter(temp);
	TempEdge = vertices[temp]->edges->head;
		while(TempEdge!=NULL){ /*iterating on all the edges bounded to this vertex*/
			/*in the following sections weight is divided by 2 because each edge is added twice*/
			if(IS_VALUE_1(TempEdge->InCluster)){
				sumInCluster+=(TempEdge->weight/2);
				
				ClusterScore[vertices[temp]->ClusterBelonging-1]+=(TempEdge->weight/2);
				countInCluster++;
			}
			else{
				sumBetweenCluster+=(TempEdge->weight/2);
				countBetweenCluster++;
			}
			TempEdge=TempEdge->next;
		}
		temp++;
	}
	avgInClusters = Calculate(sumInCluster,countInCluster);
	avgBetweenClusters = Calculate(sumBetweenCluster,countBetweenCluster);
}

double Calculate(double sum, int count){
	return (sum/count);
}

void findDiameter(int vertexID){
	int cluster = vertices[vertexID]->ClusterBelonging;
	int temp=0,ans=0;
	int *visited=(int *) calloc(vertexNum,sizeof(int));/*an array to check if
	the vertex was aleady checked or not, 0= wasn't checked, 1=checked*/
	if(visited==NULL){
		allFail(1);
	}
	ans=Bfs(vertexID,visited,cluster);
	if (ans>ClusterDiameter[cluster-1]){ /*if the diameter bfs found a bigger distance in the cluster 
									   this is the new cluster diameter*/
		ClusterDiameter[cluster-1]=ans;
	}
	free(visited);
}


int Bfs(int presentVertex,int *visited, int cluster){
	int tempSize=0,success=0;
	edge iterEdges;
	Queue *Q = CreateQueue(vertexNum); /*creates an empty queue in the size of all the vertices*/
	visited[presentVertex] = 1;
	/* Iterate through all the vertices connected to the presentVertex and perform bfs on those
	vertices if they are not visited before */
	Enqueue(Q,presentVertex);
	while(Q->size){
		presentVertex = Front(Q);
		Dequeue(Q);
		iterEdges=vertices[presentVertex]->edges->head;
		while(iterEdges!=NULL){
			if(IS_VALUE_1(iterEdges->InCluster)){
				if(visited[iterEdges->id_vertex]!=0){
					if((vertices[iterEdges->id_vertex])->ClusterBelonging==cluster){
						Enqueue(Q,iterEdges->id_vertex);
						visited[iterEdges->id_vertex]=1;
						success=1;
					}
				}
			}
		}
		tempSize=tempSize+success;
		success=0;
	}
	free(Q);
	return tempSize;
}

