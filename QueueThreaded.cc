/*
 * QueueThreaded.cpp
 *
 *  Created on: Feb 1, 2014
 *      Author: edwardekstrom
 */

#include "QueueThreaded.h"

namespace std {

QueueThreaded::QueueThreaded() {
	sem_init(&s, 0, 1);
	sem_init(&n, 0, 0);
	sem_init(&e, 0, 10);

}

QueueThreaded::~QueueThreaded() {

}

void
QueueThreaded::enqueue(int client){
	sem_wait(&e);
	sem_wait(&s);
	theQueue.push(client);
	cout << "pushed" <<endl;
	sem_post(&s);
	sem_post(&n);
}

int
QueueThreaded::dequeue(){
	sem_wait(&n);
	sem_wait(&s);
	int client = theQueue.front();
	theQueue.pop();
	cout << "popped" << endl;
	sem_post(&s);
	sem_post(&e);
}


} /* namespace std */
