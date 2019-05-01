////////////////////////////////////////////////////////////////////////////////
// node.cpp : Implements the node of the network.
//
//   @uthor: Elnur Alimirzayev,  <elnur.alimirzayev@gmail.com>
//                               <aliveit.elnur@gmail.com>
// 

#include "include/node.hpp"
#include "include/message.hpp"
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
        defineSession(socket);
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

void Node::defineSession(boost::asio::ip::tcp::socket& _socket) {
    boost::system::error_code error;
    boost::asio::streambuf response;
    if (!boost::asio::read(_socket, response,
        boost::asio::transfer_at_least(1),error))
        return;
    boost::asio::streambuf::const_buffers_type indata=response.data();
    std::string data(boost::asio::buffers_begin(indata), 
        boost::asio::buffers_end(indata));
    boost::property_tree::ptree pt;
    boost::property_tree::read_json(data, pt);
    int method = pt.get<int>("method");
    
    switch (method) {
        case DIJKSTRA:
            this->dijkstraSession(pt);
        default:
            std::cout << "Method not recognized" << std::endl;
            return;
    }
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
    std::cout << data << std::endl;
}

void Node::dijkstraCalculation(int _start, int _end) {
    Message msg(_start, _end);
    std::string strToSend = msg.encodeString();
    typedef boost::container::map<std::string, int> map_def;
    boost::asio::io_service _service;
    BOOST_FOREACH(map_def::value_type& v, this->neighbourNodeEndpoints) {
        boost::asio::ip::tcp::endpoint _ep(
            boost::asio::ip::address::from_string("127.0.0.1"), v.second);
        boost::asio::ip::tcp::socket _sock(_service);
        _sock.connect(_ep);
        write(_sock, boost::asio::buffer(strToSend, strToSend.length()));
        //////////////////////////////////////////////////////////////////////
        // The following code is waiting for node to handle response /////////
        //////////////////////////////////////////////////////////////////////
        boost::asio::io_service io_service;
        boost::asio::ip::tcp::endpoint ep(
            boost::asio::ip::address::from_string("127.0.0.1"), 
            this->port + 1000);
        boost::asio::ip::tcp::acceptor acceptor(io_service, ep);

        std::cout << "Waiting for connection..." << std::endl;

        boost::asio::ip::tcp::socket socket(io_service);
        acceptor.accept(socket);

        boost::system::error_code error;
        boost::asio::streambuf response;
        if (!boost::asio::read(socket, response,
            boost::asio::transfer_at_least(1),error))
            return;
        boost::asio::streambuf::const_buffers_type indata=response.data();
        std::string data(boost::asio::buffers_begin(indata), 
            boost::asio::buffers_end(indata));
        
        Message incomingMsg(data);
        //////////////////////////////////////////////////////////////////
        // Check for new known nodes
        std::vector<int> newKnownNodes = incomingMsg.getKnownNodes();
        std::vector<int> oldKnownNodes = msg.getKnownNodes();
        std::sort(newKnownNodes.begin(), newKnownNodes.end());
        std::sort(oldKnownNodes.begin(), oldKnownNodes.end());
        if (newKnownNodes != oldKnownNodes) msg.setKnownNodes(newKnownNodes);

        //////////////////////////////////////////////////////////////////
        // Check for new visited nodes
        std::vector<int> newVisitedNodes = incomingMsg.getVisitedNodes();
        std::vector<int> oldVisitedNodes = msg.getVisitedNodes();
        std::sort(newVisitedNodes.begin(), newKnownNodes.end());
        std::sort(oldVisitedNodes.begin(), oldVisitedNodes.end());
        if (newVisitedNodes != oldVisitedNodes) 
            msg.setVisitedNodes(newVisitedNodes);

        //////////////////////////////////////////////////////////////////
        // 
        boost::container::map<int, int> newTags = incomingMsg.getTags();
        boost::container::map<int, std::vector<int>> newPaths
            = incomingMsg.getPaths();
        //////////////////////////////////////////////////////////////////////
        //////////////////////////////////////////////////////////////////////
    }
}

void Node::dijkstraSession(boost::property_tree::ptree& _pt) {
    //
}

Node::~Node() {
    this->acceptor.cancel();
}
