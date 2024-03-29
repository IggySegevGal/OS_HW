#ifndef _CLASSES_H
#define _CLASSES_H

#include <iostream>
#include <cstring>
#include <vector>
#include <pthread.h>
#include <unistd.h>
#include <fstream>
#include <cstdlib>
#include <cmath>
using namespace std;
/* defines: */
#define DATA_PCK_LEN 516 /* Longest string to echo */
#define DATA_HEADER_LEN 4 /* Longest string to echo */
#define OPCODE_WRQ 2
#define OPCODE_DATA 3
#define OPCODE_ACK 4
#define OPCODE_ERROR 5
#define MAX_MSG 100


/* packets structs */
struct data_pck{
    short opcode;
    short block_number;
    char data[DATA_PCK_LEN - DATA_HEADER_LEN];
} __attribute__((packed));

struct ack_pck{ 
    short opcode;
    short block_number;
} __attribute__((packed));

struct error_pck{
    short opcode;
    short error_code;
    char msg[MAX_MSG];
} __attribute__((packed));




#endif
