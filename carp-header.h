/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include <vector>
#include <list>
#include <ostream>
#include "ns3/header.h"
#include "ns3/ipv4-address.h"

namespace ns3 {
namespace carp {

enum MessageType
{
  CARPTYPE_PING  = 1,   
  CARPTYPE_PONG  = 2,
  CARPTYPE_HELLO = 3,   
  CARPTYPE_DATA_ACK = 4    // This kind of message type would be investigated subsequently
};

//Description and use cases of Type Header

class TypeHeader : public Header
{
public:

  TypeHeader (MessageType t = CARPTYPE_PING);

  // Header serialization/deserialization
  static TypeId GetTypeId ();
  TypeId GetInstanceTypeId () const;
  uint32_t GetSerializedSize () const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
  void Print (std::ostream &os) const;

  /// Return type
  MessageType Get () const { return m_type; }
  /// Check that type if valid
  bool IsValid () const { return m_valid; }
  //bool operator== (TypeHeader const & o) const;
private:
  MessageType m_type;
  bool m_valid;
};


/* This class is used to define the required fields for the PING packet */
class PingHeader : public Header 
{
public:
  PingHeader (uint32_t num_pkt = 0, Ipv4Address origin = Ipv4Address ());

  // Header serialization/deserialization
  static TypeId GetTypeId ();
  TypeId GetInstanceTypeId () const;
  uint32_t GetSerializedSize () const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
  void Print (std::ostream &os) const;

  // Fields
  void PacketCount (uint32_t num_pkt) { m_num_pkt = num_pkt; }
  uint32_t GetPacketCount () const { return m_requestID; }
  void SetOrigin (Ipv4Address a) { m_origin = a; }
  Ipv4Address GetOrigin () const { return m_origin; }
  

  bool operator== (PingHeader const & o) const;
private:
  uint32_t       m_num_pkt;      ///< Number of pkt to be sent by sending node
  Ipv4Address    m_origin;         ///< Originator IP Address

};

std::ostream & operator<< (std::ostream & os, PingHeader const &);

/**

* \brief PONG (PONG) Message Format
  
  0                   1                   2                   3
  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |          |Lq|    Queue     |Energy|   Hop Count   |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |                     Destination IP address                    |
  +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
  |                    Originator IP address                      |
  
  \endverbatim
*/


class PongHeader : public Header
{
public:
  PongHeader (uint8_t queue = 0, uint8_t hopCount = 0, Ipv4Address dst =
                Ipv4Address (), Ipv4Address origin =Ipv4Address (), double energy = 0.0
               double linkQualIty = 0.0);
  // Header serialization/deserialization
  static TypeId GetTypeId ();
  TypeId GetInstanceTypeId () const;
  uint32_t GetSerializedSize () const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
  void Print (std::ostream &os) const;

  // Fields
  void SetHopCount (uint8_t count) { m_hopCount = count; }
  uint8_t GetHopCount () const { return m_hopCount; }
  void SetDst (Ipv4Address a) { m_dst = a; }
  Ipv4Address GetDst () const { return m_dst; }
  void SetOrigin (Ipv4Address a) { m_origin = a; }
  Ipv4Address GetOrigin () const { return m_origin; }
  void SetQueue (uint8_t queue);
  uint8_t GetQueue () const{return m_queue; };
  void SetEnergy (double energy);
  double GetEnergy () const{return m_energy; };
  void SetLinkQuality (double linkQuality);
  double GetLinkQuality () const{return m_linkQuality; };
        

  bool operator== (PongHeader const & o) const;
private:

  uint8_t       m_queue;         ///< Buffer Size
  uint8_t       m_hopCount;         ///< Hop Count
  Ipv4Address   m_dst;              ///< Destination IP Address
  Ipv4Address   m_origin;           ///< Source IP Address
  double        m_energy;
  double        m_linkQuality;
};

std::ostream & operator<< (std::ostream & os, PongHeader const &);


class HelloHeader : public Header 
{
public:
  HelloHeader (uint32_t hopCount = 0, Ipv4Address origin = Ipv4Address ());

  // Header serialization/deserialization
  static TypeId GetTypeId ();
  TypeId GetInstanceTypeId () const;
  uint32_t GetSerializedSize () const;
  void Serialize (Buffer::Iterator start) const;
  uint32_t Deserialize (Buffer::Iterator start);
  void Print (std::ostream &os) const;

  // Fields
  void HopCount (uint32_t hopCount) { m_hopCount = hopCount; }
  uint32_t GetHopCount () const { return m_hopCount; }
  void SetOrigin (Ipv4Address a) { m_origin = a; }
  Ipv4Address GetOrigin () const { return m_origin; }
  
  // Method to invoke the Hello packet
  void SetHello (Ipv4Address src, uint32_t srcSeqNo);

  bool operator== (HelloHeader const & o) const;
private:
  uint32_t       m_hopCount;      ///< Hop count of node from sink
  Ipv4Address    m_origin;         ///< Originator IP Address

};

std::ostream & operator<< (std::ostream & os, PingHeader const &);



}

}
