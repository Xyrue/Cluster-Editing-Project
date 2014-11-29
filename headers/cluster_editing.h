
#ifndef CLUSTER_EDITING_H
#define CLUSTER_EDITING_H

/* You may and should alter the file */

/* Bring in the CPLEX function declarations and the C library 
   header file stdio.h with include of cplex.h. */

#include <ilcplex/cplex.h>
#include "valid.h"
/*#include <stdlib.h>*/

/* Bring in the declarations for the string functions */
/*#include <string.h>*/

/* Include declaration for functions */
void free_and_null (void **ptr);
int  cluster();

#endif