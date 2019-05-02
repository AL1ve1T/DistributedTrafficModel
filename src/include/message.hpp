///////////////////////////////////////////////////////////////////////////////
// This file contains definition of Message class.
// See implementation in node.cpp file
//
//   @uthor: Elnur Alimirzayev,  <elnur.alimirzayev@gmail.com>
//                               <aliveit.elnur@gmail.com>
// 

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/container/map.hpp>

///////////////////////////////////////////////////////////////////////////////
// The following code contains definition of class Message which is used
// to transport messages about computing state of distributed algorithm.
///////////////////////////////////////////////////////////////////////////////
class Message {
    private:
        // Core field of the message
        boost::property_tree::ptree jsonTree;

        // Method (used for enumeration)
        int method;

        // Start node
        int startNode;

        // Destination node
        int destinationNode;

        // "Tags" representing the shortest ways to nodes
        boost::container::map<int, int> tags;

        // Already visited nodes
        std::vector<int> visitedNodes;

        // Already known nodes
        std::vector<int> knownNodes;

        // Array of paths
        boost::container::map<int, std::vector<int>> paths;
    public:
        // Get destination node
        int getDestination();

        // Get start node
        int getStart();

        // Get knownNodes
        std::vector<int> getKnownNodes();

        // Set knownNodes
        void setKnownNodes(std::vector<int>& _val);

        // Get visitedNodes
        std::vector<int> getVisitedNodes();

        // Set visitedNodes
        void setVisitedNodes(std::vector<int>& _val);

        // Get shortest paths to nodes
        boost::container::map<int, std::vector<int>> getPaths();

        // Get tags from node
        boost::container::map<int, int> getTags();

        // Encodes JSON tree into string to send it via network.
        std::string encodeString();

        // Forms Message using existing property tree
        Message(const boost::property_tree::ptree& _tree);

        // Forms Message from a string
        Message(const std::string _str);

        // Forms an initial Message from start and destination nodes
        Message(int _start, int _end);

        // Common destructor
        ~Message();
};
