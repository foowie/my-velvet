#ifndef __CRANDOM_H__
#define __CRANDOM_H__

#include <stdlib.h>
#include <ctime>
#include "cMersenneTwister.h"

namespace arg
{
	/**
	 * \class arg::cRandom
	 * \brief An implementation of simple adjustable pseudo-random number generator.
	 *
	 * This version is not static and thread safe, iff every thread uses private cRandom
	 * object. In the OpenMP framework, one can use the cRandom like this:
	 * \code
	 * #pragma omp parallel
	 * {
	 *		const int tid = omp_get_thread_num();
	 *		arg::cRandom rnd(tid*time(NULL));
	 *		...
	 * }
	 * \endcode
	 * \author Pavel Krömer (pkromer), (c) 2011
	 *
	 * \b History:
	 *		- 2011-09,	pkromer,	Created as extension of mersene twister.
	 */
	class cRandom: public cMersenneTwister
	{
			static cMersenneTwister m_Generator;

		public:

			inline cRandom();

			inline cRandom(const unsigned int);
			inline void Seed(const unsigned int); 			///< Seed with some value.
			inline int Next(void); 							///< Next integer.
			inline double Next(const double); 				///< Next double lower equal to argument
			inline double Next(const double, const double); ///< Next double from a range
			inline int NextInt(const int); 					///< Next integer lower equal to argument
	};

	inline cRandom::cRandom()
	{
		seed( (const unsigned int) time(NULL));
	}

	inline cRandom::cRandom(const unsigned int seed_val)
	{
		seed(seed_val);
	}

	inline void cRandom::Seed(const unsigned int seed_val)
	{
		seed(seed_val);
	}

	inline int cRandom::Next(void)
	{
		return randInt();
	}

	inline double cRandom::Next(const double up_to)
	{
		return rand(up_to);
	}

	inline double cRandom::Next(const double from, const double to)
	{
		return from + rand() * (to - from);
	}

	inline int cRandom::NextInt(const int up_to)
	{
		return randInt(up_to);
	}

	/**
	 * \class arg::cStaticRandom
	 * \brief An implementation of simple adjustable pseudo-random number generator.
	 *
	 * Provides static utility methods for pseudo rng. Uses a \b static cMerseneTwister
	 * in background. I.e. every component that uses this class gets pseudo random
	 * numbers from the same source.
	 *
	 * \author Pavel Krömer (pkromer), (c) 2005 - 2010
	 *
	 * \b History:
	 *		- 2005,		pkromer,	initial version
	 *		- 2006,		pkromer, 	more methods
	 *		- 2011-02,	pkromer,	changed to a facade to a static cMersenneTwister
	 *
	 */
	class cStaticRandom
	{
			static cMersenneTwister m_Generator;

		public:
			inline static void Seed(const unsigned int); 			///< Seed with some value.
			inline static int Next(void); 							///< Next integer.
			inline static double Next(const double); 				///< Next double lower equal to argument
			inline static double Next(const double, const double); 	///< Next double from a range
			inline static int NextInt(const int); 					///< Next integer lower equal to argument
	};

	inline void cStaticRandom::Seed(const unsigned int seed)
	{
		return cStaticRandom::m_Generator.seed(seed);
	}

	inline int cStaticRandom::Next(void)
	{
		return cStaticRandom::m_Generator.randInt();
	}

	inline double cStaticRandom::Next(const double up_to)
	{
		return m_Generator.rand(up_to);
	}

	inline double cStaticRandom::Next(const double from, const double to)
	{
		return from + cStaticRandom::m_Generator.rand() * (to - from);
	}

	inline int cStaticRandom::NextInt(const int up_to)
	{
		return cStaticRandom::m_Generator.randInt(up_to);
	}
}

#endif
