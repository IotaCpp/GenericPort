#ifndef NEIGHBOR_H_
#define NEIGHBOR_H_

#include <string>
using std::string;
#include <sstream>
using std::ostringstream;
#include <vector>
using std::vector;

class Neighbor
{
public:
    using SocketAddress = int;
    using DatagramPacket = vector<char>;

public:

    explicit Neighbor (const SocketAddress& address) :
        m_address(address)
    {}

    void send (const DatagramPacket& packet)
    {
        try
        {
            // TODO:
            // packet.setSocketAddress(address);
            // Node.socket.send(packet);
        }
        catch (...)
        {
        }
    }

    bool operator== (const Neighbor& obj) const
    {
        return (m_address == obj.m_address);
    }

    int hashCode (void) const
    {
        // TODO: return m_address.hashCode();
        return 0;
    }

    string toString (void) const
    {
        ostringstream ss;
        ss << "\"address\": \"" << m_address << "\""
                << ", \"numberOfAllTransactions\": " << m_numberOfAllTransactions
                << ", \"numberOfNewTransactions\": " << m_numberOfNewTransactions
                << ", \"numberOfInvalidTransactions\": " << m_numberOfInvalidTransactions;
        return ss.str();
    }

private:
    SocketAddress m_address;
    int m_numberOfAllTransactions;
    int m_numberOfNewTransactions;
    int m_numberOfInvalidTransactions;
};

#endif
