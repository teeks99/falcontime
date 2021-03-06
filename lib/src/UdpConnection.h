/*
    Copyright 2013 Thomas Kent
*/

/************************************************************************ 
This file is part of Falcon Time.

    Falcon Time is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as 
    published by the Free Software Foundation, either version 3 of the 
    License, or (at your option) any later version.

    Falcon Time is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Falcon Time.  If not, see <http://www.gnu.org/licenses/>.
************************************************************************/

#ifndef _UdpConnection_h_
#define _UdpConnection_h_

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <cstddef>
#include <string>

namespace FalconTime{
    class RealtimeSorter;
    //! Establishes UDP connections for the high-priority messages
    class UdpConnection{
    public:
        // Use in the server, listens for all clients
        UdpConnection(unsigned short port, RealtimeSorter* sorter);
        // Use in the client for talking to the server
        UdpConnection(std::string host, unsigned short port, RealtimeSorter* sorter);
        ~UdpConnection();

        void send(void* message, std::size_t size);
        void send(void* message, std::size_t size, boost::asio::ip::udp::endpoint to);
    private:
        // Handles the messages after they are successfully received
        RealtimeSorter* _sorter;
        // Call this after every reply from the async messages, if the _io_service doesn't
        // have one of these it will stop working
        void start_receive();
        // Callback to be registered with the socket
        void receive(const boost::system::error_code& error, std::size_t bytes);

        boost::thread* _io_thread;
        boost::asio::io_service _io_service;
        bool _server;
        boost::asio::ip::udp::endpoint _host;
        boost::asio::ip::udp::socket* _socket;
        boost::asio::ip::udp::endpoint _received_from;
        static const size_t _max_buf_size = 4096; //4KB
        unsigned char* _rcv_buf;
    };
};
#endif //_UdpConnection_h_