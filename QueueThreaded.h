/*
 * QueueThreaded.h
 *
 *  Created on: Feb 1, 2014
 *      Author: edwardekstrom
 */

#ifndef QUEUETHREADED_H_
#define QUEUETHREADED_H_

#include <semaphore.h>
#include <pthread.h>
#include <queue>
#include <stdio.h>
#include <stdlib.h>
#include <istream>
#include <iostream>

namespace std {

class QueueThreaded {
public:
	QueueThreaded();
	virtual ~QueueThreaded();
	void enqueue(int);
	int dequeue();
private:

	queue<int> theQueue;
	sem_t s;
	sem_t n;
	sem_t e;
};

} /* namespace std */
#endif /* QUEUETHREADED_H_ */
