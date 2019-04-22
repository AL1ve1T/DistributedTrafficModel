// node.cpp : Implements the node of the network.
//
//   @uthor: Elnur Alimirzayev,  <elnur.alimirzayev@gmail.com>
//                               <aliveit.elnur@gmail.com>
// 
////////////////////////////////////////////////////////////////////////////////

#include "include/node.hpp"
#include <iostream>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

Node::Node(const boost::property_tree::ptree& traffic_config, 
        int port, char* id) :
        endpoint(boost::asio::ip::address::from_string("127.0.0.1"), port),
        acceptor(context, endpoint) {
    boost::property_tree::ptree curr_node = traffic_config.get_child(id);
    boost::property_tree::ptree neighbours = curr_node.get_child("neighbours");
    this->port = port;
    this->id = id;
    
    BOOST_FOREACH(boost::property_tree::ptree::value_type& v, neighbours) {
        std::pair<std::string, int> _pair;
        boost::property_tree::ptree _neighbour = v.second;

        std::string _id = _neighbour.get_value<std::string>("id");
        int _weight = _neighbour.get<int>("weight");
        int _port   = _neighbour.get<int>("port");

        _pair = std::make_pair(_id, _weight);
        this->neighbourNodes.insert(_pair);

        _pair = std::make_pair(_id, _port);
        this->neighbourNodeEndpoints.insert(_pair);
    }
}

void Node::acceptConnections() {
    using boost::asio::ip::tcp;
    while (true) {
        tcp::socket socket(context);
        acceptor.accept(socket);
        echoSession(socket);
        boost::system::error_code ignored_error;
        boost::asio::write(socket, boost::asio::buffer(STATUS_OK), 
            ignored_error);
    }
}

void Node::checkNetwork() {
    std::string echo = "echo";
    typedef boost::container::map<std::string, int> map_def;
    boost::asio::io_service _service;
    BOOST_FOREACH(map_def::value_type& v, this->neighbourNodeEndpoints) {
        boost::asio::ip::tcp::endpoint _ep(
            boost::asio::ip::address::from_string("127.0.0.1"), v.second);
        boost::asio::ip::tcp::socket _sock(_service);
        _sock.connect(_ep);
        write(_sock, boost::asio::buffer(echo, echo.length()));
    }
}

bool Node::passMessageTo(const boost::system::error_code& ec) {
    /*if (ec) {
        std::cerr << "FAILED" << std::endl;
        return false;
    }
    */
}

void Node::echoSession(boost::asio::ip::tcp::socket& _socket) {
    boost::system::error_code error;
    boost::asio::streambuf response;
    if (!boost::asio::read(_socket, response,
        boost::asio::transfer_at_least(1),error))
        return;
    boost::asio::streambuf::const_buffers_type indata=response.data();
    std::string data(boost::asio::buffers_begin(indata), 
        boost::asio::buffers_end(indata));
    ////////////////////////////////////////////////////////////////
    // Here begins processing
    std::cout << data << std::endl;
}

Node::~Node() {
    //
}
