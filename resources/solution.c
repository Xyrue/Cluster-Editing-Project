#include "solution.h"


extern double* sol;
extern int varNum;
extern int vertexNum;
extern vertex *vertices;
extern edge *edges;
extern char * outputPath;
extern double objval;
extern float objcon;
extern float C;
extern edge** edgeMatrix;
extern int realEdgesNum;
int clusterNum=0;
clus * clusters;

float avgBetweenClusters=0; /*Average of all the edges that are between clusters*/
float avgInClusters=0; /*Average of all the edges that are in clusters*/

/*an array of the clusters where each cell in the array
is the cluster score of the equivalent cluster*/
/*float *ClusterScore;*/

/*an array of the clusters where each cell in the array
is the cluster diameter of the equivalent cluster*/
/*int *ClusterDiameter; */

void updateGraph(){
	int iter;
	for(iter=0;iter<varNum-1;iter++){
		if(IS_VALUE_1(sol[iter])){
			if(edges[iter]->inCluster==0){
				(edges[iter]->inCluster=2); /*edge wasn't in original graph and was added*/
			}
		}else{
			if((edges[iter])->inCluster==1){
				((edges[iter])->inCluster=3); /* edge was in orignial graph and is removed in solution*/
				edgeMatrix[edges[iter]->id_vertex][edges[iter]->vertexBelong]->inCluster=3;
			}
		}
	}
	return;
}

int findClusters(){
	int iter;
	clus *tmp, newCluster;
	edge tempEdge;
	clusterList cl;
	element e;
	int *checked = (int *)calloc(vertexNum,sizeof(int));
	clusters=(clus *)malloc(sizeof(clus)*vertexNum); /*we will realoc it later to fit the cluster number*/
	if (clusters==NULL){
		allFail(0);
		if (checked!=NULL){
			free(checked);
		}
		return -1;
	}
	if (checked==NULL){
		allFail(1);
		return -1;
	}
	for(iter=0;iter<vertexNum;iter++){
		if(checked[iter]==0){
			
			cl = (clusterList) malloc(sizeof(struct s_clusterList));
			if (cl==NULL){
				allFail(0);
				free(checked);
				return -1;
			}
			newCluster=(clus)malloc(sizeof(struct s_clust));
			clusters[clusterNum]=newCluster;
			clusters[clusterNum]->clustVertices = cl;
			clusters[clusterNum]->size=1;
			clusters[clusterNum]->score=0;
			clusters[clusterNum]->diameter=0;
			clusterNum++;
			
			tempEdge=vertices[iter]->edges->head;
			e = (element) malloc(sizeof(struct s_element));
			if (e==NULL){
				allFail(0);
				free(checked);
				return -1;
			}
			e->vertex = iter; /*Adding the first vertex in the cluster to the Cluster list*/
			cl->headl = e;
			e->next=NULL;
			checked[tempEdge->vertexBelong]=1;
			while(tempEdge!=NULL){
				if((tempEdge->inCluster==1) || (tempEdge->inCluster==2)){
					if (checked[tempEdge->id_vertex]==0){
						checked[tempEdge->id_vertex]=1;
						e = (element) malloc(sizeof(struct s_element));
						if (e==NULL){
							perror("Problem in function: 'findClusters'\n");
							allFail(0);
							free(checked);
							return -1;
						}
						e->vertex = tempEdge->id_vertex;
						e->next = cl->headl;
						cl->headl = e;
						clusters[clusterNum-1]->size++;
					}
				}
				tempEdge=tempEdge->next;
			}
		}
	}
	tmp=(clus*)realloc(clusters,sizeof(clus)*clusterNum);
	if(tmp==NULL){
		free(clusters);
		free(checked);
		return -1;
	}
	clusters=tmp;
	setClusters();
	free(checked);
	return 0;
}

/*Sorts the cluster array by size using insertion sort*/
void sort(){
	int i,j;
	clus x;
	for(i=1;i<clusterNum;i++){
		j=i;
		x=clusters[i];
		while ((j>0) && (((x->size)>(clusters[j-1]->size))|| ((x->size)=(clusters[j-1]->size)) && ((x->score)>(clusters[j-1]->score)))){
			if (((x->size)>(clusters[j-1]->size)) ||
				(((x->size)==(clusters[j-1]->size)) && ((x->score)>(clusters[j-1]->score)))){
			clusters[j] = clusters[j-1];
			}
			j--;
		}
		clusters[j]=x;
	}
}
  

void setClusters(){
	int temp=0;
	element e;
	while(temp<clusterNum){
		e=clusters[temp]->clustVertices->headl;
		while(e!=NULL){
			vertices[e->vertex]->ClusterBelonging=temp+1;
			e=e->next;
		}
		temp++;
	}
}



void findStatistics(){
	float sumInCluster=0, sumBetweenCluster=0;
	int countInCluster=0, countBetweenCluster=0, temp=0;
	edge TempEdge;
	while(temp<(vertexNum)){ /*iterating on all the vertices*/
	findDiameter(temp);
	TempEdge = vertices[temp]->edges->head;
		while(TempEdge!=NULL){ /*iterating on all the edges bounded to this vertex*/
			/*in the following sections weight is divided by 2 because each edge is added twice*/
			if((TempEdge->inCluster==1)){
				sumInCluster+=(TempEdge->weight);
				(clusters[vertices[temp]->ClusterBelonging-1]->score)+=(TempEdge->weight/2);
				countInCluster++;
			}
			else if((TempEdge->inCluster==3)){
				sumBetweenCluster+=(TempEdge->weight);
				countBetweenCluster++;
			}
			TempEdge=TempEdge->next;
		}
		temp++;
	}
	if(countInCluster!=0){
		avgInClusters = Calculate(sumInCluster,countInCluster);
	}
	if(countBetweenCluster!=0){
		avgBetweenClusters = Calculate(sumBetweenCluster,countBetweenCluster);
	}
	sort();
	setClusters();
}

float Calculate(float sum, int count){
	return (sum/count);
}

void findDiameter(int vertexID){
	int vertexCluster = vertices[vertexID]->ClusterBelonging;
	int temp=0,ans=0;
	int *visited=(int *) calloc(vertexNum,sizeof(int));/*an array to check if
	the vertex was aleady checked or not, 0= wasn't checked, 1=checked*/
	if(clusters[vertexCluster-1]->diameter==(-1)){
		return;
	}
	if(visited==NULL){
		perror("Problem in function 'findDiameter':\n");
		allFail(1);
	}
	ans=Bfs(vertexID,visited,vertexCluster);
	if (ans>clusters[vertexCluster-1]->diameter || ans==-1){ /*if the diameter bfs found a bigger distance in the cluster 
									   this is the new cluster diameter*/
		(clusters[vertexCluster-1]->diameter)=ans;
	}
	free(visited);
}


int Bfs(int presentVertex,int *visited, int cluster){
	int tempSize=0,success=0;
	int flag; /*Flag in case the diameter is infinite*/
	edge iterEdges;
	Queue *Q = CreateQueue(vertexNum); /*creates an empty queue in the size of all the vertices*/
	visited[presentVertex] = 1;
	/* Iterate through all the vertices connected to the presentVertex and perform bfs on those
	vertices if they are not visited before */
	Enqueue(Q,presentVertex);
	while(Q->size){
		flag=0;
		presentVertex = Front(Q);
		Dequeue(Q);
		iterEdges=vertices[presentVertex]->edges->head;
		while(iterEdges!=NULL){
			if(iterEdges->inCluster==1 ||iterEdges->inCluster==3){
				flag=1;
				if(visited[iterEdges->id_vertex]==0){
					if((vertices[iterEdges->id_vertex])->ClusterBelonging==cluster){
						Enqueue(Q,iterEdges->id_vertex);
						visited[iterEdges->id_vertex]=1;
						success=1;
					}
				}
			}
			iterEdges=iterEdges->next;
		}
		tempSize=tempSize+success;
		success=0;
	}
	free(Q);
	if (0==flag){
		tempSize=-1; /*-1 means "infinite"*/
	}
	return tempSize;
}


Queue * CreateQueue(int maxElements){
        /* Create a Queue */
        Queue *Q;
        Q = (Queue *)malloc(sizeof(Queue));
		if(Q==NULL){
			allFail(0);
		}
        /* Initialise its properties */
        Q->elements = (int *)malloc(sizeof(int)*maxElements);
			if((Q->elements)==NULL){
			allFail(0);
		}
        Q->size = 0;
        Q->capacity = maxElements;
        Q->front = 0;
        Q->rear = -1;
        /* Return the pointer */
        return Q;
}

void Dequeue(Queue *Q){
        /* If Queue size is zero then it is empty. So we cannot pop */
		if(Q->size==0){
			printf("Queue is Empty\n"); /*need to check what to do if queue is empty*/
            return;
        }
        /* Removing an element is equivalent to incrementing index of front by one *************************/
        else{
			Q->size--;
            Q->front++;
            /* As we fill elements in circular fashion */
            if(Q->front==Q->capacity){
				Q->front=0;
			}
        }
        return;
}


int Front(Queue *Q){
        if(Q->size==0){
			printf("Queue is Empty\n");/*need to check what to do if queue is empty**********************/
            exit(0);
        }
        /* Return the element which is at the front*/
        return Q->elements[Q->front];
}

void Enqueue(Queue *Q,int element){
        /* If the Queue is full, we cannot push an element into it as there is no space for it.*/
        if(Q->size == Q->capacity){
			printf("Queue is Full\n");/*need to check what to do if queue is full**********************/
        }
        else{
			Q->size++;
            Q->rear = Q->rear + 1;
            /* As we fill the queue in circular fashion */
            if(Q->rear == Q->capacity){
				Q->rear = 0;
            }
		    /* Insert the element in its rear side */ 
            Q->elements[Q->rear] = element;
        }
        return;
}

int createResult(){
	char fileName[8] = "results";
	char* path;
	int i=0;
	edge tempE;
	FILE *ofp = NULL;
	path = (char*) malloc(sizeof(char)*(strlen(outputPath)+8));
	strcpy(path,outputPath);
	strcat(path,fileName);
	ofp = fopen(path,"w");
	if (NULL==ofp){
		perror("Problem creating output file");
		fclose(ofp);
		free(path);
		return -1;
	}
	fprintf(ofp,"Cluster Editing Optimal Solution\n");
	if (objcon>-0.0001 && objcon<=0){
		objcon=abs(objcon);
	}
	fprintf(ofp,"score: %4.3f\n",objcon);
	fprintf(ofp,"number of clusters: %d\n",clusterNum);
	fprintf(ofp,"cost for ghost: %.3f\n\n",C);
	fprintf(ofp,"The clustered network:\n");
	fprintf(ofp,"%d vertices:\n",vertexNum);
	for (i=0;i<vertexNum;i++){
		fprintf(ofp,"%s %d\n", vertices[i]->name,vertices[i]->ClusterBelonging);
	}
	if (realEdgesNum!=0){
		fprintf(ofp,"%d edges:\n",(realEdgesNum/2));  
		for (i=0;i<vertexNum;i++){
			tempE=vertices[i]->edges->head;
			while (tempE!=NULL ){
				if (tempE->inCluster==1 || tempE->inCluster==3){
					if(tempE->id_vertex>i){
						fprintf(ofp,"%s-%s %.3f\n",vertices[tempE->vertexBelong]->name,vertices[tempE->id_vertex]->name,tempE->weight);
					}
				}
				tempE=tempE->next;
			}
		}
	}
	fprintf(ofp,"\nClustering statistics for the %d clusters:\n",clusterNum);
	fprintf(ofp, "Average weight of an edge within clusters: %.3f\n",avgInClusters);
	fprintf(ofp, "Average weight of an edge between clusters: %.3f\n",avgBetweenClusters);
	for (i=0;i<clusterNum;i++){
		fprintf(ofp, "Cluster %d score: %.3f ",i+1,(clusters[i])->score);
		if (clusters[i]->diameter==-1){
			fprintf(ofp,"diameter: inf\n");
		}else{
			fprintf(ofp,"diameter: %d\n",clusters[i]->diameter);
		}
	}
	fclose(ofp);

}

void freeSol(){
	int i;
	element p,q;
	if (clusters!=NULL){
		for (i=0;i<clusterNum;i++){
			if (clusters[i]!=NULL){
				p = clusters[i]->clustVertices->headl;
				while (p!=NULL){
					q = p->next;
					free(p);
					p=q;
				}
				free(clusters[i]->clustVertices);
			}
		}
		free(clusters);
	}
	return;
}
