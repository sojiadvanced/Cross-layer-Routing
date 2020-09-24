/* This main file defines essential parameters used in developing the Channel-aware routing protocol core module*/

#include "carp-routing-protocol.h"
#include "ns3/log.h"
#include "ns3/random-variable-stream.h"
#include "ns3/inet-socket-address.h"
#include "ns3/udp-header.h"
#include "ns3/string.h"
#include "ns3/pointer.h"
#include <algorithm>
#include <limits>


namespace ns3 {

NS_LOG_COMPONENT_DEFINE ("CarpRoutingProtocol");

namespace carp {

------------------------------------------------------------------
// Routing Protocol Implementation
------------------------------------------------------------------
RoutingProtocol::RoutingProtocol ()
  : m_nextHopWait (MilliSeconds (10)),
    m_requestId (0),
    m_seqNo (0),
    m_enableBroadcast (true)

{
}


TypedId RoutingProtocol::GetTypeId (void)
{
  static TypeId tid = TypeId ("ns3::carp::RoutingProtocol")
   .SetParent<Ipv4RoutingProtocol>()
   .SetGroupName ("Carp")
   .AddConstructor<RoutingProtocol> ()
   .AddAttribute("PingWaitTime", "Period of waiting for neighbor's to reply with a PONG packet", 
                 TimeValue (MilliSeconds (10)), 
 		 MakeTimeAccessor (&RoutingProtocol::m_nextHopWait), 
 		 MakeTimeChecker ())
   ;


   return tid; 
}

// This is to confirm if the address is a neighbor of a node
bool
Neighbors::IsNeighbor (Ipv4Address addr)
{
  //Purge ();
  for (std::vector<Neighbor>::const_iterator i = m_nb.begin ();
       i != m_nb.end (); ++i)
    {
      if (i->m_neighborAddress == addr)
        {
         return true;
        }
    }
  return false;
}

// Time it takes the neighbor to expire
Time
Neighbors::GetExpireTime (Ipv4Address addr)
{
  // Purge ();
  for (std::vector<Neighbor>::const_iterator i = m_nb.begin (); i
       != m_nb.end (); ++i)
    {
      if (i->m_neighborAddress == addr)
        {
          return (i->m_expireTime - Simulator::Now ());
        }
    }
  return Seconds (0);
}

// Update the neighbor status
void
Neighbors::Update (Ipv4Address addr, Time expire)
{
  for (std::vector<Neighbor>::iterator i = m_nb.begin (); i != m_nb.end (); ++i)
    {
      if (i->m_neighborAddress == addr)
        {
          i->m_expireTime
            = std::max (expire + Simulator::Now (), i->m_expireTime);
          if (i->m_hardwareAddress == Mac48Address ())
            {
               i->m_hardwareAddress = LookupMacAddress (i->m_neighborAddress);
            }
          return;
        }
    }
  // NS_LOG_LOGIC ("Open link to " << addr);
  Neighbor neighbor (addr, LookupMacAddress (addr), expire + Simulator::Now ());
  m_nb.push_back (neighbor);
  // Purge ();
}

// The use of SendTo module to send streams of data
void 
RoutingProtocol::SendTo(Ptr<Socket> socket, Ptr<Packet> packet, Ipv4Address dest)
{
  socket->SendTo(packet, 0, InetSocketAddress(dest, CARP_PORT);
}

// Method used by the sink to broadcast the hello packet
void RoutingProtocol::SendHello ()
{
  // The need to revisit the use of socket & Ipv4InterfaceAddress are required

 for (std::map<Ptr<Socket>, Ipv4InterfaceAddress>::const_iterator j = m_socketAddress.begin (); m_socketAddress.end(); ++j)
 {
  Ptr<Socket> socket = j->first;
  Ipv4InterfaceAddress iface = j->second;

  HelloHeader helloHeader (/*hopCount*/ 0, /*Origin*/ iface.GetLocal ());
  Ptr<Packet> packet = Create<Packet> (); // You can create packet size here //
  packet->AddHeader (helloHeader);
  TypeHeader tHeader (CARPTYPE_HELLO);
  packet->AddHeader (tHeader); // You can also add the TTL tag

  Ipv4Address destination;
  if (iface.GetMask () == Ipv4Mask::GetOnes ())
  {
    destination = Ipv4Address ("255.255.255.255");
  }
  else
  {
    destination = iface.GetBroadcast ();
  }
 }
  Time jitter = Time (MilliSeconds (m_uniformRandomVariable->GetInteger (0,10));
  Simulator::Schedule (jitter, &RoutingProtocol::SendTo, this, socket, packet, destination);
  
}

// How nodes manage packets with hello headers
void RoutingProtocol::ProcessHello (Ptr<Packet> p, Ipv4Address receiver)
{
 Helloheader helloheader;
 p->RemoveHeader(helloheader); 
 Ipv4Address src = helloheader.GetOrigin ();
 if (dst == Ipv4Address("255.255.255.255")) // If it is a broadcast message ?
 {
 // A for loop might be needed somewhere, also the timer might be unnecessary given the assumption of nodes remaining in a static position for a while
  Neighbor::Update(src, timer);
  uint8_t hop = helloheader.GetHopCount () + 1;
  helloheader.SetHopCount (hop);
  Ipv4Address recv_addr = m_ipv4->GetAddress(m_ipv4->GetInterfaceForAddress(receiver),0);
  helloheader.SetOrigin (recv_addr);
  p->AddHeader (helloheader);
  // NodeDevice store; //Maybe a struct data type
  Ptr<NetDevice> dev = m_ipv4->GetNetDevice(m_ipv4->GetInterfaceAddress(receiver));
  Ipv4Address recv_addr = m_ipv4->GetAddress(m_ipv4->GetInterfaceForAddress(receiver),0);

  // Store (dev, recv_addr, hop) // This stores the IP address, hop count for every node in the topology
  // SendHello (); // During the Hello broadcast  period, every receiving node re-propagates the HELLO message
  return;
 }
 // The need to create a node store which keeps track of the hop count status of each node from the sink per every HELLO packet received
}

// Method to initiate PING, PONG, PACKET FORWARDING
Ptr<Ipv4Route>
RoutingProtocol::RouteOutput (Ptr<Packet> p, const Ipv4Header &header, Ptr<NetDevice> oif, Socket::SocketErrno &sockerr)
{
  // Condition if no defined packet 
  
  // Condition if socket address is empty 
  if (m_socketAddresses.empty ())
  {
   sockerr = Socket::ERROR_NOROUTETOHOST;
   Ptr<Ipv4Route> route;
   return route;
  }
  // sockerr = Socket::ERROR_NOTERROR; // Not sure what this is meant for
  Ipv4Address dst = header.GetDestination ();

  // Invoke Ping, Pong, Relay Selection & Route Returned
}

bool
RoutingProtocol::RouteInput (Ptr<const Packet> p, const Ipv4Header &header,
			     Ptr<const NetDevice> idev, UnicastForwardCallback ucb,
			     LocalDeliverCallback lcb, ErrorCallback ecb)

{
 if (m_socketAddress.empty())
 {
  NS_LOG_LOGIC (" No Carp interfaces ");
  return false;
 }
 
 int32_t iif = m_ipv4->GetInterfaceForDevice (idev); // Address of the node's receiving interface 
 Ipv4Address dst = header.GetDestination ();
 Ipv4Address origin = header.GetSource ();

 // Checks if duplicate packet is being sent 
 if (IsMyOwnAddress (origin) )
 {
  return true;
 }

 // Unicast local delivery 
 if (m_ipv4->IsDestinationAddress (dst, iif)
 {
   if (lcb.IsNull () == false) // This delivers the packet to the node when the local callback is not null
   {
     NS_LOG_LOGIC ("Unicast local delivery to " << dst );
     lcb (p, header, iif);
   }
   else
   {
     NS_LOG_ERROR ("Unable to deliver due to null callback" << p->GetUid () << "from" << origin );
     ecb (p, header, Socket::ERROR_NOROUTETOHOST); 
   }
   return true;
 }

 // ?? Unsure about the need to check if the device supports forwarding 
 

 /* Forward packet || This transmits the packet to the destination node once it is not meant for the local node ||
 It is important to define the PING, PONG & RELAY SELECTION in the forwarding method */
 return Forwarding (p, header, ucb, ecb);	
 
}

bool
RoutingProtocol::Forwarding (Ptr<const Packet> p, const Ipv4Header & header,
			     UnicastForwardCallback ucb, ErrorCallback ecb)
{
 Ipv4Address dst = header.GetDestination ();
 Ipv4Address origin = header.GetSource ();

 // Think about the logic for this

}


} // End of carp namespace
} // End of ns3 namespace
