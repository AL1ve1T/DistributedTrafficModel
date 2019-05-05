///////////////////////////////////////////////////////////////////////////////
// This file contains implementation of Message class.
// See definition in node.hpp file
//
//   @uthor: Elnur Alimirzayev,  <elnur.alimirzayev@gmail.com>
//                               <aliveit.elnur@gmail.com>
// 

#include "include/message.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>

////////////////////////////////////////////////////////////////////////////////
// Helper functions
template <typename T>
std::set<T> as_set(boost::property_tree::ptree const& pt, 
        boost::property_tree::ptree::key_type const& key)
{
    std::set<T> r;
    for (auto& item : pt.get_child(key))
        r.insert(item.second.get_value<T>());
    return r;
}

template <typename T>
std::vector<T> as_vector(boost::property_tree::ptree const& pt, 
        boost::property_tree::ptree::key_type const& key)
{
    std::vector<T> r;
    for (auto& item : pt.get_child(key))
        r.push_back(item.second.get_value<T>());
    return r;
}
////////////////////////////////////////////////////////////////////////////////

Message::Message(const std::string _str) {
    boost::property_tree::read_json(_str.c_str(), this->jsonTree);
    startNode       = jsonTree.get<int>("start");
    destinationNode = jsonTree.get<int>("end");
    method          = jsonTree.get<int>("method");
    visitedNodes    = as_set<int>(jsonTree, "visitedNodes");
    knownNodes      = as_set<int>(jsonTree, "knownNodes");

    boost::property_tree::ptree _tags = jsonTree.get_child("tags");

    BOOST_FOREACH(const boost::property_tree::ptree::value_type &v, 
            jsonTree.get_child("tags")) {
        int _label = atoi(v.first.data());
        int _tag = _tags.get<int>(v.first);
        std::pair<int, int> _pair;
        _pair = std::make_pair(_label, _tag);
        tags.insert(_pair);
    }

    BOOST_FOREACH(const boost::property_tree::ptree::value_type &v, 
            jsonTree.get_child("paths")) {
        int _label = atoi(v.first.data());
        std::vector<int> _array = as_vector<int>(v.second, 
            std::to_string(_label));
        paths.insert(std::make_pair(_label, _array));
    }
}

Message::Message(const boost::property_tree::ptree& _tree) {
    this->jsonTree = _tree;
}
////////////////////////////////////////////////////////////////////////////////
// Initial constructor
Message::Message(int _start, int _end) {
    boost::property_tree::ptree initTree;
    this->startNode = _start;
    this->destinationNode = _end;

    initTree.put("msgCameFrom", _start);
    initTree.put("method", 0);
    initTree.put("start", _start);
    initTree.put("end", _end);

    boost::property_tree::ptree knownNodes;
    initTree.add_child("knownNodes", knownNodes);
    this->knownNodes.insert(_start);

    boost::property_tree::ptree visitedNodes;
    boost::property_tree::ptree node;
    node.put("", _start);
    visitedNodes.push_back(std::make_pair("", node));
    initTree.add_child("visitedNodes", visitedNodes);
    this->visitedNodes.insert(_start);
    
    boost::property_tree::ptree tags;
    tags.put(std::to_string(_start), 0);
    initTree.add_child("tags", tags);

    boost::property_tree::ptree paths;
    paths.put(std::to_string(_start), "");
    initTree.add_child("paths", paths);

    this->jsonTree = initTree;
}

void Message::setMsgCameFrom(int _val) {
    jsonTree.put("msgCameFrom", _val);
}

int Message::getMsgCameFrom() {
    return jsonTree.get<int>("msgCameFrom");
}

int Message::getDestination() {
    return this->destinationNode;
}

int Message::getStart() {
    return this->startNode;
}

std::set<int> Message::getKnownNodes() {
    return this->knownNodes;
}

void Message::setKnownNodes(std::set<int>& _val) {
    this->knownNodes.insert(_val.begin(), _val.end());
    boost::property_tree::ptree knownNodesTree;
    BOOST_FOREACH(int x, _val) {
        boost::property_tree::ptree node;
        node.put("", x);
        knownNodesTree.push_back(std::make_pair("", node));
    }
    this->jsonTree.erase("knownNodes");
    this->jsonTree.add_child("knownNodes", knownNodesTree);
}

std::set<int> Message::getVisitedNodes() {
    return this->visitedNodes;
}

void Message::setVisitedNodes(std::set<int>& _val) {
    this->visitedNodes.insert(_val.begin(), _val.end());
    boost::property_tree::ptree visitedNodesTree;
    BOOST_FOREACH(int x, _val) {
        boost::property_tree::ptree node;
        node.put("", x);
        visitedNodesTree.push_back(std::make_pair("", node));
    }
    this->jsonTree.erase("visitedNodes");
    this->jsonTree.add_child("visitedNodes", visitedNodesTree);
}

boost::container::map<int, std::vector<int>> Message::getPaths() {
    return this->paths;
}

boost::container::map<int, int> Message::getTags() {
    return this->tags;
}

std::string Message::encodeString() {
    std::ostringstream oss;
    boost::property_tree::write_json(oss, this->jsonTree);
    return oss.str();
}

Message::~Message() {
    //
}