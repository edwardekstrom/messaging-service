/*
 * Worker.h
 *
 *  Created on: Feb 1, 2014
 *      Author: edwardekstrom
 */

#ifndef WORKER_H_
#define WORKER_H_

#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <istream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <list>
#include <vector>
#include <algorithm>
#include <semaphore.h>
#include <pthread.h>
#include <queue>
#include "QueueThreaded.h"
namespace std {

class Worker {
public:
	Worker(int , QueueThreaded*, map<string, vector<pair<string, string> > >*);
	virtual ~Worker();

	void set_messageMap(map<string, vector<pair<string, string> > >);
	void handle();

protected:

    void serve();
    void set_up();

    string get_request(int);
    bool send_response(int, string);
    string parse_request(string req);
    string parse_put(string putString);
    string parse_list(string listString);
    string parse_get(string listString);
    string parse_reset(string listString);
    static string wordAt(int index, string line);

    string check_whole(string reqSoFar, int client);


    int server_;
    int buflen_;
    char* buf_;

private:
    void handle(int);

   map<string, vector<pair<string, string> > > messageMap;

   QueueThreaded clientQueue;
};

} /* namespace std */
#endif /* WORKER_H_ */
