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
#include "Api.h"
#include "HighprefClock.h"
#include "Offset.h"

using namespace FalconTime;

static Api* api;

int enable_falcon_time(){
    api = new Api();
    return 0;
}
void get_start(char* start){
    std::string s = api->start();
    std::copy(s.begin(), s.end(), start);
    start[s.size()] = '\0'; // Need to null terminate
    return;
}
highpref_time get_time(){
    return api->elapsed();
}

Api::Api(){
    _clock = new SyncedClock();
    _conn = new LibraryConnection(_clock);
}

Api::~Api(){
    delete _conn;
    delete _clock;
}

highpref_time Api::elapsed(){
    return nanoseconds_to_highpref_time(_clock->nanoseconds());
}

std::string Api::start(){
    return _clock->get_offset()->get_start();
}