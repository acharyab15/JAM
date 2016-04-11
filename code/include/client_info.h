//
// Created by acharyab on 4/5/16.
//

#ifndef JAM_CLIENT_INFO_H
#define JAM_CLIENT_INFO_H

#include <netinet/in.h>
#include <string>

#include "config.h"
#include "serializer_helper.h"

class ClientInfo {
public:
    ClientInfo(sockaddr_in client, const std::string& username, bool is_Leader = false);
    bool operator<(const ClientInfo& other);
    bool operator==(const ClientInfo& other);
    sockaddr_in GetSockAddress();
    static uint32_t GetPacketSize();

    static JamStatus EncodeClientInBuffer(ClientInfo client, uint8_t *in);

    std::string get_username();
    bool is_leader();
    void set_leader(bool val);
private:

    sockaddr_in client_;
    std::string username_;
    bool isLeader_;

    int ip_address_[4];
    int port_;
};

#endif //JAM_CLIENT_INFO_H
