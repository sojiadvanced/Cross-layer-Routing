/* Helper module used to install CARP on wireless sensor nodes */

#ifndef CARP_HELPER_H
#define CARP_HELPER_H

//Required libraries
#include "ns3/object-factory.h"
#include "ns3/node.h"
#include "ns3/node-container.h"
#include "ns3/ipv4-routing-helper.h"
#include <map>
#include <set>

namespace ns3{

/* Class that adds CARP routing to nodes
It is expected to be used with ns3::InternetStackHelper::SetRoutingHelper */
class CarpHelper : public Ipv4RoutingHelper
{
public:
	//Create a CARP Constructor
	CarpHelper ();
	/* Returns pointer to clone of the CarpHelper*/
	CarpHelper* Copy () const;

	/* 
	* \param node, the node on which the routing protocol will run
	* The method is run by ns3::InternetStackHelper::Install
	*/
	virtual Ptr<Ipv4RoutingProtocol> Create (Ptr<Node> node) const;
	
	/* This method controls the attributes ns3::carp::RoutingProtocol*/
	void Set (std::string name, const AttributeValue &value);
	
	/* return the number of stream indices assigned by this helper*/
	int64_t AssignStreams (NodeContainer c, int64_t stream);

private:
	ObjectFactory m_agentFactory; 
};

} //namespace ns3
#endif 
