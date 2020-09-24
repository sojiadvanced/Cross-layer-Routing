/* Header file for Channel-aware Routing Protocol */

#include <vector>
#include <list>
#include <ostream>
#include "ns3/header.h"
#include "ns3/ipv4-address.h"
#include "ns3/carp-header.h"


namespace ns3
{
namespace carp
{

NS_OBJECT_ENSURE_REGISTERED (TypeHeader);

TypeHeader::TypeHeader (MessageType t) :
  m_type (t), m_valid (true)
{
}


TypeId
TypeHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::carp::TypeHeader")
    .SetParent<Header> ()
    .SetGroupName("Carp")
    .AddConstructor<TypeHeader> ()
  ;
  return tid;
}

TypeId
TypeHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

uint32_t
TypeHeader::GetSerializedSize () const
{
  return 1;   // Why is 1 the serialized size
}

void
TypeHeader::Serialize (Buffer::Iterator i) const
{
  i.WriteU8 ((uint8_t) m_type);
}

uint32_t
TypeHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  uint8_t type = i.ReadU8 ();
  m_valid = true;
  switch (type)
    {
   case CARPTYPE_PING:   
   case CARPTYPE_PONG: 
   case CARPTYPE_HELLO:  
   case CARPTYPE_DATA_ACK:
      {
        m_type = (MessageType) type;
        break;
      }
    default:
      m_valid = false;
    }
  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == GetSerializedSize ());
  return dist;
}

void
TypeHeader::Print (std::ostream &os) const
{
  switch (m_type)
    {
    case CARPTYPE_PING:
      {
        os << "PING";
        break;
      }
    case CARPTYPE_PONG:
      {
        os << "PONG";
        break;
      }
    case CARPTYPE_HELLO:
      {
        os << "HELLO";
        break;
      }
    case CARPTYPE_DATA_ACK:
      {
        os << "DATA_ACK";
        break;
      }
    default:
      os << "UNKNOWN_TYPE";
    }
}

bool
TypeHeader::operator== (TypeHeader const & o) const
{
  return (m_type == o.m_type && m_valid == o.m_valid);
}

std::ostream &
operator<< (std::ostream & os, TypeHeader const & h)
{
  h.Print (os);
  return os;
}


//-----------------------------------------------------------------------------
// PING
//-----------------------------------------------------------------------------
// A chained constructor to declare the default fields in the Ping headers
PingHeader::PingHeader (uint32_t num_pkt, Ipv4Address origin) :
  m_num_pkt (num_pkt), m_origin (origin)
{
}

NS_OBJECT_ENSURE_REGISTERED (PingHeader);

//The use Of typeid to define the PingHeader information
TypeId
PingHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::carp::PingHeader")
    .SetParent<Header> ()
    .SetGroupName("Carp")
    .AddConstructor<PingHeader> ()
  ;
  return tid;
}

TypeId
PingHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

uint32_t
PingHeader::GetSerializedSize () const
{
  return 23;   //What is the seriaLized sIze of a PING packet?
}

// Serialize the PING header
void
PingHeader::Serialize (Buffer::Iterator i) const
{
  i.WriteU8 (m_num_pkt);
  WriteTo (i, m_origin);
  
}

uint32_t
PingHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  m_num_pkt = i.ReadU8 ();
  ReadFrom (i, m_origin);

  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == GetSerializedSize ());
  return dist;
}

void
PingHeader::Print (std::ostream &os) const
{
  os << " source: ipv4 "<< m_origin << " Number of packets " << m_num_pkt;

}

std::ostream &
operator<< (std::ostream & os, PingHeader const & h)
{
  h.Print (os);
  return os;
}

bool
PingHeader::operator== (PingHeader const & o) const
{
  return (m_num_pkt == o.m_num_pkt && m_origin == o.m_origin );
}


//-----------------------------------------------------------------------------
// PONG
//-----------------------------------------------------------------------------

PongHeader::PongHeader (uint8_t queue, uint8_t hopCount,
 Ipv4Address dst, Ipv4Address origin,  double energy, double linkQuality) :
  m_queue (queue), m_origin (origin), m_hopCount(hopCount),
  m_energy(energy), m_linkQuality(linkQuality), m_dst(dst)
{
}

NS_OBJECT_ENSURE_REGISTERED (PingHeader);

//The use Of typeid to define the PingHeader information
TypeId
PongHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::carp::PongHeader")
    .SetParent<Header> ()
    .SetGroupName("Carp")
    .AddConstructor<PongHeader> ()
  ;
  return tid;
}

TypeId
PongHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

uint32_t
PongHeader::GetSerializedSize () const
{
  return 23;   //What is the seriaLized sIze of a PING packet?
}

// Serialize the PING header
void
PongHeader::Serialize (Buffer::Iterator i) const
{
  i.WriteU8 (m_queue);
  i.WriteU8 (m_energy);
  i.WriteU8 (m_hopCount);
  i.WriteU8 (m_linkQuality);
  WriteTo (i, m_origin);
  WriteTo (i, m_dst);
  
}

uint32_t
PongHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  m_queue = i.ReadU8 ();
  m_energy = i.ReadU8 ();
  i.WriteU8 (m_hopCount);
  i.WriteU8 (m_linkQuality);
  ReadFrom (i, m_origin);
  ReadFrom (i, m_dst);

  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == GetSerializedSize ());
  return dist;
}

void
PongHeader::Print (std::ostream &os) const
{
  os << " source: ipv4 "<< m_origin << " Residual energy " << m_energy
     << " dest ipv4 " << m_dst << " hop count "<< m_hopCount 
     << " link quality " << m_linkQuality << " Buffer size " << m_queue;

}

std::ostream &
operator<< (std::ostream & os, PongHeader const & h)
{
  h.Print (os);
  return os;
}

bool
PongHeader::operator== (PongHeader const & o) const
{
  return (m_origin == o.m_origin && m_dst == o.m_dst && m_energy == o.m_energy
         && m_hopCount == o.m_hopCount && m_queue == o.m_queue);
}


//-----------------------------------------------------------------------------
// HELLO
//-----------------------------------------------------------------------------
// A chained constructor to declare the default fields in the Hello header
HelloHeader::HelloHeader (uint32_t hopCount, Ipv4Address origin) :
  m_hopCount (hopCount), m_origin (origin)
{
}

NS_OBJECT_ENSURE_REGISTERED (PingHeader);

//The use Of typeid to define the PingHeader information
TypeId
HelloHeader::GetTypeId ()
{
  static TypeId tid = TypeId ("ns3::carp::HelloHeader")
    .SetParent<Header> ()
    .SetGroupName("Carp")
    .AddConstructor<HelloHeader> ()
  ;
  return tid;
}

TypeId
HelloHeader::GetInstanceTypeId () const
{
  return GetTypeId ();
}

uint32_t
HelloHeader::GetSerializedSize () const
{
  return 23;   //What is the seriaLized size of a Hello header?
}

// Serialize the PING header
void
HelloHeader::Serialize (Buffer::Iterator i) const
{
  i.WriteU8 (m_hopCount);
  WriteTo (i, m_origin);
  
}

uint32_t
HelloHeader::Deserialize (Buffer::Iterator start)
{
  Buffer::Iterator i = start;
  m_hopCount = i.ReadU8 ();
  ReadFrom (i, m_origin);

  uint32_t dist = i.GetDistanceFrom (start);
  NS_ASSERT (dist == GetSerializedSize ());
  return dist;
}

void
HelloHeader::Print (std::ostream &os) const
{
  os << " source: ipv4 "<< m_origin << " Hop Count " << m_hopCount;

}

std::ostream &
operator<< (std::ostream & os, HelloHeader const & h)
{
  h.Print (os);
  return os;
}

bool
HelloHeader::operator== (HelloHeader const & o) const
{
  return (m_hopCount == o.m_hopCount && m_origin == o.m_origin );
}



} // END of Carp
} //END of namespace
