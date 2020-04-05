#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N_WRITER 4 //写者数目
#define N_READER 8 //读者数目
#define W_SLEEP 1 //控制写频率
#define R_SLEEP  1 //控制读频率

 pthread_t wid[N_WRITER],rid[N_READER];


int data = 0;

int readerCnt = 0, writerCnt = 0;

pthread_mutex_t accessReaderCnt = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t accessWriterCnt = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t writeLock = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t readerLock = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t outerLock = PTHREAD_MUTEX_INITIALIZER;

 

void wri()

{

	int rd = rand()%1000+1;

	printf("writer %1ld is writing %d\n",pthread_self(),rd);

	data = rd;

}

void rea()

{

	printf("reader %1ld is reading %d\n",pthread_self(),data);

}

void * writer(void * in)

{

	while(1)

	{

		pthread_mutex_lock(&accessWriterCnt);

		{//临界区,希望修改 writerCnt,独占 writerCnt

			writerCnt++;

			if(writerCnt == 1){

				//阻止后续的读者加入待读队列

				pthread_mutex_lock(&readerLock);

			}

		}

		pthread_mutex_unlock(&accessWriterCnt);

		

		

		pthread_mutex_lock(&writeLock);

		{//临界区，限制只有一个写者修改数据

			wri();

		}

		pthread_mutex_unlock(&writeLock);

		

		pthread_mutex_lock(&accessWriterCnt);

		{//临界区,希望修改 writerCnt,独占 writerCnt

			writerCnt--;

			if(writerCnt == 0){

				//阻止后续的读者加入待读队列

				pthread_mutex_unlock(&readerLock);

			}

		}

		pthread_mutex_unlock(&accessWriterCnt);

		sleep(W_SLEEP);

	}

	pthread_exit((void *) 0);

}

 

void * reader (void * in)

{

	while(1)

	{

		//假如写者锁定了readerLock,那么成千上万的读者被锁在这里

		pthread_mutex_lock(&outerLock);

		{//临界区

			pthread_mutex_lock(&readerLock);//只被一个读者占有

			{//临界区

				pthread_mutex_lock(&accessReaderCnt);//代码段 1

				{//临界区

					readerCnt++;

					if(readerCnt == 1){

						pthread_mutex_lock(&writeLock);

					}

				}

				pthread_mutex_unlock(&accessReaderCnt);

			}

			pthread_mutex_unlock(&readerLock);//释放时,写者将优先获得readerLock

		}

		pthread_mutex_unlock(&outerLock);

 

		rea();

		

		pthread_mutex_lock(&accessReaderCnt);//代码段2

		{//临界区

			readerCnt--;

			if(readerCnt == 0){

				pthread_mutex_unlock(&writeLock);//在最后一个并发读者读完这里开始禁止写者执行写操作

			}

		}

		pthread_mutex_unlock(&accessReaderCnt);

		

		sleep(R_SLEEP);

	}

	pthread_exit((void *) 0);

}

 

int main()

{

	int i = 0;

	for(i = 0; i < N_READER; i++)

	{

		pthread_create(&rid[i],NULL,reader,NULL);

	}

	for(i = 0; i < N_WRITER; i++)

	{

		pthread_create(&wid[i],NULL,writer,NULL);

	}

	while(1){

		sleep(10);

	}

	return 0;

}
