#ifndef _WSOCKET_TYPE_H
#define _WSOCKET_TYPE_H

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib,   "ws2_32.lib ") 
#define ERRNO		(WSAGetLastError())
#define NEEDBLOCK	WSAEWOULDBLOCK
typedef int socklen_t;
#else
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#ifndef INVALID_SOCKET
#define INVALID_SOCKET	(SOCKET)(~0)
#endif
#ifndef SOCKET_ERROR
#define SOCKET_ERROR	(-1)
#endif
#ifndef closesocket
#define closesocket(x)	::close(x)
#endif
typedef int SOCKET;
#define ERRNO (errno)
#define NEEDBLOCK	EAGAIN
#define SD_RECEIVE      0x00
#define SD_SEND         0x01
#define SD_BOTH         0x02
#endif

#endif