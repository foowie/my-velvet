/* 
 * File:   Memwatch.h
 * Author: Daniel Robenek
 *
 * Created on 26. leden 2013, 11:06
 */

#ifndef MEMWATCH_H
#define	MEMWATCH_H

#define MEMWATCH_BUFFER_SIZE 8191
#define MEMWATCH_READ_TOKEN strsep(&token, " ");

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#if defined(_WIN32)
#include <windows.h>
#include <psapi.h>

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
#include <unistd.h>
#include <sys/resource.h>

#if defined(__APPLE__) && defined(__MACH__)
#include <mach/mach.h>

#elif (defined(_AIX) || defined(__TOS__AIX__)) || (defined(__sun__) || defined(__sun) || defined(sun) && (defined(__SVR4) || defined(__svr4__)))
#include <fcntl.h>
#include <procfs.h>

#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
#include <stdio.h>

#endif

#else
#error "Cannot define getPeakRSS( ) or getCurrentRSS( ) for an unknown OS."
#endif


class Memwatch {
private:
	pid_t pid;
	long unsigned int prevSize;
	long unsigned int size;
public:
	Memwatch(bool snap = true, const pid_t pid = -1);
	const Memwatch & Snap();
	const Memwatch & Display(const bool diff = true) const;
	const size_t getPeakRSS() const;
	size_t getCurrentRSS();
	const Memwatch & DisplayPeak() const;
};

Memwatch::Memwatch(bool snap, const pid_t pid) {
	this->pid = pid < 0 ? getpid() : pid;
	size = prevSize = 0;
	if(snap) {
		Snap();
	}
}

const Memwatch & Memwatch::Snap() {
	prevSize = size;
	size = getCurrentRSS();
	return *this;
	
	prevSize = size;
	char file_name[256];
	char buffer[MEMWATCH_BUFFER_SIZE];
	FILE * file;
	size_t readed;
	char * token = buffer;

	// prepare file name
	sprintf(file_name, "/proc/%d/stat", pid);

	// load data
	file = fopen(file_name, "r");
	if (file == NULL)
		return *this;
	readed = fread(buffer, sizeof (char), MEMWATCH_BUFFER_SIZE - 1, file);
	fclose(file);
	buffer[readed] = '\0';

	//load info - "man proc"
	MEMWATCH_READ_TOKEN // %d pid 
	MEMWATCH_READ_TOKEN // %s comm
	MEMWATCH_READ_TOKEN // %c state
	MEMWATCH_READ_TOKEN // %d ppid
	MEMWATCH_READ_TOKEN // %d pgrp
	MEMWATCH_READ_TOKEN // %d session
	MEMWATCH_READ_TOKEN // %d tty_nr
	MEMWATCH_READ_TOKEN // %d tpgid
	MEMWATCH_READ_TOKEN // %u flags
	MEMWATCH_READ_TOKEN // %lu minflt
	MEMWATCH_READ_TOKEN // %lu cminflt
	MEMWATCH_READ_TOKEN // %lu majflt
	MEMWATCH_READ_TOKEN // %lu cmajflt
	MEMWATCH_READ_TOKEN // %lu utime
	MEMWATCH_READ_TOKEN // %lu stime
	MEMWATCH_READ_TOKEN // %ld cutime
	MEMWATCH_READ_TOKEN // %ld cstime
	MEMWATCH_READ_TOKEN // %ld priority
	MEMWATCH_READ_TOKEN // %ld nice
	MEMWATCH_READ_TOKEN // %ld num_threads
	MEMWATCH_READ_TOKEN // %ld itrealvalue
	MEMWATCH_READ_TOKEN // %llu starttime
	sscanf(token, "%lu", &size);
//	MEMWATCH_READ_TOKEN // %lu vsize
//	MEMWATCH_READ_TOKEN // %ld rss
//	MEMWATCH_READ_TOKEN // %lu rsslim
//	MEMWATCH_READ_TOKEN // %lu startcode
//	MEMWATCH_READ_TOKEN // %lu endcode
//	MEMWATCH_READ_TOKEN // %lu startstack
//	MEMWATCH_READ_TOKEN // %lu kstkesp
//	MEMWATCH_READ_TOKEN // %lu kstkeip
//	MEMWATCH_READ_TOKEN // %lu signal
//	MEMWATCH_READ_TOKEN // %lu blocked
//	MEMWATCH_READ_TOKEN // %lu sigignore
//	MEMWATCH_READ_TOKEN // %lu sigcatch
//	MEMWATCH_READ_TOKEN // %lu wchan
//	MEMWATCH_READ_TOKEN // %lu nswap
//	MEMWATCH_READ_TOKEN // %lu cnswap
//	MEMWATCH_READ_TOKEN // %d exit_signal
//	MEMWATCH_READ_TOKEN // %d processor
//	MEMWATCH_READ_TOKEN // %u rt_priority
//	MEMWATCH_READ_TOKEN // %u policy
//	MEMWATCH_READ_TOKEN // %llu delayacct_blkio_ticks
//	MEMWATCH_READ_TOKEN // %lu guest_time
//	MEMWATCH_READ_TOKEN // %ld cguest_time
	return *this;
}

const Memwatch & Memwatch::Display(const bool diff) const {
	unsigned long int value = diff ? (size > prevSize ? size - prevSize : 0) : size;
	if(value > 1024 * 1024 * 10) {
		fprintf(stderr, "Size: %lu MB\n", value / (1024 * 1024));
	} else if(value > 1024 * 10) {
		fprintf(stderr, "Size: %lu kB\n", value / 1024);
	} else {
		fprintf(stderr, "Size: %lu bytes\n", value);
	}
	return *this;
}

const Memwatch & Memwatch::DisplayPeak() const {
	size_t value = getPeakRSS();
	if(value > 1024 * 1024 * 10) {
		fprintf(stderr, "Size: %u MB\n", value / (1024 * 1024));
	} else if(value > 1024 * 10) {
		fprintf(stderr, "Size: %u kB\n", value / 1024);
	} else {
		fprintf(stderr, "Size: %u bytes\n", value);
	}
	return *this;
}



/*
 * Author:  David Robert Nadeau
 * Site:    http://NadeauSoftware.com/
 * License: Creative Commons Attribution 3.0 Unported License
 *          http://creativecommons.org/licenses/by/3.0/deed.en_US
 */


/**
 * Returns the peak (maximum so far) resident set size (physical
 * memory use) measured in bytes, or zero if the value cannot be
 * determined on this OS.
 */
const size_t Memwatch::getPeakRSS() const
{
#if defined(_WIN32)
    /* Windows -------------------------------------------------- */
    PROCESS_MEMORY_COUNTERS info;
    GetProcessMemoryInfo( GetCurrentProcess( ), &info, sizeof(info) );
    return (size_t)info.PeakWorkingSetSize;

#elif (defined(_AIX) || defined(__TOS__AIX__)) || (defined(__sun__) || defined(__sun) || defined(sun) && (defined(__SVR4) || defined(__svr4__)))
    /* AIX and Solaris ------------------------------------------ */
    struct psinfo psinfo;
    int fd = -1;
    if ( (fd = open( "/proc/self/psinfo", O_RDONLY )) == -1 )
        return (size_t)0L;      /* Can't open? */
    if ( read( fd, &psinfo, sizeof(psinfo) ) != sizeof(psinfo) )
    {
        close( fd );
        return (size_t)0L;      /* Can't read? */
    }
    close( fd );
    return (size_t)(psinfo.pr_rssize * 1024L);

#elif defined(__unix__) || defined(__unix) || defined(unix) || (defined(__APPLE__) && defined(__MACH__))
    /* BSD, Linux, and OSX -------------------------------------- */
    struct rusage rusage;
    getrusage( RUSAGE_SELF, &rusage );
#if defined(__APPLE__) && defined(__MACH__)
    return (size_t)rusage.ru_maxrss;
#else
    return (size_t)(rusage.ru_maxrss * 1024L);
#endif

#else
    /* Unknown OS ----------------------------------------------- */
    return (size_t)0L;          /* Unsupported. */
#endif
}





/**
 * Returns the current resident set size (physical memory use) measured
 * in bytes, or zero if the value cannot be determined on this OS.
 */
size_t Memwatch::getCurrentRSS()
{
#if defined(_WIN32)
    /* Windows -------------------------------------------------- */
    PROCESS_MEMORY_COUNTERS info;
    GetProcessMemoryInfo( GetCurrentProcess( ), &info, sizeof(info) );
    return (size_t)info.WorkingSetSize;

#elif defined(__APPLE__) && defined(__MACH__)
    /* OSX ------------------------------------------------------ */
    struct mach_task_basic_info info;
    mach_msg_type_number_t infoCount = MACH_TASK_BASIC_INFO_COUNT;
    if ( task_info( mach_task_self( ), MACH_TASK_BASIC_INFO,
        (task_info_t)&info, &infoCount ) != KERN_SUCCESS )
        return (size_t)0L;      /* Can't access? */
    return (size_t)info.resident_size;

#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
    /* Linux ---------------------------------------------------- */
    long rss = 0L;
    FILE* fp = NULL;
    if ( (fp = fopen( "/proc/self/statm", "r" )) == NULL )
        return (size_t)0L;      /* Can't open? */
    if ( fscanf( fp, "%*s%ld", &rss ) != 1 )
    {
        fclose( fp );
        return (size_t)0L;      /* Can't read? */
    }
    fclose( fp );
    return (size_t)rss * (size_t)sysconf( _SC_PAGESIZE);

#else
   /* AIX, BSD, Solaris, and Unknown OS ------------------------ */
    return (size_t)0L;          /* Unsupported. */
#endif
}

#endif	/* MEMWATCH_H */

