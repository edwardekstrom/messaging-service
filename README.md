Data Structures where I used synchronization:

I used synchronization in QueueThreaded to protect the queue of incoming clients.

QueueThreaded.cc
enqueue() method
lines
27 - 32

dequeue() method
lines
37 - 43


I also used synchronization in the Server class to protect the map that stores my messages.

server.cc
parse_get()
lines
137 - 150

pase_put()
lines
182 - 197

parse_list()
lines
210 - 232
