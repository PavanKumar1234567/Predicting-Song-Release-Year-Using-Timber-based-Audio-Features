
/* run using: ./load_gen localhost <server port> <number of concurrent users>
   <think time (in s)> <test duration (in s)> */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netdb.h>
#include<time.h>
#include <pthread.h>
#include <sys/time.h>


int time_up;
FILE *log_file;

// user info struct
struct user_info {
  // user id
  int id;

  // socket info
  int portno;
  char *hostname;
  float think_time;

  // user metrics
  int total_count;
  float total_rtt;
};

// error handling function
void error(char *msg) {
  perror(msg);
  exit(0);
}

void waitFor (unsigned int secs) 
{
    unsigned int retTime = time(0) + secs;   // Get finishing time.
    while (time(0) < retTime);               // Loop until it arrives.
}


// time diff in seconds
float time_diff(struct timeval *t2, struct timeval *t1) {
  return (t2->tv_sec - t1->tv_sec) + (t2->tv_usec - t1->tv_usec) / 1e6;
}

// user thread function
void *user_function(void *arg) {
  /* get user info */
  struct user_info *info = (struct user_info *)arg;//info->hostname()64total_rtt.time_diff

  int sockfd, n;
  char buffer[2048];
  struct timeval start, end;

  struct sockaddr_in serv_addr;
  struct hostent *server;

  while (1) {
    /* start timer */
    gettimeofday(&start, NULL);

    /* TODO: create socket */
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    	
    /* TODO: set server attrs */
    
     server=gethostbyname(info->hostname);
     if (server == NULL) {
     fprintf(stderr, "ERROR, no such host\n");
        exit(0);
  	}
      bzero((char *)&serv_addr, sizeof(serv_addr));
         serv_addr.sin_family = AF_INET;
         bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
         serv_addr.sin_port = htons(info->portno);
    /* TODO: connect to server */
    
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    	error("ERROR connecting");
    	
    /* TODO: send message to server */
      char buffer[2048]="";
      strcat(buffer,"GET /apart1/index.html HTTP/1.1");
      //printf("pavan82");
     int n = write(sockfd, buffer, strlen(buffer));
     if(n<0){
        error("error writing into socket");
     }
     bzero(buffer,2048);
    /* TODO: read reply from server */
    
     n = read(sockfd, buffer, 2048);
    /* TODO: close socket */
    //printf("kumar93");
    if(n==0){
    	printf("error in reading");
    	break;
    	}
    if (n < 0)
            error("ERROR reading from socket");
           //printf("Server response: %s\n", buffer);
     printf("got response from server\n");
     close(sockfd);
    /* end timer */
    gettimeofday(&end, NULL);

    /* if time up, break */
    if (time_up)
      break;

    /* TODO: update user metrics */
      info->total_count+=1;
      info->total_rtt+=time_diff(&end,&start);
    /* TODO: sleep for think time */
    	usleep(info->think_time*1000000);
  }
  fprintf(log_file, "User #%d finished\n", info->id);
  fflush(log_file);
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  int user_count, portno, test_duration;
  float think_time;
  char *hostname;

  if (argc != 6) {
    fprintf(stderr,
            "Usage: %s <hostname> <server port> <number of concurrent users> "
            "<think time (in s)> <test duration (in s)>\n",
            argv[0]);
    exit(0);
  }

  hostname = argv[1];
  portno = atoi(argv[2]);
  user_count = atoi(argv[3]);
  think_time = atof(argv[4]);
  test_duration = atoi(argv[5]);

  printf("Hostname: %s\n", hostname);
  printf("Port: %d\n", portno);
  printf("User Count: %d\n", user_count);
  printf("Think Time: %f s\n", think_time);
  //printf("abc\n");
  printf("Test Duration: %d s\n", test_duration);

  
 
  log_file = fopen("load_gen.log", "w");
   pthread_t threads[user_count];
  struct user_info info[user_count];
  struct timeval start, end;

  /* start timer */
  gettimeofday(&start, NULL);
  time_up = 0;
  for (int i = 0; i < user_count; ++i) {//id i+1 portno s total_rtt 0 total_count 0 hostname think_time
    /* TODO: initialize user info */
       info[i].id=i;
       info[i].portno=portno;
       info[i].total_rtt=0;
       info[i].total_count=0;
       info[i].hostname=hostname;
       info[i].think_time=think_time;
       
    /* TODO: create user thread */
    
    pthread_create(&threads[i],NULL,user_function,&info[i]);
    
    fprintf(log_file, "Created thread %d\n", i);
  }

  /* TODO: wait for test duration */
   waitFor(test_duration);
   
  fprintf(log_file, "Woke up\n");

  /* end timer */
  time_up = 1;
  gettimeofday(&end, NULL);

   /* TODO: wait for all threads to finish */
    void *ptr;
    for(int user=0;user<user_count;user++){
    	pthread_join(threads[user],&ptr);
    }
    
    //printf("line 185");
  /* TODO: print results */
   FILE *fp,*fp1;
   int total_req=0;
   float total_res=0;
   fp=fopen("values.csv","a");
   fp1=fopen("values1.csv","a");
   
   for(int p=0;p<user_count;p++){
        total_req+=info[p].total_count;
        total_res+=info[p].total_rtt;
   	printf("id:%d,total_count:%d,total_rtt%f\n",p+1,info[p].total_count,info[p].total_rtt);
   }
   
   fprintf(fp,"%d,%d,%f\n",user_count,total_req,total_res);
   fprintf(fp1,"%d,%d,%d\n",user_count,total_req,test_duration);
  /* close log file */
  fclose(log_file);
  fclose(fp);
  fclose(fp1);

  return 0;
}
