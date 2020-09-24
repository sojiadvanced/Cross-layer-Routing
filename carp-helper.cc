
//Required libraries
#include "carp-helper.h"
#include "ns3/carp-routingprotocol.h"
#include "ns3/node-list.h"
#include "ns3/names.h"
#include "ns3/ptr.h"
#include "ns3/ipv4-list-routing.h"

namespace ns3
{

CarpHelper::CarpHelper() : 
  Ipv4RoutingHelper ()
{
  m_agentFactory.SetTypeId ("ns3::carp::RoutingProtocol");
}

CarpHelper* 
CarpHelper::Copy (void) const 
{
  return new CarpHelper (*this); 
}

Ptr<Ipv4RoutingProtocol> 
CarpHelper::Create (Ptr<Node> node) const
{
  Ptr<carp::RoutingProtocol> agent = m_agentFactory.Create<carp::RoutingProtocol> ();
  node->AggregateObject (agent);
  return agent;
}

void 
CarpHelper::Set (std::string name, const AttributeValue &value)
{
  m_agentFactory.Set (name, value);
}

int64_t
CarpHelper::AssignStreams (NodeContainer c, int64_t stream)
{
  int64_t currentStream = stream;
  Ptr<Node> node;
  for (NodeContainer::Iterator i = c.Begin (); i != c.End (); ++i)
    {
      node = (*i);
      Ptr<Ipv4> ipv4 = node->GetObject<Ipv4> ();
      NS_ASSERT_MSG (ipv4, "Ipv4 not installed on node");
      Ptr<Ipv4RoutingProtocol> proto = ipv4->GetRoutingProtocol ();
      NS_ASSERT_MSG (proto, "Ipv4 routing not installed on node");
      Ptr<carp::RoutingProtocol> carp = DynamicCast<carp::RoutingProtocol> (proto);
      if (carp)
        {
          currentStream += carp->AssignStreams (currentStream);
          continue;
        }
      // Carp may also be in a list
      Ptr<Ipv4ListRouting> list = DynamicCast<Ipv4ListRouting> (proto);
      if (list)
        {
          int16_t priority;
          Ptr<Ipv4RoutingProtocol> listProto;
          Ptr<carp::RoutingProtocol> listCarp;
          for (uint32_t i = 0; i < list->GetNRoutingProtocols (); i++)
            {
              listProto = list->GetRoutingProtocol (i, priority);
              listCarp = DynamicCast<carp::RoutingProtocol> (listProto);
              if (listCarp)
                {
                  currentStream += listCarp->AssignStreams (currentStream);
                  break;
                }
            }
        }
    }
  return (currentStream - stream);
}

}
