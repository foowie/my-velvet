/* 
 * File:   stopwatch.h
 * Author: Daniel Robenek
 *
 * Created on 16. listopad 2012, 16:59
 */
//#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

/*
 * C++ compiler / command line options add "-lrt"
 * http://stackoverflow.com/questions/2418157/ubuntu-linux-c-error-undefined-reference-to-clock-gettime-and-clock-settim
 */

#ifndef STOPWATCH_H
#define	STOPWATCH_H

/**
	CLOCK_REALTIME
		System-wide  realtime clock.  Setting this clock requires appro-
		priate privileges.
	CLOCK_MONOTONIC
		Clock that cannot be set and  represents  monotonic  time  since
		some unspecified starting point.
	CLOCK_PROCESS_CPUTIME_ID
		High-resolution per-process timer from the CPU.
	CLOCK_THREAD_CPUTIME_ID
		Thread-specific CPU-time clock.
  */
#define STOPWATCH_TYPE CLOCK_PROCESS_CPUTIME_ID

	class Stopwatch {
	private:
		struct timespec startTime;
		struct timespec stopTime;
	public:
		inline Stopwatch(const bool doStart = true);
		inline void Start();
		inline const Stopwatch & Stop();
		void Display() const;
	};

	Stopwatch::Stopwatch(const bool doStart) {
		memset(&startTime, 0, sizeof(timespec));
		memset(&stopTime, 0, sizeof(timespec));
		if (doStart) {
			Start();
		}
	}

	void Stopwatch::Start() {
		if (startTime.tv_sec != 0) {
			throw "Stopwatches has been already started!";
		}
		if (clock_gettime(STOPWATCH_TYPE, &startTime) != 0) {
			throw "Error during gettimeofday call!";
		}
	}

	const Stopwatch & Stopwatch::Stop() {
		if (stopTime.tv_sec != 0) {
			throw "Stopwatches has been already stopped!";
		}
		if (clock_gettime(STOPWATCH_TYPE, &stopTime) != 0) {
			throw "Error during gettimeofday call!";
		}
		return (*this);
	}

	void Stopwatch::Display() const {
		if (stopTime.tv_sec == 0 && stopTime.tv_nsec == 0) {
			throw "Stopwatches hasn't been stopped yet!";
		}
		long nsec = stopTime.tv_nsec - startTime.tv_nsec;
		long sec = stopTime.tv_sec - startTime.tv_sec;
		if (nsec < 0) {
			sec--;
			nsec += 1000000000;
		}

		
		char buffer[128];
		sprintf(buffer, "%ld.%06ld sec\n", sec, nsec);
		char * ch = strchr(buffer, '.');
		if(ch != NULL) {
			(*ch) = ',';
		}
		fprintf(stderr, "%s", buffer);
		
//		if(sec > 0) {
//			printf("%ld.%06ld sec\n", sec, nsec);
//		} else {
//			printf("%lf ms\n", nsec / 1000000.0);
//		}
	}

#endif	/* STOPWATCH_H */

