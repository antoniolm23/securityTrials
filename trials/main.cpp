#include "server.h"

//main server
int main(int argc, char **argv) {
    cout << "Hello, world!" << std::endl;
    
    Server s = Server("localhost", 1237);
    s.receiveEvents();
    
    return 0;
}
