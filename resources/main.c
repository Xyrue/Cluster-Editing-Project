#include "valid.h"

extern float C;

int main (int argc, char* argv[]){
	if (argc!=4){
		return -1;
	}
	validate(argv[1],argv[2],atoi(argv[3]));
	C = atof(argv[3]);
	cluster();
	return 0;
}