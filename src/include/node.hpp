///////////////////////////////////////////////////////////////
// This file contains definition of Node class. 
// See implementation in node.cpp file
//
//   @uthor: Elnur Alimirzayev,  <elnur.alimirzayev@gmail.com>
//                               <aliveit.elnur@gmail.com>
// 

#include <algorithm>
// Boost includes
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/container/map.hpp>
#include <boost/asio.hpp>

class Node {
    private:
        // Property tree for JSON parser
        boost::property_tree::ptree pt;

        // IO service from boost::asio lib
        boost::asio::io_service service;

        // IP endpoint
        boost::asio::ip::tcp::endpoint endpoint;

        // Acceptor
        boost::asio::ip::tcp::acceptor acceptor;

        // Contains list of neighbour nodes 
        // in the following format:
        // <node_label : port_on_host_machine> 
        boost::container::map
            <std::string, std::string> neighbourNodes;

        // Label of current node.
        // Used to determine current node
        // in a distributed system
        std::string label;

        // Port using on a host machine
        // (container port is always :80)
        int port;
    public:
        // Constructor
        Node(std::string traffic_config);

        // Accept incoming connections
        void acceptConnections();

        // Checks if connection with neighbour nodes
        // is establiched and network is ready to pass
        // messages between nodes
        void checkNetwork();

        // Pass message to neighbour node.
        // If message is passed successfully method 
        // returns 'true', else 'false'
        bool passMessageTo();
        
        // Sends "echo" message to neighbour nodes
        static void echoSession(boost::asio::ip::tcp::socket socket);

        // Destructor
        ~Node();
};