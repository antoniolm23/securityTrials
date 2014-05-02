#include "server.h"

//main server
int main(int argc, char **argv) {
    cout << "Hello, world!" << std::endl;
    
    Server s = Server("localhost", 1235);
    s.receiveEvents();
    
    return 0;
}
