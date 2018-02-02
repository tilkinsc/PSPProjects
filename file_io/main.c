// include psp stuff
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspdisplay.h>

// calloc, malloc, relloc
#include <stdlib.h>
// snprintf
#include <stdio.h>

// include commons stuff
#include "../common/callback.h"


// configure PSP stuff
#define VERS    1
#define REVS    0

PSP_MODULE_INFO("FileIO", PSP_MODULE_USER, VERS, REVS);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER); 

// make printing easier on us
#define printf pspDebugScreenPrintf


// limits.h doesn't have PATH_MAX
#define PATH_MAX 4096


#define TITLE "bin"

// #define ROOT "ms0:/PSP/GAME/" TITLE "/"
#define ROOT "umd0:/"


void crash(int error, const char* crasher, const char* message) {
	sceDisplayWaitVblankStart();
	printf("Error (%d) : %s\n", error, crasher);
	printf("%s\n", message);
	sceKernelSleepThread();
}

void test_crash() {
	sceDisplayWaitVblankStart();
	printf("No crash occured.");
	sceKernelSleepThread();
}



void check_file(SceIoStat* info, const char* path) {
	// open file description
	int status;
	if((status = sceIoGetstat(path, info)) < 0)
		crash(status, "Checking File", "File not found or no access!");
}

int open_file(const char* path, int params, int chmod) {
	// open file handle
	int uid = sceIoOpen(path, params, chmod);
	if(uid < 0)
		crash(uid, "Opening File", "File not found or no access!");
	return uid;
}

void close_file(int uid) {
	int status;
	if((status = sceIoClose(uid)) < 0)
		crash(status, "Close File", "File could not be closed!");
}

void write_file(int uid, const char* out, int size) {
	// write to file
	if(sceIoWrite(uid, out, size) != size)
		crash(uid, "Writing File", "File wrote incorrect number of bytes!");
}

void read_file(int uid, char** out, int size) {
	// allocate, read into buffer, pad with \0
	char* buffer = calloc(size+1, sizeof(char));
	if(buffer == 0)
		crash(0, "Memory Allocation", "Memory allocation failed!");
	int read = sceIoRead(uid, buffer, size);
	if(read != size)
		crash(read, "Read File", "File size read doesn't match expected!");
	buffer[size] = '\0';
	*out = buffer;
}



void do_example1() {
	const char* src = ROOT "test.txt";
	
	// Reading file (existant) src > (char*) file_data
	char* file_data;
	SceIoStat info;
	check_file(&info, src);
	
	printf("%d\n", (int)info.st_size);
	
	int uid = open_file(src, PSP_O_RDONLY, 0777);
	read_file(uid, &file_data, (int)info.st_size);
	close_file(uid);
	
	printf("Read data:\n");
	printf("%s\n", file_data);
	free(file_data);
	file_data = 0;
}

void do_example2() {
	const char* new = ROOT "test2.txt";
	const char output[] = "Hello World 123\nabc";
	
	// Writing new file output[] > (non-existant) test2.txt
	int uid = open_file(new, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_EXCL | PSP_O_TRUNC, 0777);
	write_file(uid, output, sizeof(output)-1);
	close_file(uid);
}

void do_example3() {
	const char* src = ROOT "test.txt";
	const char* out = ROOT "diff.txt";
	
	// Copy file test.txt > diff.txt
	char* file_data;
	SceIoStat info;
	check_file(&info, src);
	
	// multiple files can be open at once, but since using one variable
	// closing the file would be wise before losing the data
	int uid = open_file(src, PSP_O_RDONLY, 0777);
	read_file(uid, &file_data, (int)info.st_size);
	close_file(uid);
	
	uid = open_file(out, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	write_file(uid, file_data, (int)info.st_size);
	close_file(uid);
	
	free(file_data);
	file_data = 0;
}

void get_files() {
	
	const char* src = ROOT;
	
	// open the directory
	int uid = sceIoDopen(src);
	if(uid <= 0)
		crash(uid, "Directory Iteration", "Failure or empty directory");
	
	printf("Opened, reading directory.\n");
	
	// iterate over the directory for files, print name and size of array (always 256)
	// this means you use strlen() to get length of file name
	SceIoDirent dir;
	char temp_name[PATH_MAX]; // 4096 is linux's forced policy
	while(sceIoDread(uid, &dir) > 0) {
		printf("Read `%s` (%d) \n", dir.d_name, sizeof(dir.d_name));
		
		// format a path, print it (remember to include stdio.h)
		snprintf(temp_name, PATH_MAX, "%s%s", ROOT, dir.d_name);
		printf("@%s\n", temp_name);
	}
	
	printf("Done reading directory.\n");
	
	// clean up
	sceIoDclose(uid);
}



int main(int argc, char** argv)
{
	// basic init
	setupExitCallback();
	pspDebugScreenInit();
	
	sceDisplayWaitVblankStart();
	pspDebugScreenSetXY(0, 0);
	
	printf("Dumping args\n");
	int i;
	for (i=0; i<argc; i++)
		printf("%s\n", argv[i]);
	printf("Done.\n");
	
	printf("%s\n", ROOT);
	
	// printf("Reading file\n");
	// do_example1();
	// printf("Done.\n");
	// printf("Writing file\n");
	// do_example2();
	// printf("Done.\n");
	// printf("Copying file\n");
	// do_example3();
	// printf("Done.\n");
	
	printf("Listing ROOT files...\n");
	get_files();
	printf("Done.\n");
	
	while(!isRunning()) {
		sceDisplayWaitVblankStart();
	}

	// Sleep Thread
	sceKernelSleepThread();
	
	sceKernelExitGame();
	return 0;
}


