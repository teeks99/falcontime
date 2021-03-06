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

#ifndef _NetworkSyncer_h_
#define _NetworkSyncer_h_

#include "base_messages.h"
#include <boost/cstdint.hpp>
#include <boost/thread/thread.hpp>

namespace FalconTime{
    enum UPDATE_ALGORITHM{
        RAW_VALUE,
        HALF_ROUND_TRIP
    };
    class Offset;
    class MainClock;
    class LibraryConnection;

    //! Controls time synchronization with a remote server
    class NetworkSyncer{
    public:
        NetworkSyncer(Offset* offset, MainClock* local_clock, LibraryConnection* conn);
        ~NetworkSyncer();
        
        //! Creates a thread that will sync automaically (default every 1s)
        //! \param wait_ms is the time to wait between syncs, you shouldn't need to 
        //! do it very often, once every few seconds should be more than enough.
        void enable_auto_sync(uint64_t wait_ms = 10000); 
        //! Manually sync a thread (Don't do this if you have enabled auto sync)
        void sync();

        // Callbacks registered with RealtimeSorter and HousekeepingSorter
        void process_response(time_response_message m);
        void process_algorithm_update(offset_update_algorithm m);

    private:
        // Initiated in a new thread, loops until not _auto_sync_running.
        void sync_thread_loop();
        Offset* _offset;
        MainClock* _local_clock;
        LibraryConnection* _conn;
        UPDATE_ALGORITHM _update_algorithm;
        int64_t _ignore_below;
        unsigned int _rate;
        uint64_t _send_time;

        bool _auto_sync_running;
        uint64_t _auto_sync_wait_ms;

        boost::thread* _sync_thread;

    };
};

#endif _Syncer_h_
