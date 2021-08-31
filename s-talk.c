#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include"list.h"
#include<unistd.h>

#define h_addr h_addr_list[0]

static int sock;
static int fromlen;

typedef struct inputs{
    char* first;
    char* second;
    char* third;
}inputs;


static List *receiverList, *senderList;

static pthread_mutex_t sender = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t receiver = PTHREAD_MUTEX_INITIALIZER;

static pthread_cond_t senderCondition = PTHREAD_COND_INITIALIZER;
static pthread_cond_t receiverCondition = PTHREAD_COND_INITIALIZER;

char* dynamicAllctdMsgRecv;

char* dynamicAllctdMsgSnd;

static pthread_t threadPID1; //receiver thread
static pthread_t threadPID2; // input thread
static pthread_t threadPID3; //output thread
static pthread_t threadPID4; //sender thread

void* inputThread(void * args){

    while(1){
    
        dynamicAllctdMsgSnd = (char*)malloc(sizeof(char)*1024);
        fgets(dynamicAllctdMsgSnd,1024,stdin);

        pthread_mutex_lock(&sender);
           
            List_prepend(senderList,dynamicAllctdMsgSnd);
            
        pthread_mutex_unlock(&sender);
        pthread_cond_signal(&senderCondition);


    }
}

void* senderThread(struct inputs* values){
   //takes the item off the list and sends it across the socket 
    

    int clientLength, n;
    struct sockaddr_in client;
    struct hostent *hp;
    
    if(sock < 0){
        error("socket");
    }
  
    client.sin_family = AF_INET;
    hp = gethostbyname(values->second);

    if(hp==0){
        error("unknown host");
    } 

    bcopy((char*)hp->h_addr,(char*)&client.sin_addr,hp->h_length);
    client.sin_port = htons(atoi(values->third));
    clientLength = sizeof(struct sockaddr_in);
    char* val;

    while(1){
        
        pthread_mutex_lock(&sender);
        
            if(List_count(senderList) == 0){
                pthread_cond_wait(&senderCondition,&sender);
            }

            val = (char*)List_remove(senderList);

            n = sendto(sock,val,strlen(val),0,&client,clientLength);
           
        pthread_mutex_unlock(&sender);
	    //free(dynamicAllctdMsgSnd);

        if(strcmp(val,"!\n") == 0)
            {
                printf("Sender side ended\n");
                pthread_cancel(threadPID1);
                pthread_cancel(threadPID2);
                pthread_cancel(threadPID3);
                pthread_cancel(threadPID4);
                exit(0);
            }
      
        } 
    
    }


//using the socket code now
void* receiverThread(struct inputs *values1){


    int serverLength,n;


    //setting up for the socket
    struct sockaddr_in server; //socket for networking, hold info about who i wanna talk to
    struct sockaddr_in from;
    
    serverLength = sizeof(server);
    memset(&server,0,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY); //we are willing to work on any local port
    
    server.sin_port = htons(atoi(values1->first));
    fromlen = sizeof(struct sockaddr_in);

    // sock = socket(PF_INET,SOCK_DGRAM,0); // created socket    
    if(sock < 0){
        error("opening socket");
    }

    if(bind(sock,(struct sockaddr *)&server,serverLength)<0){ //binding to the socket
    
        error("binding");
    
    }

    while(1)
    
    {
        dynamicAllctdMsgRecv = (char*)malloc(sizeof(char)*1024);

        n = recvfrom(sock,dynamicAllctdMsgRecv,1024,0,(struct sockaddr*)&from,&fromlen);
     
        if(n<0){
            error("recvfrom");
        }
        
        pthread_mutex_lock(&receiver);
        {
            List_prepend(receiverList,dynamicAllctdMsgRecv);
        }
        pthread_mutex_unlock(&receiver);        
        pthread_cond_signal(&receiverCondition);  

        
        if(strcmp(dynamicAllctdMsgRecv,"!\n") == 0)
            {
            
                printf("Receiver side ended\n");
                pthread_cancel(threadPID1);
                pthread_cancel(threadPID2);
                pthread_cancel(threadPID3);
                pthread_cancel(threadPID4);
                exit(0);
            }     
        

    }
  
}

void* outputThread(void* args){

    char* ret;
    while(1){

        pthread_mutex_lock(&receiver);
        
            if(List_count(receiverList) == 0){
            
            pthread_cond_wait(&receiverCondition,&receiver);
            }

            ret = (char*)List_remove(receiverList);

        //free(dynamicAllctdMsgRecv);   
        pthread_mutex_unlock(&receiver);
        
        write(1,"Peer:",6);
        write(1,ret,strlen(ret));

    }
    
}


int main(int argc, char* argv[]){
    

    printf("Starting...\n");
    
    //structure to hold cmdline arguments
    if(argc<4){
        printf("Enter all arguments and try again\n");
        exit(0);
    }


    printf("Enter your message:\n");

    inputs ins;
    ins.first = argv[1];
    ins.second = argv[2];
    ins.third = argv[3];

    //main socket and data holder lists
    sock = socket(PF_INET,SOCK_DGRAM,0); 
    receiverList = List_create();
    senderList = List_create();
    
    //starting the threads
    pthread_create(&threadPID1,NULL,receiverThread,&ins);
    pthread_create(&threadPID2,NULL,inputThread,NULL);
    pthread_create(&threadPID3,NULL,outputThread,NULL);
    pthread_create(&threadPID4,NULL,senderThread,&ins);
    
    //waiting for all threads to end
    pthread_join(threadPID1,NULL);
    pthread_join(threadPID2,NULL);
    pthread_join(threadPID3,NULL);
    pthread_join(threadPID4,NULL);

    //cleanup
    pthread_mutex_destroy(&receiver);
    pthread_mutex_destroy(&sender);
    pthread_cond_destroy(&senderCondition);
    pthread_cond_destroy(&receiverCondition);
    close(sock);

    return 0;
}
