/**
 * UDP Wrapper module acts as a low-level I/O handler to send/receive message
 * over UDP datagram reliably.
 *
 * @author: Hung Nguyen
 * @version 1.0 03/31/16
 */

#ifndef JAM_UDP_WRAPPER_H
#define JAM_UDP_WRAPPER_H

#include "config.h"
#include "payload.h"
#include "concurrent_queue.h"

#include <boost/thread/thread.hpp>
#include <string.h>
#include <sstream>
#include <iomanip>
#include <chrono>

class UdpWrapper {
public:
    UdpWrapper();

    ~UdpWrapper();

    /**
     * Initialize UDP socket and start listening
     *
     * @return          SUCCESS on normal operation, other JamStatus errors otherwise
     */
    JamStatus Start();

    // TODO: implement start as client

    /**
     * Terminate all working threads and exit
     *
     * @return          SUCCESS on normal operation, other JamStatus errors otherwise
     */
    JamStatus Stop();

    /**
     * Update internal client addresses
     *
     * @param clients   list of client sockaddr_in
     */
    void UpdateClientAddresses(std::vector<sockaddr_in> *clients);

    /**
     * Put payload to a single receiver to queue
     *
     * UDP socket must be init before this function can be used.
     * Receiver's address must be encoded in the payload.
     *
     * @param payload   ready to encode payload
     *
     * @return          SUCCESS on normal operation, other JamStatus errors otherwise
     */
    JamStatus SendPayload(Payload payload);

    /**
     * Put payload to client list to queue
     *
     * UDP socket must be init before this function can be used.
     * Wrapper will use internal client list for distributing.
     *
     * @param payload   ready to encode payload
     *
     * @return          SUCCESS on normal operation, other JamStatus errors otherwise
     */
    JamStatus DistributePayload(Payload payload);

    /**
     * Convert IP address and port to sockaddr_in type
     *
     * @param ip        ip address or host name
     * @param port      port
     * @param addr      return correspond sockaddr_in
     *
     * @return          SUCCESS on normal operation, other JamStatus errors otherwise
     */
    static JamStatus GetAddressFromInfo(const char *ip, const char *port, sockaddr_in *addr);

private:
    /**
     * Internal monitor variable for each payload sent out.
     *
     * Mainly used for ack queue.
     */
    struct Ticket {
        uint32_t uid;                           // Assigned UID
        uint8_t num_retries;                    // Number of retries left
        std::chrono::milliseconds time_sent;    // Time sent by Writer (miliseconds since epoch)
    };

    bool is_ready_;                             // UDP socket ready for communication
    int sockfd_;                                // Main socket file descriptor
    sockaddr_in this_addr_;                     // This client's address
    std::vector<sockaddr_in> clients_;          // Up-to-date client addresses
    uint32_t uid_;                              // UID counter

    ConcurrentQueue<Payload> out_queue_;        // Thread-safe outgoing payload queue for distributing
    ConcurrentQueue<Payload> in_queue_;         // Thread-safe incoming payload queue for processing
    ConcurrentQueue<Ticket> ack_queue_;         // Thread-safe incoming ack queue for monitoring

    boost::thread t_reader_;                    // Reader thread for RunReader()
    boost::thread t_writer_;                    // Writer thread for RunWriter()
    boost::thread t_monitor_;                   // Monitor thread for RunMonitor()

    /**
     * Initialize listening UDP socket (bind to specific port)
     *
     * @returns         SUCCESS if bind normally, other JamStatus errors otherwise
     */
    JamStatus InitUdpSocket();

    /**
     * Start reader thread to listen for incoming packets.
     */
    void RunReader();

    /**
     * Start writer thread to distribute packets.
     */
    void RunWriter();

    /**
     * Start monitor thread to keep track of non-ack packets.
     */
    void RunMonitor();

    // -- Helper functions
    std::string u32_to_string(uint32_t in);
};

#endif //JAM_UDP_WRAPPER_H
