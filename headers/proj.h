#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#include<math.h>
#include<float.h>
#include<stdarg.h>

/********************************************************************************************/
/*Things to finish:
- Make sure that rounding is done correctly
- Add a function to free memory properly
- Update validate (Does not need to receive C from user)
*/
/********************************************************************************************/

struct element_s {
	int unsigned edges_id;
	int unsigned id_vertex;
	float weight;
	double InCluster; /* 1 = edge is in a cluster, 0= edge is between clusters*/   
	struct element_s   *next;
	struct element_s   *prev;
};

typedef   struct element_s *  edge;

struct  linked_list_s{
	edge head;
	edge tail;
};

typedef   struct linked_list_s *  linked_list;

typedef linked_list queue;


struct vertex_s {
   int unsigned ID;
   char *name;
   int ClusterBelonging;
   linked_list edges;
};

typedef struct  vertex_s * vertex;


int probParams();
void allFail(int func);
/*int checkLastChar(char* path);*/
