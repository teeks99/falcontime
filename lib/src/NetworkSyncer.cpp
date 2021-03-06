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

#include "NetworkSyncer.h"
#include "HighprefClock.h"
#include "MainClock.h"
#include "Offset.h"
#include "LibraryConnection.h"
#include <boost/bind.hpp>
#include <boost/thread/thread.hpp>
#include <stdlib.h>

using namespace FalconTime;

NetworkSyncer::NetworkSyncer(Offset* offset, MainClock* local_clock, LibraryConnection* conn){
    _auto_sync_running = 0;
    _offset = offset;
    _local_clock = local_clock;
    _conn = conn;
    _update_algorithm = RAW_VALUE;
    _ignore_below = 10000000; //10ms //TODO ????
}

NetworkSyncer::~NetworkSyncer(){
    _auto_sync_running = false;
}

void NetworkSyncer::enable_auto_sync(uint64_t wait_ms){
    _auto_sync_wait_ms = wait_ms;
    _sync_thread = new boost::thread(boost::bind(&NetworkSyncer::sync_thread_loop, this));
}

void NetworkSyncer::sync_thread_loop(){
    _auto_sync_running = true;
    while(_auto_sync_running){
        this->sync();
        // This might not be accurate down to the millisecond on windows but that shouldn't matter
        boost::this_thread::sleep_for(boost::chrono::milliseconds(_auto_sync_wait_ms));
    }
}

void NetworkSyncer::sync(){
    time_request_message* m = new time_request_message();
    m->message_id = TIME_MESSAGE_REQUEST_ID;
    m->client_id = _conn->get_client_id();
    _conn->send_udp(m, 8);
    _send_time = _local_clock->nanoseconds();
}

void NetworkSyncer::process_response(time_response_message m){
    uint64_t local_ns = _local_clock->nanoseconds();

    highpref_time remote_time;
    remote_time.seconds = m.seconds;
    remote_time.nanoseconds = m.nanoseconds;

    uint64_t remote_ns = highpref_time_to_nanoseconds(remote_time);
    
    int64_t difference;
    // TODO: Add different algorithms to update here, may need serious re-structuring
    switch(_update_algorithm){
    case HALF_ROUND_TRIP:
        break;
    case RAW_VALUE:
    default:
       difference = local_ns - remote_ns;    
    }
    // How far off is the difference when you include the offset?
    int64_t current_difference = difference - _offset->get_offset();
    // If its really close, don't do anything.
    if((current_difference > _ignore_below) || (current_difference < (_ignore_below * -1))){
        _offset->set_offset(difference);
        //std::cout << "Updating offset, difference: " << difference << std::endl;
    }
}

void NetworkSyncer::process_algorithm_update(offset_update_algorithm m){
    //TODO: do string lookup

}