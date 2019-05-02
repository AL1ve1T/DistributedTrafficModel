////////////////////////////////////////////////////////////////////////////////
// node.cpp : Implements the node of the network.
//
//   @uthor: Elnur Alimirzayev,  <elnur.alimirzayev@gmail.com>
//                               <aliveit.elnur@gmail.com>
// 

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
        std::pair<int, int> _pair;
        boost::property_tree::ptree _neighbour = v.second;

        int _id = _neighbour.get<int>("id");
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
    typedef boost::container::map<int, int> map_def;
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
    // by default
    int method = 0;
    boost::property_tree::ptree pt;
    boost::system::error_code error;
    boost::asio::streambuf response;
    try {
        if (!boost::asio::read(_socket, response,
            boost::asio::transfer_at_least(1),error))
            return;
        boost::asio::streambuf::const_buffers_type indata=response.data();
        std::string data(boost::asio::buffers_begin(indata), 
            boost::asio::buffers_end(indata));
        std::stringstream ss;
        ss << data;
        boost::property_tree::read_json(ss, pt);
        method = pt.get<int>("method");
    }
    catch (std::exception& _e) {
        std::cerr << _e.what() << std::endl;
    }
    
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

void Node::dijkstraCalculation(Message& msg) {
    std::string strToSend = msg.encodeString();
    typedef boost::container::map<int, int> map_def;
    boost::asio::io_service _service;
    BOOST_FOREACH(map_def::value_type& v, this->neighbourNodeEndpoints) {
        // Check if message contains current node as visited
        std::vector<int> vis_arr = msg.getVisitedNodes();
        if(std::find(vis_arr.begin(), vis_arr.end(), v.first) != vis_arr.end())
            continue;

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

        std::cout << "Waiting for callback..." << std::endl;

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
        // Find out whether "tag" was updated and if so, update the 
        // existing related path
        boost::container::map<int, int> newTags = incomingMsg.getTags();
        boost::container::map<int, std::vector<int>> newPaths
            = incomingMsg.getPaths();
        boost::container::map<int, int> currentTags = msg.getTags();
        boost::container::map<int, std::vector<int>> currentPaths 
            = msg.getPaths();

        for (auto it = currentTags.begin(); it != currentTags.end(); ++it) {
            if (currentTags.at(it->first) < newTags.at(it->first)) {
                currentTags[it->first] = newTags[it->first];
                newTags.erase(it->first);
                currentPaths[it->first] = newPaths[it->first];
            }
        }
        
        for (auto it = newTags.begin(); it != newTags.end(); ++it) {
            currentTags.insert(std::make_pair(it->first, it->second));
            currentPaths.insert(
                std::make_pair(it->first, newPaths.at(it->first)));
        }
        //////////////////////////////////////////////////////////////////////
        // Now send response back to node
        strToSend = incomingMsg.encodeString();
        _ep = boost::asio::ip::tcp::endpoint(
            boost::asio::ip::address::from_string("127.0.0.1"), v.second + 1000);
        _sock = boost::asio::ip::tcp::socket(_service);
        _sock.connect(_ep);
        write(_sock, boost::asio::buffer(strToSend, strToSend.length()));

        //////////////////////////////////////////////////////////////////////
        // End of code block
        //////////////////////////////////////////////////////////////////////
    }
}

void Node::dijkstraSession(boost::property_tree::ptree& _pt) {
    Message msg(_pt);
    dijkstraCalculation(msg);
}

Node::~Node() {
    this->acceptor.cancel();
}
