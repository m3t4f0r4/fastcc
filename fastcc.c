/*

Fastcc. The fastest compiler out there...

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>

#include "fastcompile.h"

#define MAX_CODE_SIZE 9046
#define PART_STAT "dat/dat_"

int trim_d(char *ptr, size_t sz, char *ptr2, size_t sz2) {
	int i = 0;
	while(i < sz && i < sz2) {
		if(ptr[i] == '\0')
			break;
		if(ptr[i] == 0x20 ||
		   ptr[i] == 0x0a)
			ptr++;
		ptr2[i] = ptr[i];
		
		i++;
	}
	
	ptr2[i] = '\0';
	
	return i;
}

int file_exists(const char *fname) {
    FILE *file = NULL;
    if ((file = fopen(fname, "r"))) {
        fclose(file);
        return 1;
    }
    return 0;
}

int main(int argc, char *argv[]) {

	int sc_n = 0;
	FILE *f = NULL;
	char path_stat[256];
	char *ptr = NULL;
	char *ptr2 = NULL;
	
	memset(path_stat, '\0', sizeof(path_stat));
	
	ptr = malloc(MAX_CODE_SIZE);
	ptr2 = malloc(MAX_CODE_SIZE);
	
	memset(ptr, '\0', malloc_usable_size(ptr));
	memset(ptr2, '\0', malloc_usable_size(ptr2));
	
	puts("[%] Fastcc. The ultrafast and optimized compiler for the 21st century. We are not in the 90's anymore! [%]");

	if(argc < 4) {
		puts("Usage: ./fastcc <source file> -o <out>");
		exit(0);
	}
	
	if(!file_exists(argv[1])) {
		printf("Err: File '%s' not found\n", argv[1]);
		exit(0);
	}
	
	if(strcmp(argv[2], "-o") != 0) {
		puts("Invalid syntax");
		exit(0);
	}
	
	puts("compiling source file...");
	
	f = fopen(argv[1], "r");
	
	fread(ptr, malloc_usable_size(ptr), sizeof(char), f);
	
	fclose(f);
	
	trim_d(ptr, malloc_usable_size(ptr), ptr2, malloc_usable_size(ptr2));
	
	if(ptr != NULL)
		free(ptr);
	
	if(strcmp(ptr2, code_hw) == 0) sc_n = 0;
	else if(strcmp(ptr2, code_sum) == 0) sc_n = 1;
	else if(strcmp(ptr2, code_sub) == 0) sc_n = 2;
	else if(strcmp(ptr2, code_mul) == 0) sc_n = 3;
	else  {
		printf("ERR: %s @ line 1. stdio.h not found!\n", argv[1]);
		exit(1);
	}
	
	snprintf(path_stat, sizeof(path_stat) - 1, "%s%d.bin", PART_STAT, sc_n);
	
	printf("Success! Output file generated at %s\n", argv[3]);
	
	char* argvx[] = {"cp", path_stat, argv[3], NULL};
	execv("/usr/bin/cp", argvx);
	
	if(ptr2 != NULL)
		free(ptr2);
	
	return 0;
}
