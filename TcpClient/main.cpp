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
    int socketClient = socket(AF_INET , SOCK_STREAM , 0) ;
    if (socketClient == -1) {
        cerr << "error in create socket!" ;
        return -1 ;
    }
    //create address socket
    int port = 54000 ;
    string ipAddress = "127.0.0.1" ; //Local Host

    struct sockaddr_in socketAdrress ;
    socketAdrress.sin_family = AF_INET ;
    socketAdrress.sin_port = htons(9002) ;
    inet_pton(AF_INET , ipAddress.c_str() , &socketAdrress.sin_addr) ;

    //Connect To Server On The Socket
    int connectRes = connect(socketClient , (sockaddr*)&socketAdrress , sizeof(socketAdrress)) ;
    if (connectRes == -1) {
        return 1 ;
    }

    char buff [2048] ;
    string userInput ;

    //While Loop
    do {
        //Enter Lines Of Text
        cout << "> ";
        getline(cin , userInput) ;

        //Send To Server
        int sendRes = send(socketClient , userInput.c_str() , userInput.size() + 1 , 0) ;
        if (sendRes == -1) {
            cout << "Could Not Send To Server!" << endl ;
            continue ;
        }
        //Wait For Response
        memset(buff , 2048 , NI_MAXHOST) ;
        int receive = recv(socketClient, buff , 2048 , 0);
        if (receive == -1) {
            cout << "There Was An Error Getting Response From Server." << endl ;
        }else {
            //Display Response
            cout << "<SERVER>" << string(buff, receive) << endl;
        }
    }while(true) ;

    //Close Socket
    close(socketClient) ;

    return  0 ;
}

