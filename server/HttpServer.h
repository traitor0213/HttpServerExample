#ifndef _HTTP_SERVER_H
#define _HTTP_SERVER_H

DWORD WINAPI HttpServerProcedure(LPVOID lpParam)
{
    //for multi threading
    LPPROCEDURE_INFO info = (LPPROCEDURE_INFO)lpParam;
    SOCKET IOSocket = info->ResponseSocket;
    *info->MoveGarantie = TRUE;

    //create default http raw
    char lpHttpRaw[HTTP_LINE_SIZE];
    int size = 0;

    size = CreateHttpRaw(lpHttpRaw, sizeof(lpHttpRaw),
                         "HTTP/1.1 200 OK",
                         "Content", "");

    //response
    WriteHttpRaw(IOSocket, lpHttpRaw, size);
    //close http/1.1 session
    CloseHttpSession(IOSocket);

    return TRUE;
}

typedef int (WSAAPI *_closesocket)(SOCKET);
typedef PSTR (WINAPI *_RtlIpv4AddressToStringA) (const IN_ADDR *, PSTR);

void HttpServer(SOCKET *fd)
{
    //gui server thread 
    SOCKADDR_IN SocketInfo = {
        0,
    };

    PROCEDURE_INFO ProcedureInfo;
    BOOL MoveGarantie = FALSE;

    _RtlIpv4AddressToStringA lpfnRtlIpv4AddressToStringA;
    char IP[16];

    //accept loop
    for (;;)
    {
        //async accept
        int ResponseSocket = AcceptRequest(&SocketInfo, *fd);
        if (ResponseSocket < 0)
        {
            //case of socket error
            if (WSAGetLastError_() != WSAEWOULDBLOCK)
            {
                //thread exit
                break;
            }
            else 
            {
                Sleep(1);
            }
        }
        else
        {
            //ready for io procedure thread arguments 
            MoveGarantie = FALSE;
            ProcedureInfo.ResponseSocket = ResponseSocket;
            ProcedureInfo.MoveGarantie = &MoveGarantie;
            
            lpfnRtlIpv4AddressToStringA = (_RtlIpv4AddressToStringA)GetProcAddress(GetModuleHandleA("ntdll"), "RtlIpv4AddressToStringA");
            lpfnRtlIpv4AddressToStringA(&SocketInfo.sin_addr, IP);

            printf("%s\n", IP);

            //io procedure
            CreateThread_((FUNCTION_ADDRESS)HttpServerProcedure, &ProcedureInfo);

            //wait for variable move in thread stack  
            for (; MoveGarantie == FALSE;)
            {
                Sleep(0);
            }
        }
    }
}

#endif