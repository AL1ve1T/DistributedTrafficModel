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
        acceptor(service, endpoint) {
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
    boost::asio::ip::tcp::socket sock(service);
    sock.connect(endpoint);
    while (true) {
        boost::shared_ptr<boost::asio::ip::tcp::socket>
            socket(new boost::asio::ip::tcp::socket(service));
        acceptor.async_accept(server_socket, boost::bind(&echoSession));
        client_socket.async_connect(acceptor.local_endpoint(), 
            boost::bind(&noop));
        io_service.run();
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
    boost::asio::streambuf read_buffer;
    std::size_t bytes_transferred;
    bytes_transferred = boost::asio::read(_socket, 
        read_buffer.prepare(bytes_transferred));
    read_buffer.commit(bytes_transferred);      
    std::cout << "Read: " << 
    [] (boost::asio::streambuf& streambuf) -> std::string {
        return {boost::asio::buffers_begin(streambuf.data()),
                 boost::asio::buffers_end(streambuf.data())};
    } << std::endl;
    read_buffer.consume(bytes_transferred);
}

Node::~Node() {
    //
}
