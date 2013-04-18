#pragma once
#include <iostream>
#include <string>

#include <stdlib.h>
#include <winsock.h>//dont forget to add wsock32.lib to linker dependencies

using namespace std;

class AchievementHandler
{
public://hud_crosshair
	AchievementHandler(void);
	~AchievementHandler(void);

	static bool hasAchievement(string user, string achievementName)
	{
		string request;
		string response;
		int resp_leng;
		const int BUFFERSIZE = 400;
		char buffer[BUFFERSIZE];
		struct sockaddr_in serveraddr;
		int sock;

		WSADATA wsaData;
		char *ipaddress = "127.0.0.1";
		int port = 8080;
		string url = "/hasAchievement/" + user + "/" + achievementName;
		request+="GET " + url + " HTTP/1.1\r\n";//  /hasAchievement.html
		request+="Host: localhost\r\n";
		request+="\r\n";

		//init winsock
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			die_with_wserror("WSAStartup() failed");

		//open socket
		if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
			die_with_wserror("socket() failed");

		//connect
		memset(&serveraddr, 0, sizeof(serveraddr));
		serveraddr.sin_family      = AF_INET;
		serveraddr.sin_addr.s_addr = inet_addr(ipaddress);
		serveraddr.sin_port        = htons((unsigned short) port);
		if (connect(sock, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
			die_with_wserror("connect() failed");

		//send request
		if (send(sock, request.c_str(), request.length(), 0) != request.length())
			die_with_wserror("send() sent a different number of bytes than expected");

		//get response
		response = "";
		resp_leng= BUFFERSIZE;
		while (resp_leng == BUFFERSIZE)
		{
			resp_leng= recv(sock, (char*)&buffer, BUFFERSIZE, 0);
			if (resp_leng>0)
				response+= string(buffer).substr(0,resp_leng);
			//note: download lag is not handled in this code
		}

		//disconnect
		closesocket(sock);
		
		//cleanup
		WSACleanup();
		
		unsigned found = response.find("true");
		if(found != string::npos) return true;
		return false;
	}

	static bool addAchievement(string user, string achievementName)
	{
		string request;
		string response;
		int resp_leng;
		const int BUFFERSIZE = 400;
		char buffer[BUFFERSIZE];
		struct sockaddr_in serveraddr;
		int sock;

		WSADATA wsaData;
		char *ipaddress = "127.0.0.1";
		int port = 8080;
		string url = "/insertAchievement/" + user + "/" + achievementName;
		request+="GET " + url + " HTTP/1.1\r\n";//  /hasAchievement.html
		request+="Host: localhost\r\n";
		//request+="Connection: close\r\n";
		//request+="Content-Disposition: form-data; user=\"Nevik\"; achievement=\"Secret1\"\r\n";
		//request+=
		request+="\r\n";

		//init winsock
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
			die_with_wserror("WSAStartup() failed");

		//open socket
		if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
			die_with_wserror("socket() failed");

		//connect
		memset(&serveraddr, 0, sizeof(serveraddr));
		serveraddr.sin_family      = AF_INET;
		serveraddr.sin_addr.s_addr = inet_addr(ipaddress);
		serveraddr.sin_port        = htons((unsigned short) port);
		if (connect(sock, (struct sockaddr *) &serveraddr, sizeof(serveraddr)) < 0)
			die_with_wserror("connect() failed");

		//send request
		if (send(sock, request.c_str(), request.length(), 0) != request.length())
			die_with_wserror("send() sent a different number of bytes than expected");

		//get response
		response = "";
		resp_leng= BUFFERSIZE;
		while (resp_leng == BUFFERSIZE)
		{
			resp_leng= recv(sock, (char*)&buffer, BUFFERSIZE, 0);
			if (resp_leng>0)
				response+= string(buffer).substr(0,resp_leng);
			//note: download lag is not handled in this code
		}

		//disconnect
		closesocket(sock);
		
		//cleanup
		WSACleanup();
		
		return true;
	}

	static void die_with_error(char *errorMessage)
	{
		cerr << errorMessage << endl;
		exit(1);
	}

	static void die_with_wserror(char *errorMessage)
	{
		cerr << errorMessage << ": " << WSAGetLastError() << endl;
		exit(1);
	}
};
