#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>


int value = 0,rc = 0;			// value is the shared variable to be modified or read
sem_t mutex2,mutex1;   			// mutex1 & mutex2 are for ensuring mutual exclusion.

void *writer(void *a)			//Function for writers
{
  int b;
  b = ((int) a);
  sem_wait(&mutex1);				//lock applied so other reader or writer may not come
  printf("no. of readers present is %d ",rc);
  value++;					//value incremented by writers
  printf("The written value is %d by writer %d\n",value,b);
  sem_post(&mutex1);				//lock released so readers or writers may come
}

void *reader(void *a)                    //Function for readers
{
  int b;
  b = ((int)a);
  sem_wait(&mutex2);			//mutex lock applied on readers //wait semaphore will decrease the value
  rc = rc++;
  if(rc==1)				// first reader check
   sem_wait(&mutex1);                   //mutex lock applied on writers if readcount is 1   //wait semaphore will decrease the value
  sem_post(&mutex2);			//mutex lock released ,so readers may come //post semaphore will increase the value
  printf("The value read is %d by reader %d\n",value,b);
  sem_wait(&mutex2);			//mutex lock applied on readers //wait semaphore will decrease the value
  rc = rc--;
  if(rc==0)				//last reader
   sem_post(&mutex1);			//mutex lock released //post semaphore will increase the value
  sem_post(&mutex2);			//mutex lock relesed //post semaphore will increase the value
}



int main()
{ 
  pthread_t r[5],w[5];                                //5 each of reader writer threads
  sem_init(&mutex2,0,1);
  sem_init(&mutex1,0,1);
  pthread_create(&r[0],NULL,reader,(void *)1);		
  pthread_create(&r[1],NULL,reader,(void *)2);
  pthread_create(&r[2],NULL,reader,(void *)3);
  pthread_create(&w[0],NULL,writer,(void *)1);
  pthread_create(&w[1],NULL,writer,(void *)2);
  pthread_create(&r[3],NULL,reader,(void *)4);
  pthread_create(&w[2],NULL,writer,(void *)3);
  pthread_create(&w[3],NULL,writer,(void *)4);
  pthread_create(&r[4],NULL,reader,(void *)5);
  pthread_create(&w[4],NULL,writer,(void *)5);
  pthread_join(r[0],NULL);
  pthread_join(r[1],NULL);
  pthread_join(r[2],NULL);
  pthread_join(w[0],NULL);
  pthread_join(w[1],NULL);
  pthread_join(r[3],NULL);
  pthread_join(w[2],NULL);
  pthread_join(w[3],NULL);
  pthread_join(r[4],NULL);
  pthread_join(w[4],NULL);

  return 0;
}
