// node.cpp : Defines the node of the network.
//
//   @uthor: Elnur Alimirzayev,  <elnur.alimirzayev@gmail.com>
//                               <aliveit.elnur@gmail.com>
// 
////////////////////////////////////////////////////////////////////////////////

#include <algorithm>
// Boost includes
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/container/map.hpp>
#include <boost/asio.hpp>

#define STATUS_OK "OK"

class Node {
    private:
        // Property tree for JSON parser
        boost::property_tree::ptree pt;

        // IO service from boost::asio lib
        boost::asio::io_context context;

        // IP endpoint
        boost::asio::ip::tcp::endpoint endpoint;

        // Acceptor
        boost::asio::ip::tcp::acceptor acceptor;

        // Contains list of neighbour nodes 
        // in the following format:
        // <node_label : weight_of_edge> 
        boost::container::map<std::string, int> neighbourNodes;
        
        // Contains endpoints of neighbour nodes
        // in the following format:
        // <node_label : port>
        boost::container::map<std::string, int> neighbourNodeEndpoints;

        // Label of current node.
        // Used to determine current node
        // in a distributed system
        std::string id;

        // Port using on a host machine
        // (container port is always :80)
        int port;
    public:
        // Constructor
        Node(const boost::property_tree::ptree& traffic_config, 
            int port, char* id);

        // Accept incoming connections
        void acceptConnections();

        // Checks if connection with neighbour nodes
        // is establiched and network is ready to pass
        // messages between nodes
        void checkNetwork();

        // Pass message to neighbour node.
        // If message is passed successfully method 
        // returns 'true', otherwise 'false'
        static bool passMessageTo(const boost::system::error_code& ec);
        
        // Sends "echo" message to neighbour nodes
        static void echoSession(boost::asio::ip::tcp::socket& _socket);

        // Destructor
        ~Node();
};