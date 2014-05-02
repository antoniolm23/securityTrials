#include "everything.h"
#include "server.h"
#include "client.h"

//main server
int main(int argc, char **argv) {
    cout << "Hello, world!" << std::endl;
    
    Server s = Server("localhost", 1234);
    s.receiveEvents();
    
    return 0;
}
