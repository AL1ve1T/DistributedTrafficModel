// DistributedTrafficModel.cpp : Defines the entry point for the application.
//

/*
	This section describes common bussiness logic of 
	this application.

	Generally we have a set of nodes which description
	is given in config.json file with the following format:

	{
		"Node": {
			"Neighbouring nodes": {"Their data"}
		}
	}
*/

#include "./include/DistributedTrafficModel.h"
#include <iostream>
// Boost includes
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

#define PROJ_ROOT "/Users/AliveIT/DistributedTrafficModel"

int main(int argc, char* argv[])
{
	try
    {
        std::stringstream ss;
		std::ifstream inFile("../traffic.json");
        // Read file
        std::string data(std::istreambuf_iterator<char>(), 
            std::istreambuf_iterator<char>());

        boost::property_tree::ptree pt;
        boost::property_tree::read_json(ss, pt);

        BOOST_FOREACH(boost::property_tree::ptree::value_type &v, pt.get_child("0.1"))
        {
            assert(v.first.empty()); // array elements have no names
            std::cout << v.second.data() << std::endl;
        }
    }
    catch (std::exception const& e)
    {
        std::cerr << e.what() << std::endl;
    }
	return 0;
}
