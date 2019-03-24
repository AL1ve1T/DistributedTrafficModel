///////////////////////////////////////////////////////////////
// This file contains definition of Node class and Message 
// struct. See implementation in node.cpp file
//
//   @uthor: Elnur Alimirzayev,  <elnur.alimirzayev@gmail.com>
//                               <aliveit.elnur@gmail.com>
// 

#include <algorithm>
// Boost includes
#include <boost/asio/io_context.hpp>
#include <boost/container/map.hpp>

// Defines a message format which is used 
// in communication between nodes 
struct Message {

};

class Node final {
    private:
        // Contains list of neighbour nodes 
        // in the following format:
        // <node_label : port_on_host_machine> 
        boost::container::map<std::string, std::string> neighbourNodes;

        // Label of current node.
        // Used to determine current node
        // in a distributed system
        std::string label;

        // Port using on a host machine
        // (container port is always :80)
        int port;
    public:
        // Common destructor
        ~Node();

        // Checks if connection with neighbour nodes
        // is establiched and network is ready to pass
        // messages between nodes
        void checkNetwork();

        // Pass message to neighbour node.
        // If message is passed successfully method 
        // returns 'true', else 'false'
        bool passMessageTo();
};