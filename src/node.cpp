//
//   @uthor: Elnur Alimirzayev,  <elnur.alimirzayev@gmail.com>
//                               <aliveit.elnur@gmail.com>
// 

#include "include/node.hpp"
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

Node::Node(std::string traffic_config) :
    endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 8000),
    acceptor(service, endpoint) {
        //
}

void Node::acceptConnections() {
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
