#include "everything.h"
#include "server.h"
#include "client.h"

//main server
int main(int argc, char **argv) {
    cout << "Hello, world!" << std::endl;
    
    Client c = Client(1234, "anto", "localhost");
    c.receiveEvents();
    
    return 0;
}
