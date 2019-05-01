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
    visitedNodes    = as_vector<int>(jsonTree, "visitedNodes");
    knownNodes      = as_vector<int>(jsonTree, "knownNodes");

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

Message::Message(int _start, int _end) {
    boost::property_tree::ptree initTree;
    this->startNode = _start;
    this->destinationNode = _end;

    initTree.put("start", _start);
    initTree.put("end", _end);

    boost::property_tree::ptree knownNodes;
    initTree.add_child("knownNodes", knownNodes);

    boost::property_tree::ptree visitedNodes;
    initTree.add_child("visitedNodes", visitedNodes);
    
    boost::property_tree::ptree tags;
    tags.put(std::to_string(_start), 0);
    initTree.add_child("tags", tags);
}

std::vector<int> Message::getKnownNodes() {
    return this->knownNodes;
}

void Message::setKnownNodes(std::vector<int>& _val) {
    this->knownNodes = _val;
}

std::vector<int> Message::getVisitedNodes() {
    return this->visitedNodes;
}

void Message::setVisitedNodes(std::vector<int>& _val) {
    this->visitedNodes = _val;
}

boost::container::map<int, std::vector<int>> Message::getPaths() {
    return this->paths;
}

boost::container::map<int, int> Message::getTags() {
    return this->tags;
}

std::string Message::encodeString() {
    std::ostringstream oss;
    boost::property_tree::ini_parser::write_ini(oss, this->jsonTree);
    return oss.str();
}

Message::~Message() {
    //
}