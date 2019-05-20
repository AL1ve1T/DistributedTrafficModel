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
        std::set<int> visitedNodes;

        // Already known nodes
        std::set<int> knownNodes;

        // Array of paths
        boost::container::map<int, std::vector<int>> paths;
    public:
        // Update JSON tree
        void updateJsonTree();

        // Set label of Node from where message came
        void setMsgCameFrom(int _val);

        // Get label of Node from where message came
        int getMsgCameFrom();

        // Get destination node
        int getDestination();

        // Get start node
        int getStart();

        // Get knownNodes
        std::set<int> getKnownNodes();

        // Set knownNodes
        void setKnownNodes(std::set<int>& _val);

        // Get visitedNodes
        std::set<int> getVisitedNodes();

        // Set visitedNodes
        void setVisitedNodes(std::set<int>& _val);

        // Get shortest paths to nodes
        boost::container::map<int, std::vector<int>>& getPaths();

        // Set shortest paths to nodes
        void setPaths(boost::container::map<int, std::vector<int>>& paths);

        // Get tags from node
        boost::container::map<int, int>& getTags();

        // Set tags to node
        void setTags(boost::container::map<int, int>& tags);

        // Encodes JSON tree into string to send it via network.
        std::string encodeString();

        // Forms Message using existing property tree
        Message(boost::property_tree::ptree& _tree);

        // Forms Message from a string
        Message(std::string _str);

        // Forms an initial Message from start and destination nodes
        Message(int _start, int _end);

        // Common destructor
        ~Message();
};
