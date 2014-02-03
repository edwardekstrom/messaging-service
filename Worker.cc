/*
 * Worker.cc
 *
 *  Created on: Feb 1, 2014
 *      Author: edwardekstrom
 */

#include "Worker.h"

namespace std {

void *doit(void * vptr){
	Worker * w;
	w = (Worker*) vptr;
	w->handle();
}
Worker::Worker(int bufLength,
		QueueThreaded *qqq,
		map<string, vector<pair<string, string> > > *mmm) {

	buflen_ = bufLength;
	buf_ = new char[buflen_+1];

	clientQueue = qqq;
//	sem_t s = clientQueue.s;
//	sem_t n = clientQueue.n;
//	sem_t e = clientQueue.e;
//	queue<int> que = clientQueue;
	messageMap = mmm;

//	vector<pair<string, string> > vectorOfPairs;
//	vectorOfPairs = messageMap["eddy"];
//	pair<string, string> theMessage = vectorOfPairs[0];
//	cout << theMessage.first << ": " << theMessage.second << endl;

//	pthread_create(&thr, NULL, &doit, this);

};



Worker::~Worker() {
	// TODO Auto-generated destructor stub
}

void
Worker::handle(){
	while(1){
//		sem_wait(&(clientQueue->n));
//		sem_wait(&(clientQueue->s));
//		int client = clientQueue->theQueue.front();
//		clientQueue->theQueue.pop();
//		cout << "pop" << endl;
//		sem_post(&(clientQueue->s));
//		sem_post(&(clientQueue->e));
		int client = clientQueue->dequeue();
		handle(client);
	}
	pthread_join(thr, NULL);
}

void
Worker::handle(int client) {
    // loop to handle all requests
    while (1) {
        // get a request
        string request = get_request(client);

        // break if client is done or an error occurred
        if (request.empty())
        	break;

       //cout<< request;
        string response = parse_request(request);


        // send response
        bool success = send_response(client,response);
        // break if an error occurred
        if (not success)
            break;
    }
    close(client);
}

string
Worker::parse_request(string req){
	istringstream request(req);

	string line;

	if(req.find("put") == 0){
		try{
			parse_put(req);
			return "OK\n";
		}catch (int e){
			return "error incorrect send syntax.\n";
		}
	}
	else if(req.find("list") == 0){
		try{
			return parse_list(req);
		}catch (int e){
			return "error incorrect list syntax.\n";
		}
	}
	else if(req.find("get") == 0){
		try{
			return parse_get(req);
		}catch (int e){
			return "error incorrect read syntax.\n";
		}
	}
	else if(req.find("reset") == 0){
		try{
			return parse_reset(req);
		}catch (int e){
			return "error incorrect read syntax.\n";
		}
	}
	else{
//		cout << req;
		return "error bad syntax\n";
	}
}

string
Worker::parse_reset(string getString){
	messageMap->clear();
	return "OK\n";
}

string
Worker::parse_get(string getString){
//	int pos = getString.find("get") + 2;
	if(getString.length() < 5)
		throw 0;
	string name = wordAt(2, getString);
	int pos = getString.find(name) + name.length();
	if(pos+1 > getString.length()){
		throw 0;
	}
//	pos = getString.find("name");
//	if(pos > getString.length()){
//		throw 0;
//	}
	string indexString = wordAt(3, getString);
	indexString.erase(remove(indexString.begin(), indexString.end(), '\n'), indexString.end());
//	cout<<name<<indexString;
	int index;
	istringstream ( indexString ) >> index;
	index--;

	vector<pair<string, string> > vecInMap = (*messageMap)[name];
	int vectorSize = vecInMap.size() - 1;
	if(index > vectorSize){
		throw 0;
	}

	pair<string, string> pairInVector = vecInMap[index];

	string subject = pairInVector.first;
	string message = pairInVector.second;

	string toReturn = "message " + subject + "\n" + message;
	toReturn = toReturn.substr(0, toReturn.size()-1);
	return toReturn;
}

string
Worker::parse_put(string putString){
	string line;
	stringstream ss(putString);
	getline(ss, line);
	string name = wordAt(2, line);
	int pos = line.find(name) + name.length();
	if(pos+1 > line.length()){
		throw 0;
	}
	string subject = line.substr(pos + 1);
//	subject = subject.substr(0, subject.size()-2);
	if(!isdigit( subject.at(subject.length() -1 ))){
		throw 0;
	}
	string toReturn = "put " + name + " " + subject;
	string message = "";

	while(getline(ss, line)){
		message += line;
		message += "\n";
	}
	pair<string, string> newMessage;
	newMessage = make_pair(subject, message);
//	cout << "subject: " << newMessage.first << " message: " << newMessage.second << "\n";
	if(messageMap->count(name)){
		vector<pair<string, string> > messageList = (*messageMap)[name];
		messageList.push_back(newMessage);

		pair<string, vector<pair<string, string> > > messagePair(name, messageList);
		messageMap->erase(name);
		messageMap->insert(messagePair);

	}else{
		vector<pair<string,string> > newVector;
		newVector.push_back(newMessage);
		//pair<string, string> pairInVector = newVector[0];

//		cout << inList.first << inList.second << "\n";

//		pair<string, vector<pair<string, string> > > messagePair;
		(*messageMap)[name] = newVector;

		//vector<pair<string, string> > vecInMap = messageMap[name];

		//pair<string, string> inVecInMap = vecInMap[0];

//		cout << name << vecInMap.size() << " " << inVecInMap.first << inVecInMap.second <<  "\n";
	}

	return toReturn;
}

string
Worker::parse_list(string listString){
	string toReturn = "list ";
	if(listString.length() < 6){
		throw 0;
	}
	string user = wordAt(2, listString);
	user.erase(remove(user.begin(), user.end(), '\n'), user.end());
//	cout << user << "\n";

	vector<pair<string, string> > vecInMap = (*messageMap)[user];
//	cout << vecInMap.size() << "\n";element.first
	ostringstream messageCount;
	messageCount <<  vecInMap.size();

	toReturn += messageCount.str() + "\n";
	for(int i = 0; i < vecInMap.size(); i++){
		ostringstream index;
		index << i+1;
		pair<string , string> element = vecInMap[i];

		string subject = element.first;
		while(isdigit(subject[subject.size() - 1])){
			subject = subject.substr(0, subject.size() - 1);
		}
		if(isspace(subject[subject.length()-1])){
			subject = subject.substr(0, subject.size()-1);
		}

		toReturn += index.str() + " " + subject + "\n";
	}
//	cout<<toReturn;
	return toReturn;

}

string
Worker::get_request(int client) {
    string request = "";
    // read until we get a two newlines
    while (request.find("\n") == string::npos) {
        int nread = recv(client,buf_,buflen_,0);
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
        request.append(buf_,nread);
        request = check_whole(request, client);
    }
    // a better server would cut off anything after the newline and
    // save it in a cache
    //request.append("eddy is awesome");
    //cout<<request;
    return request;
}

string
Worker::check_whole(string reqSoFar, int client){
	string toReturn;
	toReturn += reqSoFar;
	try {
		if (reqSoFar.find("put") == 0) {

			string line;
			stringstream ss(reqSoFar);
			getline(ss, line);
			string name = wordAt(2, line);
			int pos = line.find(name) + name.length();
			if(pos+1 > line.length()){
				throw 0;
			}
			string subject = line.substr(pos + 1);
		//	subject = subject.substr(0, subject.size()-2);
			if(!isdigit( subject.at(subject.length() -1 ))){
				throw 0;
			}

			string lengthString = "";// = wordAt(4, reqSoFar);
			int j = subject.length() - 1;
			while (isdigit(subject[j])){
				lengthString = subject[j] + lengthString;
				j--;
			}

			while (!isdigit(lengthString[lengthString.size() - 1])) {
				lengthString = lengthString.substr(0, lengthString.size() - 1);
			}
			if(!isdigit(lengthString[0])){
				throw 0;
			}
			int length;
			istringstream buffer(lengthString);
			buffer >> length;

			string message = reqSoFar.substr(reqSoFar.find("\n") + 1,
					string::npos);
			while (message.length() < length) {
				int nread = recv(client, buf_, buflen_, 0);
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
				message.append(buf_, nread);
				toReturn.append(buf_, nread);
			}
			return toReturn;
		} else {
			return toReturn;
		}
	} catch (int e) {
		return toReturn;
	}

}

bool
Worker::send_response(int client, string response) {
    // prepare to send response
    const char* ptr = response.c_str();
    int nleft = response.length();
    int nwritten;
    // loop to be sure it is all sent
    while (nleft) {
        if ((nwritten = send(client, ptr, nleft, 0)) < 0) {
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

string
Worker::wordAt(int index, string line)
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

} /* namespace std */
