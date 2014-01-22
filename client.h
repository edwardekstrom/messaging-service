#pragma once

#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class Client {
public:
    Client();
    ~Client();


    void run();

protected:
    virtual void create();
    virtual void close_socket();
    void echo();
    bool send_request(string);

    string prepare_send(string line);
    string prepare_list(string line);
    string prepare_read(string line);
    static string wordAt(int index, string line);


    bool get_response();
    void parse_response(string s);

    int server_;
    int buflen_;
    char* buf_;
};
