#include "proj.h"

#define num_commands 3 /* # of commands */

int checkName(char* name);
/*edge* findEdge(int v1, int v2);*/
int only_whitespaces(char * str);
int compareStrings(char* str1, char* str2);
int add_vertex(char* name);
int checkResize();
int addEdge(char** param);
int checkList(const linked_list l,const int id);
int addToList(const int, const int id, const int w, const int ghost);
void addGhosts();
void FillEdges();
void createEdgeMat();
void fillConst();


