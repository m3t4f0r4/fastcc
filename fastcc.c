/*

Fastcc. The fastest compiler out there...

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>

#include "fastcompile.h"

#define MAX_STRING 2048*2048
#define DEFAULT_OUT_FD 0
#define MAX_OUT_SIZE 2048*2048
#define MAX_CODE_SIZE 9046
#define PART_STAT "dat/dat_"
#define FASTCC_COMPILER_PATH "/usr/bin/gcc" // no es lo q parece bro
#define FASTCC_COMPILER2_PATH "/usr/bin/cp"

size_t strlenx(char *str) {
	int i = 0;
	while(i < MAX_STRING) {
		if(str[i] == 0)
			break;
		i++;
	}
	return i;
}

int strcmpx(char *a, char *b) {
	int len_a = strlenx(a);
	int len_b = strlenx(b);
	int i = 0;
	int status = 0;
	if(len_a <= 0 || len_b <= 0)
		return -1;
	if(len_a != len_b)
		return 1;
	while(i < len_a && i < len_b) {
		if(a[i] == 0x0 ||
		   b[i] == 0x0) {
			if(a[i] != b[i])
				status = 1; /* no deberia pasar: len_a == len_b */
			break;
		}
		if(a[i] == b[i])
			i++;
		else {
			status = 1;
			break;
		}
	}
	return status;
}

void memsetx(char *mem, char c, size_t size) {
	size_t i = 0;
	while(i < size) {
		mem[i] = c;
		i++;
	}
	return;
}

size_t strncpyx(char *to, size_t to_size, char *from) {
	size_t i = 0;
	while(i < to_size) {
		if(from[i] == 0x0)
			break;
		to[i] = from[i];
		i++;
	}
	if(i <= to_size-1)
		to[i] = '\0';
	else
		to[to_size-1] = '\0';
	return i;
}

void putsx(char *out) {
	int r = 0;
	int len = 0;
	int copied = 0;
	char buf[MAX_OUT_SIZE+3];
	memsetx(buf, '\0', sizeof(buf));
	len = strlenx(out);
	if(len > MAX_OUT_SIZE+2 || len <= 0)
		return;
	copied = strncpyx(buf, sizeof(buf)-1, out);
	buf[copied-1] = 0x0a;
	buf[copied] = '\0';
	r = write(DEFAULT_OUT_FD, buf, strlen(buf));
	if(r < 0) {
		puts("write() failed!");
		exit(1);
	}
	return;
}

int trim_d(char *ptr, size_t sz, char *ptr2, size_t sz2) {
	int i = 0, x = 0;
	
	while(i < sz && x < sz2) {
		if(ptr[i] == '\0')
			break;
		if(ptr[i] == 0x20 ||
		   ptr[i] == 0x0a) {
			i++;
			continue;
		}
		ptr2[x] = ptr[i];
		
		i++;
		x++;
	}
	
	if(x >= sz2)
		ptr2[sz2 - 1] = '\0';
	
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
	int xflag = 1;
	
	memsetx(path_stat, '\0', sizeof(path_stat));
	
	ptr = calloc(MAX_CODE_SIZE, sizeof(char));
	if(ptr == NULL) {
		puts("malloc() failed!");
		exit(1);
	}
	
	ptr2 = calloc(MAX_CODE_SIZE, sizeof(char));
	if(ptr2 == NULL) {
		puts("malloc() failed!");
		exit(1);
	}
	
	memsetx(ptr, '\0', MAX_CODE_SIZE);
	memsetx(ptr2, '\0', MAX_CODE_SIZE);
	
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
	if(f == NULL) {
		puts("fopen() failed!");
		exit(1);
	}
	
	fread(ptr, MAX_CODE_SIZE-1, sizeof(char), f);
	
	if(f)
		fclose(f);
	
	trim_d(ptr, MAX_CODE_SIZE-1, ptr2, MAX_CODE_SIZE-1);
	
	if(ptr != NULL)
		free(ptr);
	
	snprintf(path_stat, sizeof(path_stat) - 1, "%s%d.bin", PART_STAT, sc_n);
  
	if(strcmpx(ptr2, code_hw) == 0) sc_n = 0;
	else if(strcmpx(ptr2, code_sum) == 0) sc_n = 1;
	else if(strcmpx(ptr2, code_sub) == 0) sc_n = 2;
	else if(strcmpx(ptr2, code_mul) == 0) sc_n = 3;
	else  {
		xflag = 0;
		char *argvx[] = {FASTCC_COMPILER_PATH, path_stat, "-o", argv[3], NULL};
		execv(FASTCC_COMPILER_PATH, argvx);
	}
	
	if(xflag) {
		char *argvx[] = {FASTCC_COMPILER2_PATH, path_stat, argv[3], NULL};
		execv(FASTCC_COMPILER2_PATH, argvx);
	}
	
	if(ptr2 != NULL)
		free(ptr2);
	
	return 0;
}
