# Q2
## Using Fifos
The speak program sends 5 values to listen, listen returns the value of the highest index to speak. This continues till 50 elements are sent to listen.

_To run code:_

Go to q2/fifo/listen and q2/fifo/speak. Make them in their respective directories and run them on separate shell terminals. Make speak.c before listen.c 

__**If there's any error**__

Remove the fifo file from the directory and make again.

## Using Sockets
Server.c listens and waits for client.c to send a request. Client.c sends 5 elements of an array in this case, and server.c reads them.
The highest index of the array element is sent by server.c to client.c.

_To run code:_

Go to q2/sockets/client and q2/sockets/server. Make them in their respective directories and run them on separate shell terminals. Make server.c before client.c

__**If there's any error**__

Remove the socket file from the directory and make again.

## Using Shared Memory
Process synchronisation is required here. We need to make sure that the shared memory is accessed by only one process at the same instant, and both the processes know when it's their turn to read/write. So, 2 semaphores are needed here.

_To run code:_

Go to q2/sharedmem/reciever and q2/sharedmem/sender. Make them in their respective directories and run them on separate shell terminals. Make sender.c before reciever.c

__**If there's any error**__

__Run:__

rm /dev/shm/meow /tmp/semshm /tmp/semsync







