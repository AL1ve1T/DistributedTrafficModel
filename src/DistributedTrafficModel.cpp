// DistributedTrafficModel.cpp : Defines the entry point for the application.
//
/////////////////////////////////////////////////////////////////////////////
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
/////////////////////////////////////////////////////////////////////////////

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
        std::string path = PROJ_ROOT + std::string("/resource/traffic.json");
		std::ifstream inFile(path);
        // Read file
        std::string data((std::istreambuf_iterator<char>(inFile)), 
            (std::istreambuf_iterator<char>()));
        ss << data;

        boost::property_tree::ptree pt;
        boost::property_tree::read_json(ss, pt);
    }
    catch (const boost::property_tree::json_parser::json_parser_error& e)
    {
        std::cerr << "Invalid JSON" << std::endl;
        std::cerr << e.what() << std::endl;
    }
    catch (std::exception const& e)
    {
        std::cerr << e.what() << std::endl;
    }
	return 0;
}
