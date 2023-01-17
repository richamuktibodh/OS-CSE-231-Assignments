# Q1

## 1 a)
To avoid deadlock, 1 philosopher starts picking up the forks in an opposite direction compared to the other 4. A semaphore array maintains whether the fork is in use or not.

## 1 b)
To avoid deadlock, a semaphore variable called mutex is used. Mutex gives a chance to 4 philosophers to pick up forks. If the fork is in use, the semaphores in the array forks[] won't let the philosopher pick the fork up.

## 2 

To avoid deadlock, a semaphore variable mutex and a counting semaphore for bowls is used, along with the semaphore array forks[].


