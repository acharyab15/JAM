/**
 * JAM class - Module Coordinator acts as the link between all internal modules.
 * It functions as the single point of contact of JAM.
 *
 * @author: Hung Nguyen
 * @version 1.0 04/07/16
 */

#ifndef JAM_JAM_H
#define JAM_JAM_H

#include "udp_wrapper.h"
#include "user_handler.h"
#include "client_manager.h"

class JAM {
public:
    JAM();

    ~JAM();

    /**
     * Start JAM as a leader
     *
     * Assume new group, start listening to incoming packets.
     *
     * @param username  user's name
     */
    void StartAsLeader(const char *username);

    /**
     * Start JAM as a client
     *
     * Join group by connecting to server address. If server doesn't respond then JAM returns error.
     *
     * @param username  user's name
     * @param addr      server's address
     * @param port      server's port
     */
    void StartAsClient(const char *username,
                       const char *addr,
                       const char *port);

    /**
     * Main running loop
     */
    void Main();

private:
    CentralQueues queues_;
    UdpWrapper udpWrapper_;
    UserHandler userHandler_;
    ClientManager clientManager_;

    /**
     * Construct string of ip address and port for network interfaces
     *
     * @param port      bind UDP port
     *
     * @return          ip:port string
     */
    std::string GetInterfaceAddressStr(const char *port);

    /**
     * Construct string of ip address and port for a specific network interface
     *
     * @param interface only select this specific interface
     * @param port      bind UDP port
     *
     * @return          ip:port string
     */
    std::string GetInterfaceAddressStr(const char *interface, const char *port);

    /**
     * Construct sockaddr_in of ip address and port for a specific network interface
     *
     * @param interface only select this specific interface
     * @param port      bind UDP port
     * @param addr      returned sockaddr_in
     *
     * @return          TRUE if detected; FALSE otherwise
     */
    bool GetInterfaceAddress(const char *interface, const char *port, sockaddr_in *addr);
};

#endif //JAM_JAM_H
