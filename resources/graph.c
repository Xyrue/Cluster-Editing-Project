#include "graph.h"


unsigned int arrSize; /*size of vertex array */
vertex *vertices; /*graph*/
int vertexNum=0; /* number of vertices*/
edge** edges;
edge** edgeMatrix;

extern int C;
extern double* rhs;
extern char* sense;
extern double* lb;
extern double* ub;
extern double* rngval;
extern double* obj;
extern int varNum;
extern int conNum;
extern int* matcnt;
extern int* matind;
extern int* matbeg;
extern double* matval;
extern char* ctype;
extern int* indices;
extern double* sol;

/*int C;
int* rhs;
char* sense;
double* lb;
double* ub;
double* rngval;
double* obj;
int varNum;
int conNum;*/


/* checks if the command contains only white spaces*/
int only_whitespaces(char * str){
	while(*str!='\0'){
		if(!isspace(*str)){
			return 0;
		}
		++str;
	}
	return 1;
}


/*This function compares two strings*/
int compareStrings(char* str1, char* str2){
	while (*str1 !='\0' ){
		if (*str1!=*str2){
			return 0;
		}
		*str1++;
		*str2++;
	}
	if (!(isspace(*str2) || '\0'==*str2)){
		return 0;
	}
	return 1;
}

/*This command is responsible for adding vertex to the graph*/
int add_vertex(char* name){
	linked_list l;
	vertex v;
	
	if (checkName(name)){
		printf("Vertex name already exists");
		return -1;
	}
	if((l=(linked_list)malloc(sizeof(struct  linked_list_s)))==NULL){
		allFail(0);
	}
	if ((v = (vertex) malloc(sizeof(struct vertex_s)))==NULL){
		allFail(0);
	}

	checkResize(); /*checks if the array is full and doubles it if needed*/
	l->head=NULL;
	l->tail=NULL;
	v->ID = vertexNum;
	v->name=(char *)malloc(strlen(name)*sizeof(char));
	if ((v->name)==NULL){
		allFail(0);
	}
	strcpy(v->name,name);
	v->ClusterBelonging=0;
	v->edges=l;
	vertices[vertexNum]=v;
	vertexNum++;
	return 0;
}

/*This function checks if the array is full and doubles it size if needed*/
int checkResize(){
	vertex* p;

	if (0==vertexNum){
		p = (vertex*)malloc(sizeof(struct vertex_s));
		if (NULL==p){
			allFail(0);
			return -1; /*Sysyem error*/
		}
		arrSize=1;
		vertices = p;
	}
	if (vertexNum==arrSize){
		arrSize*=2;
		p = (vertex*)realloc(vertices,arrSize*sizeof(struct vertex_s));
		if (NULL==p){
			allFail(2);
			return -1;
		}
		vertices=p;
	}
	return 0;
}

/*Checks if the name is unique*/
int checkName(char* name){
	int i;

	for (i=0;i<vertexNum;i++){
		if (compareStrings(name,vertices[i]->name)){
			return 1;
		}
	}
	return 0;
}

/*this function add edge between two vetices*/
int addEdge(char** param){
	int first = atoi(param[1]); /*first vertex*/
	int second = atoi(param[2]); /*second vertex*/
	/*float weight = floorf(atof(param[3]) * 1000 + 0.5) / 1000;*/
	float weight = atof(param[3]);
	if (weight<=0 || weight>1){
		printf("Error: Invalid weight\n");
		return -1;
	}if((first >= vertexNum) || (first <0)|| (second>=vertexNum) || (second<0)){
		printf("Error: ID number is not in the system\n");
	}else if(first==second){
		printf("Error: edge must be between two different vertices\n");
	}else{
		if(!checkList(vertices[first]->edges,second)){
			addToList(first,second,weight,1);
			addToList(second,first,weight,1);
		}
	}
	return 0;
}

/*This function checks if an edge is already exists.*/
int checkList(const linked_list l,const int id){
	edge e;
	if (l->head==NULL){
		return 0;
	}
	e = l->head;
	while(e!=NULL){
		if (e->id_vertex==id){
			printf("Error: edge is already in the graph\n");
			return -1;
		}
		e=e->next;
	}
	return 0;
}

/*This function adds the element with the id given to the linked list*/
int addToList(const int id1, const int id2, const int w,const int ghost){
	edge e;
	linked_list l;
	l= vertices[id1]->edges;
	if(NULL==(e = (edge)malloc(sizeof(struct element_s)))){
		allFail(0);
		return -1;
	}
	e->id_vertex=id2;
	e->weight=w;
	e->prev=NULL;
	e->next=l->head;
	e->InCluster=1;
	if (l->head==NULL){
		l->tail=e;
	}else{
		l->head->prev=e;
	}
	l->head=e;
	if (ghost==0 && id1<id2){
		edgeMatrix[id1][id2] = (vertices[id1]->edges->head); 
	}
	return 0;
}

void addGhosts(){
	int v1=0;
	int v2;
	while(v1<vertexNum){
		for (v2=v1+1;v2<vertexNum;v2++){
			if((edgeMatrix[v1][v2])==NULL){
				addToList(v1,v2,C,0);
				addToList(v2,v1,C,0);
			}
		}
		v1++;
	}
}

int probParams(){
	varNum = vertexNum*(vertexNum-1)/2;
	conNum = vertexNum*(vertexNum-1)*(vertexNum-2)/2;
	rhs = (double*) malloc(conNum*sizeof(double));
	if (rhs==NULL){
		fprintf(stderr,"0");
		allFail(0);
		return 0;
	}
	sense = (char*) malloc(conNum*sizeof(char));
	if (sense==NULL){
		fprintf(stderr,"00");
		allFail(0);
		return 0;
	}
	lb = (double*) calloc(conNum,sizeof(double));
	if (lb==NULL){
		fprintf(stderr,"10");
		allFail(1);
		return 0;
	}
	ub = (double*) malloc(conNum*sizeof(double));
	if (ub==NULL){
		allFail(0);
		fprintf(stderr,"3");
		return 0;
	}
	edges = (edge**) malloc(varNum*sizeof(edge*));
	if (edges==NULL){
		allFail(0);
		fprintf(stderr,"4");
		return 0;
	}
	obj = (double*) malloc(varNum*sizeof(double));
	if (obj==NULL){
		allFail(0);
		fprintf(stderr,"5");
		return 0;
	}
	matbeg = (int*) malloc(varNum*sizeof(int));
	if (matbeg==NULL){
		allFail(0);
		fprintf(stderr,"8");
		return 0;
	}
	matcnt = (int*) malloc(varNum*sizeof(int));
	if (matcnt==NULL){
		allFail(0);
		fprintf(stderr,"07");
		return 0;
	}
	matind = (int*) malloc(varNum*3*(vertexNum-2)*sizeof(int));
	if (matind==NULL){
		allFail(0);
		fprintf(stderr,"dsf0");
		return 0;
	}
	matval = (double*) malloc(varNum*3*(vertexNum-2)*sizeof(double));
	if (matbeg==NULL){
		allFail(0);
		fprintf(stderr,"sdd0");
		return 0;
	}

	ctype = (char*) malloc(varNum*sizeof(char));
	if (ctype==NULL){
		allFail(0);
		fprintf(stderr,"fdg0");
		return 0;
	}
	indices = (int*) malloc(varNum*sizeof(int));
	if (indices==NULL){
		allFail(0);
		fprintf(stderr,"0yugh");
		return 0;
	}
	sol = (double*) calloc(varNum,sizeof(double));
	if (sol==NULL){
		allFail(0);
		fprintf(stderr,"my70");
		return 0;
	}
	createEdgeMat();
	addGhosts();
	FillEdges();
	fillConst();
	return 0;
}

void FillEdges(){
	int i=0,j,edgesNum=0;
	for (;i<vertexNum;i++){
		for (j=i+1;j<vertexNum;j++){
			(edgeMatrix[i][j])->edges_id = edgesNum;
			edges[edgesNum]=(&edgeMatrix[i][j]);
			obj[edgesNum]=(edgeMatrix[i][j])->weight*(-1);
			ub[edgesNum]=1;
			ctype[edgesNum] = 'B';
			indices[edgesNum] = edgesNum;
			matcnt[edgesNum]=3*(vertexNum-2); /*Each edge appears in 3*(|V|-2) equations*/
			matbeg[edgesNum]=edgesNum*3*(vertexNum-2);
			edgesNum++;
		}
	}
}

/*edge* findEdge(int v1, int v2){
	edge p;
	p=vertices[v1]->edges->head;
	while(p!=NULL){
		if (p->id_vertex==v2){
			return &p;
		}
		p=p->next;
	}
	return NULL;
}*/

void createEdgeMat(){
	int i=0,j=0;
	edge *temp;
	edge *p;
	p =(edge *) calloc(vertexNum*vertexNum,sizeof(edge));
	edgeMatrix =(edge **) calloc(vertexNum,sizeof(edge *));
	for( i=0 ; i<vertexNum ; i++ ){
		edgeMatrix[i] = p+i*vertexNum;
	}
	for(j=0;j<vertexNum;j++){
		temp=&(vertices[j]->edges->head);
		while((*temp)!=NULL){

			if (j<(*temp)->id_vertex){
				edgeMatrix[j][(*temp)->id_vertex]=*temp;
			}
			temp=&((*temp)->next);
		}
	}
}

void fillConst(){
	int* VarArr;
	int i=0,j,k,conIndex=0;
	int e1,e2,e3;
	VarArr = (int*) calloc(varNum,sizeof(int));
	for(;i<vertexNum-2;i++){
		for(j=i+1;j<vertexNum-1;j++){
			for(k=j+1;k<vertexNum;k++){
				e1 = (edgeMatrix[i][j])->edges_id;
				e2 = (edgeMatrix[i][k])->edges_id;
				e3 = (edgeMatrix[j][k])->edges_id;
				matind[e1*3*(vertexNum-2)+VarArr[e1]] =conIndex;
				matind[e2*3*(vertexNum-2)+VarArr[e2]] =conIndex;
				matind[e3*3*(vertexNum-2)+VarArr[e3]] =conIndex;
				matval[e1*3*(vertexNum-2)+VarArr[e1]] =1;
				matval[e2*3*(vertexNum-2)+VarArr[e2]] =1;
				matval[e3*3*(vertexNum-2)+VarArr[e3]] =-1;
				rhs[conIndex]=1;
				sense[conIndex] = 'L';
				conIndex++;
				VarArr[e1]++;
				VarArr[e2]++;
				VarArr[e3]++;
				matind[e1*3*(vertexNum-2)+VarArr[e1]] =conIndex;
				matind[e2*3*(vertexNum-2)+VarArr[e2]] =conIndex;
				matind[e3*3*(vertexNum-2)+VarArr[e3]] =conIndex;
				matval[e1*3*(vertexNum-2)+VarArr[e1]] =1;
				matval[e2*3*(vertexNum-2)+VarArr[e2]] =-1;
				matval[e3*3*(vertexNum-2)+VarArr[e3]] =1;
				rhs[conIndex]=1;
				sense[conIndex] = 'L';
				conIndex++;
				VarArr[e1]++;
				VarArr[e2]++;
				VarArr[e3]++;
				matind[e1*3*(vertexNum-2)+VarArr[e1]] =conIndex;
				matind[e2*3*(vertexNum-2)+VarArr[e2]] =conIndex;
				matind[e3*3*(vertexNum-2)+VarArr[e3]] =conIndex;
				matval[e1*3*(vertexNum-2)+VarArr[e1]] =-1;
				matval[e2*3*(vertexNum-2)+VarArr[e2]] =1;
				matval[e3*3*(vertexNum-2)+VarArr[e3]] =1;
				rhs[conIndex]=1;
				sense[conIndex] = 'L';
				conIndex++;
				VarArr[e1]++;
				VarArr[e2]++;
				VarArr[e3]++;
			}
		}
	}
}
