#define WIN32_LEAN_AND_MEAN

#define DEFAULT_BUFFER_SIZE 2048
#define DEFAULT_INNER_BUFFER_SIZE 256

#include "../url/url.h"
#include "../native/wrap.h"
#include "../network/http.h"

#include <malloc.h>
#include <windows.h>
#include "server.h"
#include "HttpServer.h"


//main thread
int main()
{
    /*
    This function is using for initialize windows socket api function
    This function is call LoadLibraryA function 
    So, is not for linux OS, Only for windows
    */
    printf("[+] intialize tcp/ip library..\n");
    HMODULE SocketModule = InitializeTcpLibrary();
    printf("[+] %p\n", SocketModule);

    /*
    This WSAAPI function is using for initialize windows socket api module
    
    first argument is version number
    second argument is address of WSADATA variable
    */
    printf("[+] startup windows socket api\n");
    WSADATA wsa;
    WSAStartup_(MAKEWORD(2, 2), &wsa);

    const int ServerPort = 8080;
    const int HttpPort = 80;
    const int Backlog = 1024;

    SOCKET ServerSocket = OpenTcpPort(ServerPort, Backlog, TRUE);
    SOCKET HttpSocket = OpenTcpPort(HttpPort, Backlog, TRUE);

    /*
    error check
    if socket value is small than zero, error case
    */

    if (ServerSocket < 0 || HttpSocket < 0)
    {
        /*
        bind error || listen error
        
        defined "../network/tcp.h"

        #define ERROR_FROM_SOCKET 0x1
        #define ERROR_FROM_BIND 0x2
        #define ERROR_FROM_LISTEN 0x3
        #define ERROR_FROM_ACCEPT 0x4
        #define ERROR_FROM_CONNECT 0x5
        #define ERROR_FROM_IO 0x6
        */

        if (ServerSocket == ERROR_FROM_BIND || ServerSocket == ERROR_FROM_LISTEN)
        {
            printf("error from port open\n");
            closesocket_(ServerSocket);
            closesocket_(HttpSocket);
        }

        //free windows socket api
        WSACleanup_();
        //unmapping socket module
        FreeLibrary(SocketModule);

        return 0;
        //end of program
    }

    printf("[+] open port=%d, %d; backlog=%d\n", ServerPort, HttpPort, Backlog);

    /*
    create server thread
    'hServer' variable is server thread handle 
    */

    HANDLE hHttpServer = CreateThread_((FUNCTION_ADDRESS)HttpServer, (LPVOID)&HttpSocket);

    getchar();

    printf("[+] close socket..\n");
    //if called closesocket function, server thread is return
    closesocket_(ServerSocket);
    closesocket_(HttpSocket);
    //free windows socket api
    WSACleanup_();
    //unmapping socket module
    FreeLibrary(SocketModule);

    printf("[+] wait for thread..\n");
    /*
    wait for server thread return
    WaitForSingleObject function is blocking function 

    first argument is server thread handle
    second argument is wait time
    */
    WaitForSingleObject(hHttpServer, INFINITE);

    printf("[+] free..\n");

    for (int x = 0; x != _msize(UserName) / sizeof(char*); x++)
		free(UserName[x]);
	free(UserName);

    for (int x = 0; x != _msize(ActivateApplication) / sizeof(char*); x++)
		free(ActivateApplication[x]);
	free(ActivateApplication);
    
    for (int x = 0; x != _msize(Time) / sizeof(char*); x++)
		free(Time[x]);
	free(Time);

    //end of program

    return 0;
}
