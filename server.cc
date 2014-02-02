#include "server.h"

Server::Server() {
    // setup variables
    buflen_ = 1024;
    buf_ = new char[buflen_+1];
    clientQueue = QueueThreaded();

}

Server::~Server() {
    delete buf_;
}

void
Server::run() {
    // create and run the server
    create();
    set_up();
    serve();

}

void
Server::create() {

}

void
Server::set_up(){
//	Worker wo(buflen_, &clientQueue, &messageMap);
//	w=wo;
	for(int i = 0; i < 10; i++){
		Worker w(buflen_, &clientQueue, &messageMap);
		tenWorkers.push_back(w);
		sleep(1);
	}
}


void
Server::close_socket() {
}

void
Server::serve() {

    // setup client
    int client;
    struct sockaddr_in client_addr;
    socklen_t clientlen = sizeof(client_addr);

      // accept clients
    while (1) {
    	client = accept(server_,(struct sockaddr *)&client_addr,&clientlen);
    	clientQueue.enqueue(client);
//    	w.handle();
    }
    close_socket();
}


