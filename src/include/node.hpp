////////////////////////////////////////////////////////////////////////////////
// node.cpp : Defines the node of the network.
//
//   @uthor: Elnur Alimirzayev,  <elnur.alimirzayev@gmail.com>
//                               <aliveit.elnur@gmail.com>
// 

#include <algorithm>
#include "message.hpp"
// Boost includes
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/container/map.hpp>
#include <boost/asio.hpp>

#define STATUS_OK "OK"

////////////////////////////////////////////////////////////////////////////////
// The following enumeration represents method which will be used by this app.
////////////////////////////////////////////////////////////////////////////////
enum Methods { DIJKSTRA = 0 };

////////////////////////////////////////////////////////////////////////////////
// Following code defines class implementing the node of distributed system.
// The Node keeps connection with neighbour nodes and accepts requests from
// them to participate the calculation of the shortest path.
////////////////////////////////////////////////////////////////////////////////
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
        boost::container::map<int, int> neighbourNodes;
        
        // Contains endpoints of neighbour nodes
        // in the following format:
        // <node_label : port>
        boost::container::map<int, int> neighbourNodeEndpoints;

        // Label of current node.
        // Used to determine current node
        // in a distributed system
        int id;

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

        // Reads whole JSON tree and defines the method
        void defineSession(boost::asio::ip::tcp::socket& _socket);

        // Pass message to neighbour node.
        // If message is passed successfully method 
        // returns 'true', otherwise 'false'
        static bool passMessageTo(const boost::system::error_code& ec);
        
        // Sends "echo" message to neighbour nodes
        static void echoSession(boost::asio::ip::tcp::socket& _socket);

        // Method implements Dijkstra's algorithm in the distributed system
        void dijkstraCalculation(Message& msg);

        // Method implements Levit's algorithm in the distributed system
        void levitCalculation(Message& msg);

        // Handles session computing Dijkstra's algorithm
        void dijkstraSession(std::string _data);

        // Handles session computing Levit's algorithm
        void levitSession(std::string _data);

        // Destructor
        ~Node();
}; // end Node