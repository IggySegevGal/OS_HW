/* defines: */ 
#define DATAMAX 512 /* Longest string to echo */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <climits>
#include <iostream>
#include <fstream>
#define SYS_CALL_FAIL_MSG "TTFTP_ERROR"
#define NUM_ARG 4

void error_func(char *msg) {
    perror(msg);
    exit(1);
}

/*struct sockaddr_in {
    short int sin_family; //Address family, AF_INET
    unsigned short int sin_port; // Port number
    struct in_addr sin_addr; // Internet address (IP)
    unsigned char sin_zero[8]; // for allignments
} */

int main(int argc, char *argv[]) {

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
        error_func(SYS_CALL_FAIL_MSG);
    }

    struct sockaddr_in ServAddr; /* Local address */
    memset(&ServAddr, 0, sizeof(ServAddr));
    ServAddr.sin_family = AF_INET;
    ServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    ServAddr.sin_port = htons(ServPort);

    /* bind socket to ServPort*/
    if (bind(sockfd, (struct sockaddr*) &ServAddr,sizeof(ServAddr)) < 0){
        error_func(SYS_CALL_FAIL_MSG);
    }

    /* initialize variables for while loop */
    string file_name;
    ofstream outputfile;
    short curr_opcode = 0;
    short curr_block = 0;
    bool server_busy = false;
    int timeout_counter = 0;
    int rec_bytes = 0;
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
            error_func(SYS_CALL_FAIL_MSG);
        }


        /* check timeout */
        if (server_busy && (retval_select == 0)) { /* timeout from client */

            /* update timeout counter */
            timeout_counter++;
            
            /* check timeout counter limit */
            if (timeout_counter >= max_num_of_resends) {
                /* timeout limit exceeded - send error msg to client */
				send_error(curr_client_addr, 0, "Abandoning file transmission", sockfd);
				
                /* remove file */
                if(!(remove(file_name) == 0)){
					error_func(SYS_CALL_FAIL_MSG);
				}
                
                /* reset connection */
                timeout_counter = 0;
                curr_block = 0;
                server_busy = false;
                memset(&client_addr, 0, sizeof(client_addr));
                memset(&buffer, 0, sizeof(buffer));
                
                /* reset timeout: */
                FD_ZERO(&rfds);
                FD_SET(sockfd, &rfds);
                /* Wait up to timeout seconds. */
                tv.tv_sec = timeout;
                tv.tv_usec = 0;
                file.close();
				continue;
			}

            /* reset timeout: */
            FD_ZERO(&rfds);
            FD_SET(sockfd, &rfds);
            /* Wait up to timeout seconds. */
            tv.tv_sec = timeout;
            tv.tv_usec = 0;
			continue;
        }

        /* handle packet */
        if (( retval_select > 0 && FD_ISSET(sockfd, &rfds))) {
            rec_msg_size = recvfrom(sockfd, (void*)&buffer, sizeof(buffer), 0,
                (struct sockaddr *)&new_client_addr, &addr_length);

            /* check msg size */
            if (rec_msg_size < 0) {
                error_func(SYS_CALL_FAIL_MSG);
            }
	
            /* check if new client == curr client, if not - error */ 
            if (server_busy && !sockaddr_equal(curr_client_addr,new_client_addr)) {
                send_error(new_client_addr, 4, "Unexpected packet", sockfd);
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
                    send_error(new_client_addr, 4, "Unexpected packet", sockfd);
                    continue;
                }

				/* get file_name */
				memcpy(&file_name, &buffer[2] , sizeof(buffer) - OCTET_STRING_SIZE);
				
                /* create inputfile and check if file already exists */
                ifstream inputfile;
				inputfile.open(file_name);

                if (inputfile) {
                /* file_name exists, send error */
                    send_error(new_client_addr, 6, "File already exists", sockfd);
					continue;
				}

                else{
                /* file_name does not exist */
                    outputfile.open(file_name);
                }    

                /* send ack to client */
                memcpy(&curr_client_addr, &new_client_addr, sizeof(sockaddr_in));
                send_ack(curr_client_addr, sockfd, curr_block);
                server_busy = true;
                curr_block++;
            }

            /* data request */
            else if (buffer_opcode == 3) {
				
                /* if server is not busy, send error msg 7 to client (unknown user) */
                if (!server_busy) { 
                    send_error(new_client_addr, 7, "Unknown user", sockfd);
                    continue;
                }

                /* else, client is ok: create data packet */
				struct data_pck_struct curr_data;

                /* get opcode and block number */
                memset(&curr_data, 0, sizeof(curr_data));
                memcpy(&curr_data, &buffer, rec_msg_size);
				curr_data.opcode = ntohs(curr_data.opcode);
				curr_data.block_number = ntohs(curr_data.block_number);
				
                /* check block number, if not equal send error */
				if(curr_data.block_number != curr_block) {
					send_error(curr_client_addr, 0, "Bad block number", sockfd);
					continue;
				}

                /* write to file */
                outputfile.write(curr_data.data, rec_msg_size - 2*sizeof(short));

                /* send ack to client */
                send_ack(curr_client_addr, listening_socket, curr_block);
				curr_block++;

                /* if last packet - close file and reset connection */
                if (rec_msg_size < DATA_PCK_LEN) {
                /* reset connection */
                timeout_counter = 0;
                curr_block = 0;
                server_busy = false;
                memset(&client_addr, 0, sizeof(client_addr));
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

void send_error(struct sockaddr_in client_addr, int error_code, string error_msg, int socket) {
    struct error_packet error_pckt;
    error_pckt.error_opcode = htons(ERR_OPCODE);
    error_pckt.error_code = htons(error_code);
	
    strcpy(error_pckt.error_msg, error_msg.c_str());
	//cout << "*** SENDING ERR *** " << error_msg << endl; // DEBUG
	//cout << "HOST error_pckt.error_opcode - " << ERR_OPCODE << ", error_pckt.error_code - " << error_code << endl; // DEBUG     cout << endl; // DEBUG
    if(sendto(socket, &error_pckt, sizeof(error_pckt), 0 ,(struct sockaddr *) &client_addr, sizeof(client_addr)) 
        != sizeof(error_pckt)) {
        perror("TTFTP_ERROR: send_function error");
        exit(1);
    }
}

bool send_ack(struct sockaddr_in client_addr, int socket, short block_num) {
    struct ack_packet ack_pckt;
    ack_pckt.ack_opcode = htons(ACK_OPCODE);
    ack_pckt.block_num = htons(block_num);
	//cout << "*** SENDING ACK *** " << endl; // DEBUG
	//cout << "HOST ack_pckt.ack_opcode - " << ACK_OPCODE << ", ack_pckt.block_num - " << block_num << endl; // DEBUG
	//cout << "NETWORK ack_pckt.ack_opcode - " << ack_pckt.ack_opcode << ", ack_pckt.block_num - " << ack_pckt.block_num << endl; // DEBUG
	if(sendto(socket, &ack_pckt, sizeof(struct ack_packet), 0 ,(struct sockaddr *) &client_addr, sizeof(client_addr))
        != sizeof(struct ack_packet)) {
        return false;
    }
    return true;
}

bool sockaddr_equal(struct sockaddr_in first_client_addr, struct sockaddr_in second_client_addr) {
    if (first_client_addr.sin_addr.s_addr == second_client_addr.sin_addr.s_addr
        && first_client_addr.sin_port == second_client_addr.sin_port)
        return true;
    return false;
}