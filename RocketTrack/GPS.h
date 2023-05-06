
#pragma once

extern char gps_type[];
extern int initial_baud;
extern int final_baud;
extern int fix_rate;

extern uint8_t gps_numSats;

extern int32_t gps_lat;
extern int32_t gps_lon;
extern int32_t gps_height;

extern uint16_t gps_year;
extern uint8_t gps_month;
extern uint8_t gps_day;
extern uint8_t gps_hour;
extern uint8_t gps_min;
extern uint8_t gps_sec;
