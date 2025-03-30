#include <stdio.h>

void *heap_start = ( void * )0xa000, *heap_end = (void * )0xd000; 

void _abort(void) {
}

int close(int i) {
}
;
int creat(const char *_name, int _mode) {
}

void _exit(int _code) {
}

long lseek(int a, long b, int c) {
}

int open(const char * _name, int _mode) {
}

size_t read(int n, void *p, size_t t) {
}

int unlink(const char *c) {
}

size_t write(int n, void *p, size_t t) {
}

int isatty(int fd) {
	return 1;
}

int main (int argc, char **argv) {
	printf("argc: %d\n");
	return 0;
}