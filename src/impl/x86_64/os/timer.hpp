#ifndef __avetharun_clearos_rt_os_timer_hpp
#define __avetharun_clearos_rt_os_timer_hpp
#include "types.h"
#include "asm.h"
#include "vga.h"
#include "iostream.h"
#include "stdlib.h"
#define bios_ctpd 0x1800B0
#define $asmt \n\t
typedef void (*TI)(void);

int ticks = 0;			// increases every n ticks
int ticks_last = 0;		// Amount of ticks since last interval size reached 
struct time__T{
// I think (2^64)-1 should be enough, hehe
	uint64 __internalTicks; // Measures ticks since boot. *Note: Innacurate, use get_ticks_since_boot*() instead. This updates when the clock speed is updated. ( Which should be every second )
	uint16 system_hours; // 0000 - 9999 needed
	uint16 system_minutes;
	uint16 system_seconds;
	uint64 system_ticks;
	uint64 system_ticks_last;
	uint64 system_ticks_first;

      unsigned char second;
      unsigned char second_last;
      unsigned char minute;
      unsigned char hour;
      unsigned char day;
      unsigned char month;
      unsigned int year;
      unsigned char century;

      uint64 ticks_per_ms = 10000;
};
time__T time;
/* this is used with ticks to measure
 * a desired interval; e.g. if you increase it
 * every 1000 ticks and one tick is 1 ms, it measures secods */
int interval = 0;
int interval_size = 0; // sets the number of ticks in one interval

void set_interval_size(int how_many_ticks)
{
	/* sets the interval variable 
	 * also resets the interval number */
	
	interval_size = how_many_ticks;
	interval = 0;
	ticks = 0;
}



int CURRENT_YEAR = 2021;                                    // witchcraft
int century_register = 0x00;                                // Set by ACPI table parsing code if possible

 
enum {
      cmos_address = 0x70,
      cmos_data    = 0x71
};
 
int get_update_in_progress_flag() {
      outb(cmos_address, 0x0A);
      return (inb(cmos_data) & 0x80);
}
 
unsigned char get_RTC_register(int reg) {
      outb(cmos_address, reg);
      return inb(cmos_data);
}
 


void read_rtc() {
      unsigned char last_second;
      unsigned char last_minute;
      unsigned char last_hour;
      unsigned char last_day;
      unsigned char last_month;
      unsigned char last_year;
      unsigned char last_century;
      unsigned char registerB;
 
      // Note: This uses the "read registers until you get the same values twice in a row" technique
      //       to avoid getting dodgy/inconsistent values due to RTC updates
 
      while (get_update_in_progress_flag());                // Make sure an update isn't in progress
      time.second = get_RTC_register(0x00);
      time.minute = get_RTC_register(0x02);
      time.hour = get_RTC_register(0x04);
      time.day = get_RTC_register(0x07);
      time.month = get_RTC_register(0x08);
      time.year = get_RTC_register(0x09);
      if(century_register != 0) {
            time.century = get_RTC_register(century_register);
      }
 
      do {
            last_second = time.second;
            last_minute = time.minute;
            last_hour = time.hour;
            last_day = time.day;
            last_month = time.month;
            last_year = time.year;
            last_century = time.century;
 
            while (get_update_in_progress_flag());           // Make sure an update isn't in progress
            time.second = get_RTC_register(0x00);
            time.minute = get_RTC_register(0x02);
            time.hour = get_RTC_register(0x04);
            time.day = get_RTC_register(0x07);
            time.month = get_RTC_register(0x08);
            time.year = get_RTC_register(0x09);
            if(century_register != 0) {
                  time.century = get_RTC_register(century_register);
            }
      } while( (last_second != time.second) || (last_minute != time.minute) || (last_hour != time.hour) ||
               (last_day != time.day) || (last_month != time.month) || (last_year != time.year) ||
               (last_century != time.century) );
 
      registerB = get_RTC_register(0x0B);
 
      // Convert BCD to binary values if necessary
 
      if (!(registerB & 0x04)) {
            time.second = (time.second  & 0x0F) + ((time.second / 16)         * 10);
            time.minute = (time.minute  & 0x0F) + ((time.minute / 16)         * 10);
            time.hour = ( (time.hour    & 0x0F) + (((time.hour  & 0x70) / 16) * 10) ) | (time.hour & 0x80);
            time.day =    (time.day     & 0x0F) + ((time.day    / 16)         * 10);
            time.month =  (time.month   & 0x0F) + ((time.month  / 16)         * 10);
            time.year =   (time.year    & 0x0F) + ((time.year   / 16)         * 10);
            if(century_register != 0) {
                  time.century = (time.century & 0x0F) + ((time.century / 16) * 10);
            }
      }
 
      // Convert 12 hour clock to 24 hour clock if necessary
 
      if (!(registerB & 0x02) && (time.hour & 0x80)) {
            time.hour = ((time.hour & 0x7F) + 12) % 24;
      }
 
      // Calculate the full (4-digit) year
 
      if(century_register != 0) {
            time.year += time.century * 100;
      } else {
            time.year += (CURRENT_YEAR / 100) * 100;
            if(time.year < CURRENT_YEAR) time.year += 100;
      }
}


static bool firstcall = false;
void timer_inc(void){
	// count ticks
	time.system_ticks_last = rdtsc();
      time.second_last = time.second;
	read_rtc();

	if (!firstcall) {
            firstcall = true;
      }
      if (time.second >= time.second_last) {
            // Measure ticks since last second
            time.system_ticks_first = rdtsc();
            time.system_ticks = time.system_ticks_last - time.system_ticks_first;
            time.ticks_per_ms = time.system_ticks / 1000; // Rough estimate. Don't rely on this!
      }
}
void tick() {
      timer_inc();
}
int get_ticks(void)
{
	/* return current number of ticks since last update cycle*/
	return ticks;
}

int get_ticks_since_boot(void)
{	// This is self explanatory, I think
	return rdtsc();

}
// Sleep N ms
// ~1ms deviation
void sleep(unsigned int ms)
{
      int _tick_start = rdtsc();
      int _tick_current = rdtsc();
      int _time_this = 0;
      int _tick_goal = ms * time.ticks_per_ms * 1000;
      while (_tick_current < _tick_goal) {
            int _start = _tick_current;
            timer_inc();
            int _end = rdtsc();
            _tick_current += _end - _start;
      }
}

void halt(void)
{
	/* makes the processor halt */
	asm("hlt");
}

// Easiest way to do this.
void srand_init() {
      timer_inc();
	srand(time.minute + time.day * time.second); // Starting off, it should be initially random. 
}


#endif