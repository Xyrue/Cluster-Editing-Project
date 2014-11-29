#include "valid.h"

char* fileName = "network";
char * commands[num_commands] = {"add_vertex","add_edge","quit"}; /*an array of the commands (numbered by their place*/
extern char* outputPath;

int validate(char* input, char* output, int C){
	char* path = (char*) malloc(strlen(input)+(strlen(fileName)+1));
	char lpn[23] = "clustering_solution.lp";
	
	FILE *ifp = NULL;
	char *p;
	char * command = (char *) malloc(500);
	int flag=1;

	outputPath = (char*) malloc(strlen(output)+(strlen(lpn)+1));
	strcpy(outputPath,output);
	strcat(outputPath,lpn);
	if (C<0){
		printf("C must be non negative");
		return -1;
	}
	strcpy(path,input);
	strcat(path,fileName);
	ifp = fopen(path,"r");
	if (NULL==ifp){
		perror("Problem opening file");
		fclose(ifp);
		free(path);
		return -1;
	}
	while (flag){
		p = readLine(ifp);
		if (NULL==p){
			flag = 0;
		}else{
			strcpy(command,p);
			execute(command);	
		}
		free(command);
	}
	fclose(ifp);
	free(path);
	return 0;
}

/*int checkLastChar(const char* path){
	return 0;
}*/

char* readLine(FILE* input){
	char* p, * line;
	char c;
	int i=0;
		
	line = (char*) malloc (500);
	p = line;
	while((c=fgetc(input))!='\n' && c!=EOF && i<501){
		i++;
		*p++=c;
	/*	*p++;*/
	}
	if (i>500){
		printf("Command too long");
		return NULL;
	}
	if (EOF==c){
		return NULL;
	}else{
		*p = '\0';
		return line;
	}
}

int execute(char* command){
	int commandNum;
	char** param;

	commandNum = checkCommand(command);
	if (commandNum==0){ /*Unknown command was entered*/
		printf("Error: command not found\n");
		return -1;
	}
	param=getParam(command);
	if(commandNum==1){ /*add vertex*/
		add_vertex(param[1]);
		free(param);
		return 0;
	}
	if(commandNum==2){ /*add edge*/
		addEdge(param);
		free(param);
		return 0;
	}
	return 0;
}

/*This function checks if 1 of the allocs have failed ( 0 - malloc, 1 - calloc , 2 - realloc)*/
void allFail(int func){
	if (0==func){
		perror("Error: standard function malloc has failed");
	}
	if (1==func){
		perror("Error: standard function calloc has failed");
	}
	if (2==func){
		perror("Error: standard function realloc has failed");
	}
}

/*This function retrives the parameter from the command line and return an array of those parameters*/
char** getParam(char* str){
	char ** res  = NULL;
	char *  p    = strtok (str, " ");
	int n_spaces = 0;

	while (p) {
		if (NULL==(res = (char**) realloc (res, sizeof (char*) * ++n_spaces))){
			allFail(2);
		}
		if (res == NULL)
			exit (-1); /* memory allocation failed */
		res[n_spaces-1] = p;
		p = strtok (NULL, " ");
	}
	if(NULL==(res = (char**)realloc (res, sizeof (char*) * (n_spaces+1)))){
		allFail(2);
	}
	res[n_spaces] = 0;
	return res;
}

/*This function returns the command num (1-3)*/
int checkCommand(char * str){
	int i = 0;
	for (;i<num_commands;i++){
		if(compareStrings(commands[i],str)){
			return i+1;
		}
	}
	return 0;
}


