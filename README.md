# MP3
The first part of this program uses the Fork() command to create a separate process that runs the dataserver process. The client then sends a series of requests through the request channel to the dataserver. The dataserver returns a response for each request.

Compile:
make

Run:
./client

The second part extracts a wide variety process information associated with a passed-in process ID

Compile:
make

Run:
./proctest –p [PID]
