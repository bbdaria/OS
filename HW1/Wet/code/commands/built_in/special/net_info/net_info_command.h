#ifndef NETINFO_COMMAND_H_
#define NETINFO_COMMAND_H_

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
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
            std::cerr << m_err_msg << std::endl;
            return;
        }

        try {
            // Get network information
            std::string ip = getIPAddress(m_interface);
            std::string subnet = getSubnetMask(m_interface);
            std::string gateway = getDefaultGateway();
            std::string dns = getDNSServers();

            // Print network information
            std::cout << "IP Address: " << ip << std::endl;
            std::cout << "Subnet Mask: " << subnet << std::endl;
            std::cout << "Default Gateway: " << gateway << std::endl;
            std::cout << "DNS Servers: " << dns << std::endl;
        } catch (const std::runtime_error& e) {
            std::cerr << "smash error: netinfo: interface " << m_interface << " does not exist" << std::endl;
        }
    }

private:
    std::string m_interface;
    bool m_error;
    std::string m_err_msg;

    std::string getIPAddress(const std::string& interface) {
        std::ifstream file("/proc/net/fib_trie");
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open network info file");
        }

        std::string line;
        std::string ip;
        while (std::getline(file, line)) {
            if (line.find(interface) != std::string::npos) {
                while (std::getline(file, line)) {
                    if (line.find("32 host") != std::string::npos) {
                        std::istringstream iss(line);
                        iss >> ip;
                        return ip;
                    }
                }
            }
        }

        throw std::runtime_error("Interface not found");
    }

    std::string getSubnetMask(const std::string& interface) {
        // You can parse this information from system files or use external tools like `ifconfig`.
        // Placeholder implementation:
        return "255.255.255.0";
    }

    std::string getDefaultGateway() {
        std::ifstream file("/proc/net/route");
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open routing table");
        }

        std::string line;
        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string iface, dest, gateway;
            if (iss >> iface >> dest >> gateway) {
                if (dest == "00000000") {
                    unsigned long gw;
                    std::stringstream ss;
                    ss << std::hex << gateway;
                    ss >> gw;

                    return std::to_string((gw & 0xFF)) + "." +
                           std::to_string((gw >> 8) & 0xFF) + "." +
                           std::to_string((gw >> 16) & 0xFF) + "." +
                           std::to_string((gw >> 24) & 0xFF);
                }
            }
        }

        throw std::runtime_error("Default gateway not found");
    }

    std::string getDNSServers() {
        std::ifstream file("/etc/resolv.conf");
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open DNS configuration");
        }

        std::string line;
        std::vector<std::string> dnsServers;
        while (std::getline(file, line)) {
            if (line.find("nameserver") == 0) {
                dnsServers.push_back(line.substr(11)); // Skip "nameserver "
            }
        }

        if (dnsServers.empty()) {
            throw std::runtime_error("No DNS servers found");
        }

        std::ostringstream oss;
        for (size_t i = 0; i < dnsServers.size(); ++i) {
            if (i > 0) oss << ", ";
            oss << dnsServers[i];
        }

        return oss.str();
    }
};

#endif // NETINFO_COMMAND_H_