/* This header file defines essential parameters used in developing the Channel-aware routing protocol core module*/

#include "ns3/ipv4-routing-protocol.h"
#include "ns3/node.h"
#include "ns3/ipv4-interface.h"
#include "ns3/ipv4-l3-protocol.h"
#include <map>
#include "ns3/output-stream-wrapper.h"
#include "ns3/random-variable-stream.h"
#include <vector>
#include "ns3/simulator.h"
#include "ns3/timer.h"
#include "ns3/ipv4-address.h"
#include "ns3/callback.h"
#include "ns3/arp-cache.h"




namespace ns3{

namespace carp {

class RoutingProtocol : public Ipv4RoutingProtocol
{

public:
 static TypeId GetTypeId (void);
 static const uint32_t CARP_PORT;
 // Constructor
 RoutingProtocol ();
 virtual ~RoutingProtocol ();

 // Methods inherited from Ipv4RoutingProtocol
 Ptr<Ipv4Route> RouteOutput (Ptr<Packet> p, const Ipv4Header &header, Ptr<NetDevice> oif, Socket::SocketErrno &sockerr); 
 bool RouteInput (Ptr<const Packet> p, const Ipv4Header &header, Ptr<const NetDevice> idev,
                  UnicastForwardCallback ucb, LocalDeliverCallback lcb);
 virtual void NotifyInterfaceUp (uint32_t interface);
 virtual void NotifyInterfaceDown (uint32_t interface);
 virtual void NotifyInterfaceDown (uint32_t interface);
 virtual void NotifyAddAddress (uint32_t interface, Ipv4InterfaceAddress address);
 virtual void NotifyRemoveAddress (uint32_t interface, Ipv4InterfaceAddress address);

 // Set broadcast enable flag
 void SetBroadcastEnable (bool f)
 {
   m_enableBroadcast = f;
 }



private: 
 /* Protocol Parameters */
 Time m_nextHopWait;  // Period of waiting for the neighbor's PONG reply 
 bool m_enableBroadcast;  // Indicates whether a broadcast data packets forwarding 
 uint32_t m_requestId;  // Broadcast ID
 uint32_t m_seqNo; // Request Sequence number

 // IP Protocol 
 Ptr<Ipv4> m_ipv4;
 // Raw unicast socket per each interface, map socket -> iface address (IP + mask)
 std::map<Ptr<Socket>, Ipv4InterfaceAddress>m_socketAddress;
 // Raw subnet directed broadcast socket per each interface (Most probably the broadcast address + mask for each interface)
 std::map<Ptr<Socket>, Ipv4InterfaceAddress>m_socketSubnetBroadcastAddress;

 /* Start Protocol Operation */
 void UpdateRouteToNeighbor (Ipv4Address sender, Ipv4Address receiver); // Update neighbor record (Not sure how important it is )
 bool IsMyOwnAddress (Ipv4Address src); // Test whether the provided address is assigned to an interface


 // Send Methods
 void SendHello (HelloHeader const & helloheader); // Send Hello Packet (Broadcast type)
 void SendPing (PingHeader const & pingheader, Ipv4Address dst); // Send Ping Packet
 void SendPong (PongHeader const & pongheader, Ipv4Address src); // Send Pong packet to sender nodes (All neighbors of source node)
 void SendPacket (Ipv4Address dst, 
 Ptr<UniformRandomVariable> m_uniformRandomVariable; // Provides uniform random variable

 // Receive Control Packets
 void RecvPing (Ptr<Packet> p, PingHeader const &pingheader); // The source information and other packet header information are contained in the header
 void RecvPong (Ptr<Packet> p, PongHeader const &pongheader); // Pong response from neighbors 
 void DataReplyAck (Ipv4Address neighbor); // Ack response from neighbor after successful reception of packets 
 void ProcessHello (Ptr<Packet> p, Ipv4Address receiver);



};

class Neighbors

{

public:

  /**

   * constructor

   * \param delay the delay time for purging the list of neighbors

   */

  Neighbors (Time delay);

  /// Neighbor description
  struct Neighbor
  {
    /// Neighbor IPv4 address
    Ipv4Address m_neighborAddress;
    /// Neighbor MAC address
    Mac48Address m_hardwareAddress;
    /// Neighbor expire time
    Time m_expireTime;
    bool close; // Not sure if this is needed for this scenario

    /**
     * \brief Neighbor structure constructor
     *
     * \param ip Ipv4Address entry
     * \param mac Mac48Address entry
     * \param t Time expire time
     */

    Neighbor (Ipv4Address ip, Mac48Address mac, Time t)
      : m_neighborAddress (ip),
        m_hardwareAddress (mac),
        m_expireTime (t),
        close (false)
    {
    }
  };

  Time GetExpireTime (Ipv4Address addr);
  /**
   * \returns true if the node with IP address is a neighbor
   */
  bool IsNeighbor (Ipv4Address addr);
  /**
   * Update expire time for entry with address addr, if it exists, else add new entry
   */
  void Update (Ipv4Address addr, Time expire);

private:
  Timer m_ntimer;
  std::vector<Neighbor>m_nb;
  Mac48Address LookupMacAddress (Ipv44Address addr);


}; // End of class Neighbors

} // End of carp namespace
} // End of ns3 namespace 

