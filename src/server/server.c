#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

/*
 * create_server : create the server and return the socket
 * arguments : domain
 *             socket_type
 *             protocol
 *             sock_opt_lvl
 *             opt_name
 *             popt_val
 *             opt_len
 *             port
 *             incoming_accept_addr
 * return socket --> success
 *        error code --> failure
 */
int create_server(int domain, int socket_type, int protocol, 
	int sock_opt_lvl, int opt_name, int opt_val, int opt_len, int port,
	unsigned long int incoming_accept_addr)
{
	int server_fd = 0;
	int socket_fd = 0;
	int ret = 0;

	struct sockaddr_in address;
	int addr_len = sizeof(struct sockaddr_in);

	/* socket creation */	
	server_fd = socket(domain, socket_type, protocol);
	if(server_fd < 0)
	{
		perror("Socket creation failed");
		return -1;
	}

	/* adding the socket options */
	ret = setsockopt(server_fd, sock_opt_lvl, opt_name,
                                &opt_val, opt_len);
	if(ret < 0)
	{
		perror("setsockopt failed");
		return -1;
	}

	/* filling the address details */
	address.sin_family = domain;
	address.sin_addr.s_addr = incoming_accept_addr;
	address.sin_port = htons(port);	

	/* binding the soocket with address */
	ret = bind(server_fd, (struct sockaddr *)&address,
                                addr_len);
	if(ret < 0)
	{
		perror("binding failed");
		return -1;
	}

	/* listening the socket */
	ret = listen(server_fd, 3);
	if(ret < 0)
	{
		perror("listen failed");
		return -1;
	}

	socket_fd = accept(server_fd, (struct sockaddr *)&address,
                                        (socklen_t*)&addr_len);
	if(socket_fd < 0)
	{
		perror("accept failed");
		return -1;
	}

	return socket_fd;
}	

