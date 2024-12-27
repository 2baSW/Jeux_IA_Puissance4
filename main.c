#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>
#include<limits.h>
#include <assert.h>

#ifdef _WIN32
#include <stdlib.h>
#else
#include <unistd.h>
#endif

#include"Puissance4.h"



int main(int argc, char** argv){

    lancerPartie();

    return EXIT_SUCCESS;
}

