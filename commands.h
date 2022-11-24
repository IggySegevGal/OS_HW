#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd> 
#include <stdio>
#include <time>
#include <stdlib>
#include <signal>
#include <string>
#include <sys/types>
#include <sys/wait>
#define MAX_LINE_SIZE 80
#define MAX_ARG 20
//typedef enum { FALSE , TRUE } bool; replace with true false
int ExeComp(char* lineSize);
int BgCmd(char* lineSize, jobs_class jobs);
int ExeCmd(jobs_class jobs, char* lineSize, char* cmdString);
void ExeExternal(char *args[MAX_ARG], char* cmdString);
#endif

