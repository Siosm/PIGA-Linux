#include <iostream>
#include <string>
#include <fstream>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstdlib>
#include <cstring>


std::string readFile(std::string filename)
{
	std::string content;
	std::cout << "file: " << filename << "trying open" << std::endl;
	std::ifstream file(filename.c_str(), std::ios::in);
	if (!file) {
		std::cerr << "file: " << filename << " can't be opened (or doesn't exist)" << std::endl;
		return -1;
	} else {
		while (std::getline(file, tmpLine)) {
			content += tmpLine + std::endl;
		}
		file.close();
		std::cout << "file: " << filename << " closed " << std::endl;
	}

	return content;
}

void writeFile(std::string filename, std::string content)
{
	std::string content;
	std::cout << "file: " << filename << "trying open" << std::endl;
	std::ifstream file(filename.c_str(), std::ios::in);
	if (!file) {
		std::cerr << "file: " << filename << " can't be opened (or doesn't exist)" << std::endl;
		return -1;
	} else {
		while (std::getline(file, tmpLine)) {
			content += tmpLine + std::endl;
		}
		file.close();
		std::cout << "file: " << filename << " closed " << std::endl;
	}
}

// int opensocket (char * host, int port)
// {
// 	int sd;
// 	struct sockaddr_in dhost;
// 	struct hostent *dhost_info = NULL;
// 
// 	dhost_info = gethostbyname(host);
// 
// 	if (dhost_info == NULL) {
// 		std::cerr << "Erreur gethostbyname(" << host << ")" << std::endl;
// 		return -1;
// 	}
// 
// 	sd = socket(AF_INET, SOCK_STREAM, 0);
// 
// 	if (sd < 0) {
// 		std::cerr << "Erreur socket" << std::endl;
// 		return -1;
// 	}
// 
// 	dhost.sin_family = dhost_info->h_addrtype;
// 	memcpy((char *) &dhost.sin_addr.s_addr, dhost_info->h_addr_list[0], dhost_info->h_length);
// 	dhost.sin_port = htons(port);
// 
// 	if(connect(sd, (struct sockaddr *) &dhost, sizeof(dhost)) < 0) {
// 		std::cerr << "Erreur : connect failed" << std::endl;
// 		close(sd);
// 		return -1;
// 	}
// 
// 	write(sd, "toto", 4);
// 
// 	sleep(2);
// 	
// 	close(sd);
// 	return 0;
// }

int main(int argc, char ** argv)
{
// 	if (argc == 1) {
		std::string a = readFile("/etc/passwd");
		writeFile("/tmp/test", a);
		std::string a = readFile("/tmp/test");
		return 0;
// 
// 	} else if (argc == 3) {
// 		return opensocket(argv[1], atoi(argv[2]));
// 	} else  {
// 		std::cout << "Usage (System): " << argv[0] << std::endl;
// 		std::cout << "Usage (Network): " << argv[0] << " @dest port" << std::endl;
// 		return -1;
// 	}

	return 0;
}
