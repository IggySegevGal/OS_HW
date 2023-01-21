/* defines: */ 
#define DATAMAX 512
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <climits>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include "classes.h"
#define SYS_CALL_FAIL_MSG "TTFTP_ERROR"
#define NUM_ARG 4
#define DATA_PCK_LEN 516



using namespace std;

//void error_func() {
//    perror("TTFTP_ERROR:");
//    exit(1);
//}


void send_ack_pck(struct sockaddr_in curr_client_addr , short curr_block, int sockfd){
    
    /* create packet */
    struct ack_pck pck;
    pck.block_number = htons(curr_block);
    pck.opcode = htons(OPCODE_ACK);
    
    /* send to client and check success */
    ssize_t send_num = sendto(sockfd, &pck, sizeof(struct ack_pck), 0 ,(struct sockaddr *) &curr_client_addr, sizeof(curr_client_addr));
	if(send_num != sizeof(pck)){
        return;
    }
    return;
}

void send_error_pck(struct sockaddr_in curr_client_addr, int sockfd, int error_code, string error_msg){
    
    /* create packet */
    struct error_pck pck;
    pck.error_code = htons(error_code);
    pck.opcode = htons(OPCODE_ERROR);
    strcpy(pck.msg, error_msg.c_str());
    //cout << "hey sis, you have an error " << error_msg << endl;
	
    /* send to client and check success */
    ssize_t send_num = sendto(sockfd, &pck, sizeof(pck), 0 ,(struct sockaddr *) &curr_client_addr, sizeof(curr_client_addr));
    if(send_num != sizeof(pck)){ /* syscall error, exit */
        perror("TTFTP_ERROR:");
        exit(1);
    }
    return;
}

/*struct sockaddr_in {
    short int sin_family; //Address family, AF_INET
    unsigned short int sin_port; // Port number
    struct in_addr sin_addr; // Internet address (IP)
    unsigned char sin_zero[8]; // for allignments
} */

int main(int argc, char *argv[]) {
const short OCTET_STRING_SIZE = 6;

    /* check num args */
    if (argc != NUM_ARG) {
        cout << "TTFTP_ERROR: illegal arguments" << endl;
        exit(1);
    }

    /* parse input */
    unsigned short ServPort = atoi(argv[1]);
    unsigned short timeout = atoi(argv[2]);
    unsigned short max_num_of_resends = atoi(argv[3]);

    /* check arguments */
    /* port value: */
    if (!(ServPort > 0 && ServPort <= USHRT_MAX)) {
        cout << "TTFTP_ERROR: illegal arguments" << endl;
        exit(1);
    }

    /* timeout value: */
    if (!(timeout > 0 && timeout <= USHRT_MAX)) {
        cout << "TTFTP_ERROR: illegal arguments" << endl;
        exit(1);
    }

    /* max_num_of_resends value: */
    if (!(max_num_of_resends > 0 && max_num_of_resends <= USHRT_MAX)) {
        cout << "TTFTP_ERROR: illegal arguments" << endl;
        exit(1);
    }

    /* create UDP socket*/
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0){
    	perror("TTFTP_ERROR:");
    	exit(1);
    }

    struct sockaddr_in ServAddr; /* Local address */
    memset(&ServAddr, 0, sizeof(ServAddr));
    ServAddr.sin_family = AF_INET;
    ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    ServAddr.sin_port = htons(ServPort);

    /* bind socket to ServPort*/
    if (bind(sockfd, (struct sockaddr*) &ServAddr,sizeof(ServAddr)) < 0){
    	perror("TTFTP_ERROR:");
    	exit(1);
    }

    /* initialize variables for while loop */
    char file_name[DATA_PCK_LEN - sizeof(short)]; //CHECKKKKKKKK
    ofstream outputfile;
    short curr_opcode = 0;
    short curr_block = 0;
    bool server_busy = false;
    int timeout_counter = 0;
    //int rec_bytes = 0;
    char buffer[DATA_PCK_LEN] = {0};
    /* select function variables */
    fd_set rfds;
    struct timeval tv;
    int retval_select;
    /* Watch stdin (fd sockfd) to see when it has input */
    FD_ZERO(&rfds);
    FD_SET(sockfd, &rfds);
    /* Wait up to timeout seconds. */
    tv.tv_sec = timeout;
    tv.tv_usec = 0;
    /* init clients adress */
    struct sockaddr_in curr_client_addr;
	memset(&curr_client_addr, 0, sizeof(curr_client_addr));
    struct sockaddr_in new_client_addr;
    memset(&new_client_addr, 0, sizeof(new_client_addr));
    socklen_t addr_length = sizeof(struct sockaddr_in);
	string  error_msg;

    while(1) {

        /* zero buffer */
        memset(&buffer, 0, sizeof(buffer));

        /* check if server is busy to decide which select */
        if (server_busy){ /* handeling curr client */
            retval_select = select(sockfd + 1, &rfds, NULL, NULL, &tv);
        }
        else { /* waiting for new client */
            retval_select = select(sockfd + 1, &rfds, NULL, NULL, NULL);
        }
            
        /* check select retval: */
        if (retval_select < 0) {
		perror("TTFTP_ERROR:");
		exit(1);
        }


        /* check timeout */
        if (server_busy && (retval_select == 0)) { /* timeout from client */

            /* update timeout counter */
            timeout_counter++;
            
            /* check timeout counter limit */
            if (timeout_counter >= max_num_of_resends) {
                /* timeout limit exceeded - send error msg to client */
                error_msg = "Abandoning file transmission";
                send_error_pck(curr_client_addr, sockfd, 0, error_msg);
				
                /* remove file */
                if(!(remove(file_name) == 0)){
                perror("TTFTP_ERROR:");
                exit(1);
				}
                
                /* reset connection */
                timeout_counter = 0;
                curr_block = 0;
                server_busy = false;
                memset(&curr_client_addr, 0, sizeof(curr_client_addr)); // change from client to curr_client
                memset(&buffer, 0, sizeof(buffer));
                
                /* reset timeout: */
                FD_ZERO(&rfds);
                FD_SET(sockfd, &rfds);
                /* Wait up to timeout seconds. */
                tv.tv_sec = timeout;
                tv.tv_usec = 0;
                outputfile.close(); 
				continue;
			}

		/* send ack */
           	 send_ack_pck(curr_client_addr, curr_block, sockfd);
            /* reset timeout: */
            FD_ZERO(&rfds);
            FD_SET(sockfd, &rfds);
            /* Wait up to timeout seconds. */
            tv.tv_sec = timeout;
            tv.tv_usec = 0;
			continue;
        }

        /* handle packet */
	int rec_msg_size = 0;
        if (( retval_select > 0 && FD_ISSET(sockfd, &rfds))) {
            rec_msg_size = recvfrom(sockfd, (void*)&buffer, sizeof(buffer), 0,
                (struct sockaddr *)&new_client_addr, &addr_length);

            /* check msg size */
            if (rec_msg_size < 0) {
            	perror("TTFTP_ERROR:");
    		exit(1);
            }
	
           /* check if new client == curr client, if not - error */ 
            bool is_equal = false;
            if (curr_client_addr.sin_addr.s_addr == new_client_addr.sin_addr.s_addr && curr_client_addr.sin_port == new_client_addr.sin_port){
                    is_equal = true;
                }
            if (server_busy && !is_equal) {
                error_msg = "Unexpected packet";
                send_error_pck(new_client_addr, sockfd, 4, error_msg);
				continue;
			}
			
            /* client is ok, msg is ok - get opcode from buffer */
			memcpy(&curr_opcode, &buffer, sizeof(short));
			curr_opcode = ntohs(curr_opcode);

            /* handle message: */
            /* WRQ request */
            if (curr_opcode == 2) { 
                
                /* if server is busy, send error msg 4 to client */
                if (server_busy) { 
                    error_msg = "Unexpected packet";
                    send_error_pck(new_client_addr, sockfd, 4, error_msg);
                    continue;
                }

				/* get file_name */
				memcpy(&file_name, &buffer[2] , sizeof(buffer) - OCTET_STRING_SIZE);
				
                /* create inputfile and check if file already exists */
                ifstream inputfile;
				inputfile.open(file_name);

                if (inputfile) {
                /* file_name exists, send error */
                    error_msg = "File already exists";
                    send_error_pck(new_client_addr, sockfd, 6, error_msg);
					continue;
				}

                else{
                /* file_name does not exist */
                    outputfile.open(file_name);
                }    

                /* send ack to client */
                memcpy(&curr_client_addr, &new_client_addr, sizeof(sockaddr_in));
                send_ack_pck(curr_client_addr, curr_block, sockfd);
                server_busy = true;
                curr_block++;
            }

            /* data request */
            else if (curr_opcode == 3) { //change from cuffer_opcode to curr_opcosr
				
                /* if server is not busy, send error msg 7 to client (unknown user) */
                if (!server_busy) { 
                    error_msg = "Unknown user";
                    send_error_pck(new_client_addr, sockfd, 7, error_msg);
                    continue;
                }

                /* else, client is ok: create data packet */
				struct data_pck curr_data;

                /* get opcode and block number */
                memset(&curr_data, 0, sizeof(curr_data));
                memcpy(&curr_data, &buffer, rec_msg_size);
				curr_data.opcode = ntohs(curr_data.opcode);
				curr_data.block_number = ntohs(curr_data.block_number);
				
                /* check block number, if not equal send error */
				if(curr_data.block_number != curr_block) {
                    error_msg = "Bad block number";
                    send_error_pck(curr_client_addr, sockfd, 0, error_msg);
			/* severe error - remove file */
		            if(!(remove(file_name) == 0)){
		                perror("TTFTP_ERROR:");
		                exit(1);
		            }
					continue;
				}

                /* write to file */
                outputfile.write(curr_data.data, rec_msg_size - 2*sizeof(short));

                /* send ack to client */
                send_ack_pck(curr_client_addr, curr_block, sockfd);
				curr_block++;

                /* if last packet - close file and reset connection */
                if (rec_msg_size < DATA_PCK_LEN) {
                /* reset connection */
                timeout_counter = 0;
                curr_block = 0;
                server_busy = false;
                memset(&curr_client_addr, 0, sizeof(curr_client_addr)); //change from client to curr_client
                memset(&buffer, 0, sizeof(buffer));
                /* reset timeout: */
                FD_ZERO(&rfds);
                FD_SET(sockfd, &rfds);
                /* Wait up to timeout seconds. */
                tv.tv_sec = timeout;
                tv.tv_usec = 0;

				outputfile.close();

				}
            }
        }


    }
    return 0;
} 


