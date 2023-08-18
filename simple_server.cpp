/* run using ./server <port> */
#include <mutex>
#include <condition_variable>
#include "http_server.hh"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <bits/stdc++.h>
#include <pthread.h>

using namespace std;

queue<int> q;
pthread_mutex_t lck=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_var=PTHREAD_COND_INITIALIZER;
void error(char *msg) {
  perror(msg);
  exit(1);
}

void signalhandler(int a)
{
	exit(0);
}
void *start_function(void *arg) {

  int my_arg = *((int *) arg);

  while(1){
    pthread_mutex_lock(&lck);
    while(q.empty())
      pthread_cond_wait(&cond_var,&lck);
    
    int element=q.front();
    q.pop();
    pthread_mutex_unlock(&lck);
     // ...thread processing...
  char buffer[2048];
  int n;
  bzero(buffer, 2048);
  n = read(element, buffer, 255);
  if (n < 0)
    error("ERROR reading from socket");
  
 
  printf("Here is the message: %s", buffer);

   if(n==0){
    pthread_exit(0);
   }
  /* send reply to client */


  HTTP_Response *ptr=handle_request(buffer);

  string str=ptr->get_string();
   bzero(buffer, 2048);
   int i=0;
  for(i=0;i<str.length();i++){
    buffer[i]=str[i];
    //cout<<str[i];
  }
  buffer[i]='\0';
  //cout<<buffer<<endl;

  n = write(element,buffer,2048);

  if (n < 0)
    error("ERROR writing to socket");

  delete ptr;
  close(element);
  
  }
  
  return (void*)0;
 }
int main(int argc, char *argv[]) {
  int sockfd, newsockfd, portno;
  socklen_t clilen;
  char buffer[256];
  struct sockaddr_in serv_addr, cli_addr;
  int n;
  
  signal(SIGINT,signalhandler);

  if (argc < 2) {
    fprintf(stderr, "ERROR, no port provided\n");
    exit(1);
  }

  /* create socket */

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    error("ERROR opening socket");

  /* fill in port number to listen on. IP address can be anything (INADDR_ANY)
   */

  bzero((char *)&serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

   /* bind socket to this port number on this machine */

  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    error("ERROR on binding");

  /* listen for incoming connection requests */

  listen(sockfd,2000);
  clilen = sizeof(cli_addr);

  printf("server is listenting to port %d...\n",portno);

  pthread_t threads[15];
  int thread_arg[15];

 for(int i=0;i<15;i++)  // creating threads
    {
       thread_arg[i]=i;
       pthread_create(&threads[i],NULL,start_function,&thread_arg[i]);
    } 

  /* accept a new request, create a newsockfd */
  while(1){

  //cout<<"working.."<<endl;
  newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

  pthread_mutex_lock(&lck);
  q.push(newsockfd);
  pthread_cond_signal(&cond_var);
  pthread_mutex_unlock(&lck);


  if (newsockfd < 0)
    error("ERROR on accept");
  /*int thread_arg = newsockfd;
   pthread_t thread_id;

   pthread_create(&thread_id,NULL,start_function,&thread_arg);
  }*/
  }
  //q.clear();
  exit(0);
  return 0;
}
