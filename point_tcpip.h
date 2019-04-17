#include <WinSock2.h>
#include <stdio.h>
#include <stdlib.h>




class POINT
{
public:
	
	POINT();
	virtual ~POINT();
	void Close();
	void Initilize();
	int Receive(char* lpData, int size);
	int Send(char* lpData, int size);
	int bind();
	int listen();
	void main();

	typedef struct student
	{
		int ID;
		char name[20];
		bool sex;
	}*target_student;
	
private:
	SOCKET  point_tcpip;
	SOCKADDR_IN addr;
	SOCKADDR_IN srcaddr;
};
//SOCKET point_tcpip(AF_INET,SOCK_STREAM,IPPROTO_TCP);