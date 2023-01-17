#include <sys/types.h>
   #include <sys/socket.h>
   #include <sys/un.h>
   #include <stdio.h>
   #include <stdlib.h>
   #include <unistd.h>
   #include<time.h>

    // server side
   #define NAME "mysocket"
   #define DATA "hiii"

   int main()
   {
       int sock, msgsock, rval, i, index = 0;
       struct sockaddr_un server;
       char buf[1024], payload[50][2];
       clock_t start, end; 

      sock = socket(AF_UNIX, SOCK_STREAM, 0); // create a socket
      if (sock < 0) {
          perror("opening stream socket");
          exit(1);
      }
      server.sun_family = AF_UNIX;
      strcpy(server.sun_path, NAME);
      if (bind(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un))) { // bind the socket to the address
          perror("binding stream socket");
          exit(1);
      }
      printf("Socket has name %s\n", server.sun_path);
      start = clock();
      listen(sock, 5);
      for (i = 0; i<1;i++) {
          msgsock = accept(sock, 0, 0); // accept a connection, tuple created between client and server
          if (msgsock == -1)
          {
            perror("accept");
          }
              
          else  {
            // bzero(payload, sizeof(payload));
            while(index + 4 <50){
              for (i = index ; i < index + 5; i++)
              {
                  rval = read(msgsock, &payload[i], sizeof(payload[i]));
                  printf("Data: %s , index: %d\n", payload[i],i);
              }
              if (i == 50){
                printf("end of data\n");
              }
              else{
                printf("next 5 elements\n");
              }

            index = i ;
            write(msgsock,&index,sizeof(int)); // write to the client
            }
          } 
          close(msgsock);
      }
      close(sock);
      unlink(NAME);
      end = clock();
      double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC; // in seconds
      printf("Time taken for transfer 50 times in seconds: %f\n", time_taken);

      return 0;
  }