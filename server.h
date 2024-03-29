#pragma once

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

using namespace std;

class Server {
public:
    Server();
    ~Server();

    void run();
    
protected:
    virtual void create();
    virtual void close_socket();
    void serve();
    void handle(int);
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
   map<string, vector<pair<string, string> > > messageMap;

};
