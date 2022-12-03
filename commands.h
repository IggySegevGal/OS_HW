#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h> 
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string>
#include <sys/types.h>
#include <sys/wait.h>
#include "classes.h"
#include<iostream>
#include<fstream>

#define MAX_LINE_SIZE 80
#define MAX_ARG 20
using namespace std;
//typedef enum { FALSE , TRUE } bool; replace with true false
int ExeComp(char* lineSize);
int BgCmd(char* lineSize, jobs_class jobs);
int ExeCmd(jobs_class &jobs, char* lineSize, char* cmdString, int &foreground_pid);
int ExeExternal(jobs_class &jobs,char *args[MAX_ARG], char* cmdString, int num_arg, int &foreground_pid);
#endif

