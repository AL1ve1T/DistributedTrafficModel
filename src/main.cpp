// main.cpp : Defines the entry point for the application.
//
//
//   @uthor: Elnur Alimirzayev,  <elnur.alimirzayev@gmail.com>
//                               <aliveit.elnur@gmail.com>
// 
////////////////////////////////////////////////////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include "include/node.hpp"

#if defined(__unix__) && !defined(__linux__)
    #define TRAFFIC_FILE "resource\\traffic.json"
#else
    #define TRAFFIC_FILE "resource/traffic.json"
#endif

/*void client_session(boost::shared_ptr<ip::tcp::socket> sock)
 {
    while ( true)
     {
        char data[512];
        size_t len = sock->read_some(buffer(data));
        if (len > 0) write(*sock, buffer("ok", 2));
    }
}*/

int main(int argc, char* argv[])
{
	try
    {
        std::stringstream ss;
        std::string path = TRAFFIC_FILE;
		std::ifstream inFile(path);
        // Read file
        std::string data((std::istreambuf_iterator<char>(inFile)), 
            (std::istreambuf_iterator<char>()));
        ss << data;

        boost::property_tree::ptree pt;
        boost::property_tree::read_json(ss, pt);

        Node node(pt, atoi(argv[2]), argv[1]);
        node.acceptConnections();
    }
    catch (const boost::property_tree::json_parser
        ::json_parser_error& e)
    {
        std::cerr << "Invalid JSON" << std::endl;
        std::cerr << e.what() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
	return 0;
}
