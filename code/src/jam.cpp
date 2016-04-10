/**
 * JAM class - Module Coordinator acts as the link between all internal modules.
 * It functions as the single point of contact of JAM.
 *
 * @author: Hung Nguyen
 * @version 1.0 04/07/16
 */

#include "../include/jam.h"

#include <ifaddrs.h>
#include <arpa/inet.h>

using namespace std;

JAM::JAM()
        : udpWrapper_(&queues_),
          userHandler_(&queues_) {
}

JAM::~JAM() {

}

void JAM::StartAsLeader(const char *user_name,
                        const char *user_interface,
                        const char *user_port) {
    // INFO
    cout << user_name << " is starting a new chat group!" << endl;

    // Detect interface address
    sockaddr_in servaddr;
    if (GetInterfaceAddress(user_interface, user_port, &servaddr)) {
        clientManager_.AddClient(servaddr);
    } else {
        cerr << "Failed to detect network interface!" << endl;
        exit(1);
    }

    // Start UDP Wrapper
    if (udpWrapper_.Start(user_port) == SUCCESS) {
        cout << "Succeeded, listening on" << GetInterfaceAddressStr(user_interface, user_port) <<
                ". Current users:" << endl;
        clientManager_.PrintClients();
    } else {
        cerr << "Failed to start new chat group!" << endl;
        exit(1);
    }

    // Start User Handler
    userHandler_.Start();

    // Start-up completed
    cout << "Waiting for others to join..." << endl;

    Main();
}

void JAM::StartAsClient(const char *user_name,
                        const char *user_interface,
                        const char *user_port,
                        const char *serv_addr,
                        const char *serv_port) {
    // INFO
    cout << user_name << " is joining a chat group at " << serv_addr << ":" << serv_port << "!" << endl;

    // Start UDP Wrapper
    if (udpWrapper_.Start(user_port) == SUCCESS) {
        cout << "Listening on" << GetInterfaceAddressStr(user_interface, user_port) << endl;
    } else {
        cerr << "Failed to start UDP service!" << endl;
        exit(1);
    }

    // Initiate hand-shake

    // Start User Handler
    userHandler_.Start();

    Main();
}

void JAM::Main() {
    // Infinite loop to monitor central communication
    for (; ;) {
        if (queues_.wait_for_data(JAM_CENTRAL_TIMEOUT)) {
            if (queues_.is_terminate())
                break;          // Only exit loop if receive terminate signal

            // Go through each queue and handle data if available
            bool has_data = false;
            do {

            } while (has_data);
        }
    }

    DCOUT("INFO: JAM - Received terminate signal");
    udpWrapper_.Stop();

    std::cout << "Bye." << std::endl;
}

string JAM::GetInterfaceAddressStr(const char *port) {
    stringstream ss;
    ifaddrs *ifap, *ifa;
    sockaddr_in *sa;                // Only print IPv4
    char *name, *addr;
    const char *sep = "";

    getifaddrs(&ifap);
    for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr->sa_family == AF_INET) {
            name = ifa->ifa_name;
            if (strcmp(name, "lo0") != 0) {     // Ignore loop-back interface
                sa = (sockaddr_in *) ifa->ifa_addr;
                addr = inet_ntoa(sa->sin_addr);
                ss << sep << " (" << (const char *) name << ")" << (const char *) addr << ":" << port;
                sep = ",";
            }
        }
    }

    return ss.str();
}

string JAM::GetInterfaceAddressStr(const char *interface, const char *port) {
    stringstream ss;
    ifaddrs *ifap, *ifa;
    sockaddr_in *sa;                // Only print IPv4
    char *name, *addr;
    const char *sep = "";

    getifaddrs(&ifap);
    for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr->sa_family == AF_INET) {
            name = ifa->ifa_name;
            if (strcmp(name, interface) == 0) {     // Only select this interface
                sa = (sockaddr_in *) ifa->ifa_addr;
                addr = inet_ntoa(sa->sin_addr);
                ss << sep << " (" << (const char *) name << ")" << (const char *) addr << ":" << port;
                sep = ",";
            }
        }
    }

    return ss.str();
}

bool JAM::GetInterfaceAddress(const char *interface, const char *port, sockaddr_in *addr) {
    bool ret = false;
    ifaddrs *ifap, *ifa;
    char *name;

    getifaddrs(&ifap);
    for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr->sa_family == AF_INET) {
            name = ifa->ifa_name;
            if (strcmp(name, interface) == 0) {     // Only select this interface
                ret = true;
                memcpy(addr, ifa->ifa_addr, sizeof(sockaddr_in));
            }
        }
    }

    return ret;
}

