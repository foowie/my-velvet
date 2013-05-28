/***************************************************************************}
{																			}
{    cHugeArray.h                                          		      		}
{																			}
{																			}
{                 Copyright (c) 2012					Jan Platos			}
{																			}
{    VERSION: 1.0											DATE 2012/04/12	}
{																			}
{    following functionality:												}
{		representes a memory array with increasing size with no copy		}
{																			}
{    UPDATE HISTORY:														}
{																			}
{																			}
{***************************************************************************/


#ifndef __CHUGEARRAY_H__
#define __CHUGEARRAY_H__

#include "cArray.h"

namespace arg {

	template <class T>
	class cHugeArray
	{
	private:
		cArray<T*> m_Pointers;
		unsigned int m_BlockSize;
		unsigned int m_Count;

	public:
		inline cHugeArray(const unsigned int blockSize = 1<<10);
		virtual ~cHugeArray();

		inline void Clear();

		inline const unsigned int Count() const;
		inline void Count(const unsigned int count);

		inline void Add(T* items, const unsigned int count=1);

		inline T& operator[](const int Index);
		inline T& operator[](const unsigned int Index);
		inline const T operator[](const int Index) const;
		inline const T operator[](const unsigned int Index) const;
	};

	template <class T>
	inline cHugeArray<T>::cHugeArray(const unsigned int blockSize)
		: m_BlockSize(blockSize), m_Count(0) 
	{
		m_Pointers.SetDelta(7);
	}

	template <class T>
	inline cHugeArray<T>::~cHugeArray()
	{	
		for(unsigned int i=0; i<m_Pointers.Count(); i++)
			delete [] m_Pointers[i];
		m_Pointers.Clear();
	}

	template <class T>
	inline const unsigned int cHugeArray<T>::Count() const
	{
		return m_Count;
	}

	template <class T>
	inline void cHugeArray<T>::Count(const unsigned int count)
	{
		m_Count = count;
	}

	template <class T>
	inline void cHugeArray<T>::Clear()
	{
		m_Count=0;
		for(unsigned int i=0; i<m_Pointers.Count(); i++)
			delete [] m_Pointers[i];
		m_Pointers.Clear();
	}

	template <class T>
	inline T& cHugeArray<T>::operator[](const int Index)
	{
		unsigned int block = Index / m_BlockSize;
		unsigned int idx = Index % m_BlockSize;
		return m_Pointers[block][idx];
	}

	template <class T>
	inline T& cHugeArray<T>::operator[](const unsigned int Index)
	{
		unsigned int block = Index / m_BlockSize;
		unsigned int idx = Index % m_BlockSize;
		return m_Pointers[block][idx];
	}

	template <class T>
	inline const T cHugeArray<T>::operator[](const int Index) const
	{
		unsigned int block = Index / m_BlockSize;
		unsigned int idx = Index % m_BlockSize;
		return m_Pointers[block][idx];
	}

	template <class T>
	inline const T cHugeArray<T>::operator[](const unsigned int Index) const
	{
		unsigned int block = Index / m_BlockSize;
		unsigned int idx = Index % m_BlockSize;
		return m_Pointers[block][idx];
	}

	template <class T>
	inline void cHugeArray<T>::Add(T* items, const unsigned int count)
	{
		while (count+m_Count > m_Pointers.Count()*m_BlockSize)
		{
			T* tmp = new T[m_BlockSize];
			m_Pointers.Add(&tmp);
		}

		for (unsigned int i=0; i<count; i++)
			(*this)[m_Count++] = items[i];
	}
	
}

#endif
