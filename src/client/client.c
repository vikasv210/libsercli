#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

/*
 * create_client : create the client and  return the socket
 * arguments : domain
 *             socket_type
 *             protocol
 *             ip_addr
 *             port
 * returns client_sock --> success
 *         -1 --> failure
 */
int create_client(int domain, int socket_type, int protocol,
		char *ip_addr, int port)
{
	int client_sock = 0;
	int ret = 0;

	struct sockaddr_in serv_addr;
	int addr_len = sizeof(struct sockaddr_in);

	/* client socket creation */
	client_sock = socket(domain, socket_type, protocol);
	if(client_sock < 0)
	{
		perror("Client socket creation failed");
		return -1;
	}

	/* Filling the address details */
	serv_addr.sin_family = domain; 
        serv_addr.sin_port = htons(port);

	/* ip address conversion */
	ret = inet_pton(domain, ip_addr, &serv_addr.sin_addr);
	if(ret < 0)
	{
		perror("ip address conversion failed");
		return -1;
	}

	ret = connect(client_sock, (struct sockaddr *)&serv_addr, addr_len);
	if(ret < 0)
	{
		perror("connect failed");
		return -1;
	}

	return client_sock;
}
