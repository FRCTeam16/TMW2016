/*
 * VisionServer.cpp
 * 	// http://www.linuxhowtos.org/C_C++/socket.htm
	// http://www.bogotobogo.com/cplusplus/sockets_server_client.php
	// http://www.microhowto.info/howto/listen_on_a_tcp_port_with_connections_in_the_time_wait_state.html
 *
 */

#include "VisionServer.h"

#include <atomic>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#include <iostream>

void RunServer(int port, VisionServer* server, VisionDataParser* parser) {
	std::cout << "VisionServer::RunServer at " << port << ")\n";

	std::cout << "Starting RunServer for vision\n";
	int sockfd, n;
	struct sockaddr_in serv_addr, cli_addr;
	char buffer[256];

	std::cout << "Connecting to socket...";
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		throw std::runtime_error(std::string("ERROR opening socket"));
	}

	std::cout << "Setting socket options...";
	int reuseaddr = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr)) < 0) {
		throw std::runtime_error("Error setting socket option");
	}

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);

	std::cout << "Binding to socket...";
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		throw std::runtime_error("Error binding to socket");
	}

	if (listen(sockfd, 1) < 0) {
		throw std::runtime_error("Error listening");
	}
	unsigned int clilen = sizeof(cli_addr);

	while (true) {
		std::cout << "Accepting connections...\n";
		server->SetClientConnected(false);
		int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0) {
			std::cerr << "Error on accept";
			throw std::runtime_error("Error on accept");
		}
		server->SetClientConnected(true);
		while (true) {
			bzero(buffer,256);
			//std::cout << "VisionServer->Waiting on read\n";  // Could put counter here to only emit every so often
			n = read(newsockfd,buffer,255);
			if (n < 0) {
				throw std::runtime_error("Error reading from socket");
			}
			//std::cout << "Data [" << buffer << "]\n";
			if (n == 0) {
				break;
			}
			parser->AddRawData(buffer, n);
		}
		close(newsockfd);
		std::cout << "Closing connection session\n";
	}
}


VisionServer::VisionServer(const int port) :
		parser(new VisionDataParser())
{
	std::cout << "*** Constructing VisionServer(" << port <<  "***\n";
	server.reset(new Task("Vision Server", RunServer, port, this, parser.get()));
}

VisionServer::~VisionServer() {
}

VisionData VisionServer::GetVisionData() const {
	return parser->GetVisionData();
}

void VisionServer::SetClientConnected(bool connected) {
	clientConnected = connected;
}

bool VisionServer::GetClientConnected() const {
	return clientConnected;
}

void VisionServer::SMDB() {
	const VisionData vd = GetVisionData();

	SmartDashboard::PutBoolean("Vis:ClientConn",clientConnected);
	SmartDashboard::PutNumber("Vis:Left::X", 	vd.leftGoal.xposition);
	SmartDashboard::PutNumber("Vis:Left::Y", 	vd.leftGoal.yposition);
	SmartDashboard::PutNumber("Vis:Left:Width", vd.leftGoal.width);
	SmartDashboard::PutNumber("Vis:Left:Height",vd.leftGoal.height);
	SmartDashboard::PutNumber("Vis:Right::X", 	vd.rightGoal.xposition);
	SmartDashboard::PutNumber("Vis:Right::Y", 	vd.rightGoal.yposition);
	SmartDashboard::PutNumber("Vis:Right:Width", vd.rightGoal.width);
	SmartDashboard::PutNumber("Vis:Right:Height",vd.rightGoal.height);
	SmartDashboard::PutNumber("Vis:Pred::X", 	vd.predictedGoal.xposition);
	SmartDashboard::PutNumber("Vis:Pred::Y", 	vd.predictedGoal.yposition);
	SmartDashboard::PutNumber("Vis:Pred:Width", vd.predictedGoal.width);
	SmartDashboard::PutNumber("Vis:Pred:Height",vd.predictedGoal.height);
}

