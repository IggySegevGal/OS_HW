#ifndef _SIGS_H
#define _SIGS_H
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h> 
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <signal.h>
using namespace std;

void ctrl_c(int sig_num, int fg_pid);
void ctrl_z(int sig_num, int fg_pid);

#endif

