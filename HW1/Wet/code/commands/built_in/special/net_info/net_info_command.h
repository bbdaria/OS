#ifndef NETINFO_COMMAND_H_
#define NETINFO_COMMAND_H_

#include <iostream>
#include <cstring>
#include <vector>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include "../../built_in_command.h"

class NetinfoCommand : public BuiltInCommand {
public:
    NetinfoCommand(char** args, int words) : m_error(false) {
        if (words <= 1) {
            m_err_msg = "smash error: netinfo: interface not specified";
            m_error = true;
        } else {
            m_interface = args[1];
        }
    }

    void execute() override {
        if (m_error) {
            printErr(m_err_msg);
            printErr(std::endl);
            return;
        }

        int sock = socket(AF_INET, SOCK_DGRAM, 0);
        if (sock < 0) {
            perror("smash error: socket failed");
            return;
        }

        struct ifreq ifr;
        memset(&ifr, 0, sizeof(ifr));
        strncpy(ifr.ifr_name, m_interface.c_str(), IFNAMSIZ - 1);

        // Get IP Address
        if (ioctl(sock, SIOCGIFADDR, &ifr) < 0) {
            printErr("smash error: netinfo: interface ");
            printErr(m_interface);
            printErr(" does not exist");
            printErr(std::endl);
            close(sock);
            return;
        }
        struct sockaddr_in* ipaddr = reinterpret_cast<struct sockaddr_in*>(&ifr.ifr_addr);
        std::string ip = inet_ntoa(ipaddr->sin_addr);

        // Get Subnet Mask
        if (ioctl(sock, SIOCGIFNETMASK, &ifr) < 0) {
            perror("smash error: ioctl failed");
            close(sock);
            return;
        }
        struct sockaddr_in* netmask = reinterpret_cast<struct sockaddr_in*>(&ifr.ifr_netmask);
        std::string subnet = inet_ntoa(netmask->sin_addr);

        // Get Default Gateway (from /proc/net/route)
        std::string gateway = getDefaultGateway(m_interface);

        // Get DNS Servers (from /etc/resolv.conf)
        std::vector<std::string> dns_servers = getDNSServers();

        // Output the results
        std::cout << "IP Address: " << ip << std::endl;
        std::cout << "Subnet Mask: " << subnet << std::endl;
        std::cout << "Default Gateway: " << gateway << std::endl;
        std::cout << "DNS Servers: ";
        for (size_t i = 0; i < dns_servers.size(); ++i) {
            std::cout << dns_servers[i];
            if (i < dns_servers.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;

        close(sock);
    }

private:
    bool m_error;
    std::string m_err_msg;
    std::string m_interface;

    std::string getDefaultGateway(const std::string& interface) {
        int fd = open("/proc/net/route", O_RDONLY);
        if (fd < 0) {
            perror("smash error: open failed");            
            return "";
        }

        char buffer[4096];
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        close(fd);
        if (bytes_read <= 0) {
            perror("smash error: read failed");            
            return "";
        }

        buffer[bytes_read] = '\0';
        std::istringstream stream(buffer);
        std::string line;
        while (std::getline(stream, line)) {
            std::istringstream line_stream(line);
            std::string iface, destination, gateway;
            line_stream >> iface >> destination >> gateway;
            if (iface == interface && destination == "00000000") {
                unsigned int gw;
                std::stringstream ss;
                ss << std::hex << gateway;
                ss >> gw;
                struct in_addr gw_addr = {gw};
                return inet_ntoa(gw_addr);
            }
        }
        return "";
    }

    std::vector<std::string> getDNSServers() {
        int fd = open("/etc/resolv.conf", O_RDONLY);
        if (fd < 0) {
            perror("smash error: open failed");            
            return {};
        }

        char buffer[4096];
        ssize_t bytes_read = read(fd, buffer, sizeof(buffer) - 1);
        close(fd);
        if (bytes_read <= 0) {
            perror("smash error: read failed");            
            return {};
        }

        buffer[bytes_read] = '\0';
        std::istringstream stream(buffer);
        std::string line;
        std::vector<std::string> dns_servers;
        while (std::getline(stream, line)) {
            if (line.find("nameserver") == 0) {
                std::istringstream line_stream(line);
                std::string ignore, address;
                line_stream >> ignore >> address;
                dns_servers.push_back(address);
            }
        }
        return dns_servers;
    }
};

#endif // NETINFO_COMMAND_H_