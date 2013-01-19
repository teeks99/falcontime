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

#ifndef _falcontime_h_
#define _falcontime_h_

struct highpref_time
{
    unsigned int seconds;
    unsigned int nanoseconds;
};

/* Returns 0 if successful */
int enable_falcon_time();
int enable_falcon_time_as_client(char* server_address, 
                                 unsigned short server_port = 10320);

/* This is a string representing the start of the seconds clock (in UTC)
   it has the format YYYY-MM-DD HH:MM:SS.DDDDDDDDDD. The char array must
   be at least 32 bytes long*/
void get_start(char* );
/* This is the number of seconds and nanoseconds that have elapsed since
   the start time */
highpref_time get_time();

#endif /*_falcontime_h_*/
