  #include <sys/types.h>
   #include <sys/socket.h>
   #include <sys/un.h>
   #include <stdio.h>
   #include <stdlib.h>
   #include <unistd.h>
   #include<time.h>

    // client side
   #define DATA "Half a league, half a league . . ."


   int main(argc, argv)
       int argc;
       char *argv[];
  {
      int sock, i;
      struct sockaddr_un server;
      char buf[1024];
      int index = 0;
    //   char payload[10][2] = {"a","b","c","d","e","f","g","h","i","j"};
    srand(time(0));
    char payload[50][2];
    for (i = 0; i < 50; i++)
    {
        payload[i][0] = 'a' + rand() % 26;
        payload[i][1] = '\0';
    }

      if (argc < 2) {
          printf("usage:%s <pathname>", argv[0]);
          exit(1);
      }


      sock = socket(AF_UNIX, SOCK_STREAM, 0);
      if (sock < 0) {
          perror("opening stream socket");
          exit(1);
      }
      server.sun_family = AF_UNIX;
      strcpy(server.sun_path, argv[1]); // which unix domain socket to connect to


      if (connect(sock, (struct sockaddr *) &server, sizeof(struct sockaddr_un)) < 0) {
          close(sock);
          perror("connecting stream socket");
          exit(1);
      }
    
    while(index + 4 <50){
      for (i = index; i < index + 5; i++)
      {
          write(sock, &payload[i], sizeof(payload[i]));
      }
      // reading from server
      read(sock,&index,sizeof(int));
      printf("Index returned: %d\n", index - 1);
    }
    

      
      
      
      close(sock); // closes the socket

      
      return 0;
  }