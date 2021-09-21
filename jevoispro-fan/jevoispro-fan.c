// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// JeVois Smart Embedded Machine Vision Toolkit - Copyright (C) 2021 by Laurent Itti, the University of Southern
// California (USC), and iLab at USC. See http://iLab.usc.edu and http://jevois.org for information about this project.
//
// This file is part of the JeVois Smart Embedded Machine Vision Toolkit.  This program is free software; you can
// redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software
// Foundation, version 2.  This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
// License for more details.  You should have received a copy of the GNU General Public License along with this program;
// if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
// Contact information: Laurent Itti - 3641 Watt Way, HNB-07A - Los Angeles, CA 90089-2520 - USA.
// Tel: +1 213 740 3527 - itti@pollux.usc.edu - http://iLab.usc.edu - http://jevois.org
// ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*! \file */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <syslog.h>

// Below TMIN (in millidegrees), fan is off (or running at very quiet speed); above TMAX it is fully on:
#define TMIN 65000
#define TMAX 85000

// Was period 1M, min duty 100k but Howie's fan is very loud, so instead use period 100k, min duty 4250:
#define PERIOD 100000
#define DMAX PERIOD
#define DMIN 4300

static long period = PERIOD, dmin = DMIN, dmax = DMAX, tmin = TMIN, tmax = TMAX;


// ####################################################################################################
// Like "echo value > fname"
void writefile(char const * fname, char const * value)
{
  int fd = open(fname, O_WRONLY);
  if (fd == -1) { syslog(LOG_ERR, "Failed to open %s for write", fname); return; }
  if (write(fd, value, strlen(value)) < 0) syslog(LOG_ERR, "Failed to write %s to %s", value, fname);
  close(fd);
}

// ####################################################################################################
// Like "cat fname" and convert to int
char buf[16];
int readfile(char const * fname, int log)
{
  int fd = open(fname, O_RDONLY);
  if (fd == -1)
  {
    if (log) { syslog(LOG_ERR, "Failed to open %s for read", fname); return tmax; } // fan full on if we fail to read
    return tmin; // min fan if we can't get TPU temp
  }
  buf[0] = '0'; buf[1] = '\0';
  if (read(fd, buf, sizeof(buf)-1) < 0 && log) { syslog(LOG_ERR, "Failed to read from %s", fname); return tmax; }
  close(fd);
  buf[sizeof(buf)-1] = '\0';
  return atoi(buf);
}

// ####################################################################################################
// main function
int main (int argc, char **argv)
{
  long temp, temp2; long duty; char dutystr[16]; int ret; int c;
  float dpmin = 4.3F, dpmax = 100.0F;
  
  openlog("jevoispro-fan", LOG_PID, LOG_DAEMON);

  // Parse command-line options:
  opterr = 0;

  while ((c = getopt(argc, argv, "p:d:D:t:T:")) != -1)
    switch (c)
    {
    case 'p': period = atoi(optarg); break;
    case 'd': dpmin = atof(optarg); break;
    case 'D': dpmax = atof(optarg); break;
    case 't': tmin = atoi(optarg) * 1000; break;
    case 'T': tmax = atoi(optarg) * 1000; break;
    case '?': syslog(LOG_ERR, "jevoispro-fan: bad command-line arguments ignored"); break;
    default: break;
    }

  dmin = (long)(period * dpmin / 100.0F + 0.499F);
  dmax = (long)(period * dpmax / 100.0F + 0.499F);
  if (tmax <= tmin) tmax = tmin + 1000;
  if (dmax > period) dmax = period;
  if (dmax < 0) dmax = 0;
  if (dmin > dmax) dmin = dmax;
  
  syslog(LOG_NOTICE, "jevoispro-fan started: period=%ldns, duty=[%.1f%%..%.1f%%], temp=[%ldC..%ldC]",
         period, dpmin, dpmax, tmin / 1000, tmax / 1000);

  // Initialize the pwm and give the fan an initial burst at DMAX. Proceed in an order such that the driver never
  // rejects our values:
  writefile("/sys/class/pwm/pwmchip8/export", "0\n");
  writefile("/sys/class/pwm/pwmchip8/pwm0/enable", "0\n");
  writefile("/sys/class/pwm/pwmchip8/pwm0/duty_cycle", "0\n");
  snprintf(dutystr, sizeof(dutystr), "%ld\n", period); writefile("/sys/class/pwm/pwmchip8/pwm0/period", dutystr);
  snprintf(dutystr, sizeof(dutystr), "%ld\n", dmax); writefile("/sys/class/pwm/pwmchip8/pwm0/duty_cycle", dutystr);
  writefile("/sys/class/pwm/pwmchip8/pwm0/enable", "1\n");

  sleep(10);

  // Throttle down to dmin unless we reach a high temp below:
  duty = dmin;
    
  while (1)
  {
    // Take the max of CPU big A73 cores, TPU0 (if present), TPU1 (if present):
    temp = readfile("/sys/class/thermal/thermal_zone1/temp", 1);
    temp2 = readfile("/sys/class/apex/apex_0/temp", 0); if (temp2 > temp) temp = temp2;
    temp2 = readfile("/sys/class/apex/apex_1/temp", 0); if (temp2 > temp) temp = temp2;
    if (temp < 25000) temp = 25000; else if (temp > 100000) temp = 100000;

    /*
    // Linear ramp algorithm: this is audibly a bit annoying
    //
    // We want full on above TMAX, off below TMIN (actually, to avoid cycling around TMIN, we never turn the fan
    // completely off, we leave it on at DMIN for all temps under TMIN). Note that the fan will not start below a
    // given duty cycle, typically 50k (DMIN) when period is 1M (DMAX=PERIOD):
    duty = (temp - tmin) * (dmax - dmin) / (tmax - tmin) + dmin;
    if (duty > dmax) duty = dmax; else if (duty < dmin) duty = dmin;
    */

    // 2-stage algorithm with hysteresis (less annoying)
    if (temp >= tmax) duty = dmax;
    else if (temp <= tmin) duty = dmin;
    // otherwise no change (hysteresis)
    
    snprintf(dutystr, sizeof(dutystr), "%ld\n", duty);
    writefile("/sys/class/pwm/pwmchip8/pwm0/duty_cycle", dutystr);
    
    sleep(2);
  }

  syslog(LOG_NOTICE, "jevoispro-fan terminated.");
  closelog();

  return EXIT_SUCCESS;
}
