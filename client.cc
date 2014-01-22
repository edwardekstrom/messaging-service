#include "client.h"

Client::Client() {
    // setup variables
    buflen_ = 1024;
    buf_ = new char[buflen_+1];
}

Client::~Client() {
}

void Client::run() {
    // connect to the server and run echo program
    create();
    echo();
}

void
Client::create() {
}

void
Client::close_socket() {
}

void
Client::echo() {
    string line;

    // loop to handle user interface
    while (getline(cin,line)) {

    	if(line.find("send") == 0){
    		line = prepare_send(line);
    	}else if(line.find("list") == 0){
//    		line = prepare_list(line);
    	}else if(line.find("read") == 0){
//    		line = prepare_read(line);
    	}else if(line.find("quit") == 0){
    		exit(0);
    	} else{
    		// append a newline
    		line += "\n";
    	}
    	cout<<line;

        // send request
//        bool success = send_request(line);
        // break if an error occurred
//        if (not success)
//            break;
        // get a response
//        success = get_response();
        // break if an error occurred
//        if (not success)
//            break;
    }
    close_socket();
}

string
Client::prepare_send(string input){
	string user = wordAt(2, input);
	string subject = input.substr(input.find(user) + user.length(), string::npos);

	string message = "";
	while(getline(cin, input)){
		if(input == "\n"){
			break;
		}else{
			message += input;
		}
	}
	message += "\n";

	string length;
	ostringstream convert;
	convert << message.length();
	length = convert.str();

	string toReturn = "put " + user + " " + subject + " " + length + "\n" + message;

	cout << toReturn;
	return toReturn;
}

bool
Client::send_request(string request) {
    // prepare to send request
    const char* ptr = request.c_str();
    int nleft = request.length();
    int nwritten;
    // loop to be sure it is all sent
    while (nleft) {
        if ((nwritten = send(server_, ptr, nleft, 0)) < 0) {
            if (errno == EINTR) {
                // the socket call was interrupted -- try again
                continue;
            } else {
                // an error occurred, so break out
                perror("write");
                return false;
            }
        } else if (nwritten == 0) {
            // the socket is closed
            return false;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return true;
}

bool
Client::get_response() {
    string response = "";
    // read until we get a newline
    while (response.find("\n") == string::npos) {
        int nread = recv(server_,buf_,buflen_,0);
        if (nread < 0) {
            if (errno == EINTR)
                // the socket call was interrupted -- try again
                continue;
            else
                // an error occurred, so break out
                return "";
        } else if (nread == 0) {
            // the socket is closed
            return "";
        }
        // be sure to use append in case we have binary data
        response.append(buf_,nread);
    }
    // a better client would cut off anything after the newline and
    // save it in a cache
    cout << response;
    return true;
}


string
Client::wordAt(int index, string line)
{
    int count = 0; // number of read words
    string word; // the resulting word
    for (int i = 0 ; i < line.length(); i++) { // iterate over all characters in 'line'
        if (line[i] == ' ') { // if this character is a space we might be done reading a word from 'line'
//        	if (line[i+1] == '\n') throw 0;
            if (line[i+1] != ' ') { // next character is not a space, so we are done reading a word
                count++; // increase number of read words
    	        if (count == index) { // was this the word we were looking for?
    	            return word; // yes it was, so return it
    	        }
//    	        else if(i+1 > line.length()){
//    	        	throw 0;
//    	        }else if(line[i+1] == '\n'){
//    	        	throw 0;
//    	        }
    	        word =""; // nope it wasn't .. so reset word and start over with the next one in 'line'
            }
        }
        else { // not a space .. so append the character to 'word'
           word += line[i];
        }
    }
}
