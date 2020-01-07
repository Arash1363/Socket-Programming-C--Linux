#include <iostream>

#include <sys/socket.h>
#include <sys/types.h>

#include <string.h>
#include <string>

#include <unistd.h>

#include <arpa/inet.h>
#include <netdb.h>
using namespace std ;

int main() {

    //create socket
    int socketServer = socket(AF_INET, SOCK_STREAM, 0);
    if (socketServer == -1) {
        cerr << "cant socket create." << endl;
        return -1;
    }

    //create socket address
    sockaddr_in sockAdd ;
    sockAdd.sin_family = AF_INET ;
    sockAdd.sin_port = htons(54000) ;
    inet_pton(AF_INET , "127.0.0.1" , &sockAdd.sin_addr) ;

    //bind to ip/port
    bind(socketServer , (sockaddr*)&sockAdd, sizeof(sockAdd)) ;

    //listening for connecion
    if (listen(socketServer , SOMAXCONN) == -1) {
        cout << "cant listen" << endl ;
        return -3 ;
    }

    //accept connection
    sockaddr_in client ;
    socklen_t clientSize = sizeof(client) ;
    char host[NI_MAXHOST] ;
    char svc[NI_MAXSERV] ;

    int clientSocket = accept(socketServer , (sockaddr*)&client , &clientSize) ;
    if (clientSocket == -1) {
        cerr << "Problem With Client Connecting!" << endl ;
        return -4 ;
    }

    //Close Server Socket
    close(socketServer) ;

    memset(host , 0 , NI_MAXHOST) ;
    memset(svc , 0 , NI_MAXSERV) ;

    int result = getnameinfo((sockaddr*)&client , sizeof(client) , host , NI_MAXHOST , svc , NI_MAXSERV , 0) ;

    if (result) {
        cout << host << " connected on " << svc << endl ;
    }else {
        inet_ntop(AF_INET , &client.sin_addr , host , NI_MAXHOST) ;
        cout << host << " connected on " << ntohs(client.sin_port) << endl ;
    }

    //Receiving and Display Message
    char buff[2048] ;
    while (true) {
        memset(buff, 0, 2048);
        int byteRecv = recv(clientSocket , buff , 2048 , 0) ;

        if (byteRecv == -1) {
            cerr << "There was a connection issue" << endl ;
            break ;
        }

        if (byteRecv == 0) {
            cerr << "The client disconnected" << endl ;
            break ;
        }
        //Display Message
        cout << "Received: " << string(buff , 0 , byteRecv) << endl ;

        //Resend Message
        send(clientSocket , buff , byteRecv + 1 , 0) ;
    }
    //Close Client Socket
    close(clientSocket) ;

    return 0 ;
}
