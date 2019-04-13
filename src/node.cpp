// node.cpp : Implements the node of the network.
//
//   @uthor: Elnur Alimirzayev,  <elnur.alimirzayev@gmail.com>
//                               <aliveit.elnur@gmail.com>
// 
////////////////////////////////////////////////////////////////////////////////

#include "include/node.hpp"
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

Node::Node(const boost::property_tree::ptree& traffic_config, 
        int port, char* id) :
        endpoint(boost::asio::ip::address::from_string("127.0.0.1"), port),
        acceptor(service, endpoint) {
    boost::property_tree::ptree curr_node = traffic_config.get_child(id);
    this->port = curr_node.get<int>("port");
    
    BOOST_FOREACH(boost::property_tree::ptree::value_type& v, curr_node) {
        std::pair<std::string, int> _pair;
        int _weight = v.second.get("weight", INT_MAX);
        _pair = std::make_pair(v.first, _weight);
        this->neighbourNodes.insert(_pair);
    }
}

void Node::acceptConnections() {
    boost::asio::ip::tcp::socket sock(service);
    sock.connect(endpoint);
    while (true) {
        boost::shared_ptr<boost::asio::ip::tcp::socket>
            socket(new boost::asio::ip::tcp::socket(service));
        acceptor.accept(*socket);
        boost::bind(echoSession, socket);
    }
}

void Node::checkNetwork() {

}

bool Node::passMessageTo() {
    
}

void Node::echoSession(boost::asio::ip::tcp::socket socket) {
    std::string echo = "echo";

}

Node::~Node() {
    //
}
