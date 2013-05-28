/***************************************************************************}
{																			}
{    cArray.h                                            		      		}
{																			}
{																			}
{                 Copyright (c) 1999, 2011					Vaclav Snasel	}
{																			}
{    VERSION: 2.0											DATE 20/2/1999	}
{																			}
{             following functionality:										}
{																			}
{																			}
{    UPDATE HISTORY:														}
{																			}
{																			}
{***************************************************************************/


#ifndef __CARRAY_H__
#define __CARRAY_H__

#include <memory.h>

namespace arg {

	const unsigned int DELTA[] =
	{
		0x000007,//0
		0x00000f,//1
		0x00001f,//2
		0x00003f,//3
		0x00007f,//4
		0x0000ff,//5
		0x000fff,//6
		0x00ffff,//7
		0x01ffff,//8
		0x03ffff,//9
		0x07ffff,//10
		0x0fffff,//11
		0x1fffff,//12
		0x3fffff,//13
		0x7fffff,//14
		0xffffff,//15
	};



	template<class T>
	class cArray
	{

	public:
		cArray();
		~cArray();
		const cArray<T>& operator = (const cArray& Other);
		inline T& operator[](const int Index);
		inline T& operator[](const unsigned int Index);
		inline const T operator[](const int Index) const;
		inline const T operator[](const unsigned int Index) const;
		inline operator T* (void);
		inline unsigned int Size(void) const;
		inline unsigned int Count(void) const;
		inline void Clear(void);
		inline void ClearCount(void);
		inline void SetCount(const unsigned int Count);
		inline void Resize(const unsigned int Size, const bool Move = false);
		inline void Resize(const unsigned int Size, const unsigned int Count, const bool Move = false);
		void Move(const T* Array, const unsigned int Count = 1);
		void Add(const T* Array);
		void Add(const T* Array, const unsigned int Count);
		inline void Left(const unsigned int Count);
		inline void Right(const unsigned int Count);
		inline void Mid(const unsigned int Left, const unsigned int Right);
		inline void Append(const T& Value);
		inline T* GetArray(const unsigned int Index = 0);
		inline T* GetArray(const unsigned int Index = 0) const;
		void Fill(const char ch, const unsigned int Count);
		inline void SetDelta(const unsigned int Delta);
		inline unsigned int GetDelta(void) const;
		inline void SetDynamic(const bool Dynamic);
		inline bool GetDynamic(void) const;

	private:
		unsigned int m_Size;
		unsigned int m_Count;
		unsigned int m_Delta;
		bool m_Dynamic;
		T* m_Array;
		void m_Resize(const unsigned int Size, const bool Move);
	};


	template<class T>
	cArray<T>::cArray():  
	m_Size(0), 
		m_Count(0), 
		m_Delta(1),
		m_Dynamic(false),
		m_Array(0)
	{
	}

	template<class T>
	cArray<T>::~cArray()
	{
		m_Resize(0, false);
	}


	template<class T>
	inline const cArray<T>& cArray<T>::operator = (const cArray& Other)
	{
		if (this != &Other)
		{
			Move((T*)(*(const_cast<cArray<T>*>(&Other))), Other.Count());
		}
		return *this;
	}


	template<class T>
	inline T& cArray<T>::operator[](const int Index)
	{
		return m_Array[Index];
	}


	template<class T>
	inline T& cArray<T>::operator[](const unsigned int Index)
	{
		return m_Array[Index];
	}


	template<class T>
	inline const T cArray<T>::operator[](const int Index) const
	{
		return m_Array[Index];
	}


	template<class T>
	inline const T cArray<T>::operator[](const unsigned int Index) const
	{
		return m_Array[Index];
	}


	template<class T>
	inline cArray<T>::operator T* (void)
	{
		return m_Array;
	}


	template<class T>
	inline unsigned int cArray<T>::Size(void) const
	{
		return m_Size;
	}


	template<class T>
	inline unsigned int cArray<T>::Count(void) const
	{
		return m_Count;
	}


	template<class T>
	inline void cArray<T>::Clear(void)
	{
		m_Resize(0, false);
	}



	template<class T>
	inline void cArray<T>::ClearCount(void)
	{
		m_Count = 0;
	}

	template<class T>
	inline void cArray<T>::SetCount(const unsigned int Count)
	{
		// JP must be less or equal
		if (Count <= m_Size)
		{
			m_Count = Count;
		}
	}


	template<class T>
	inline void cArray<T>::Resize(const unsigned int Size, const bool Move)
	{
		m_Resize(Size, Move);
	}


	template<class T>
	inline void cArray<T>::Resize(const unsigned int Size, const unsigned int Count, const bool Move)
	{
		m_Resize(Size, Move);
		m_Count = Count < m_Size? Count: m_Size;
	}


	template<class T>
	void cArray<T>::Move(const T* Array, const unsigned int Count)
	{
		if (m_Size <= Count)
		{
			m_Resize(Count, false);
		}

		if (Count == 1)
		{
			m_Array[0] = Array[0];
		}
		else
		{
			memcpy(m_Array, Array, sizeof(T) * Count);
		}
		m_Count = Count;
	}


	template<class T>
	void cArray<T>::Add(const T* Array)
	{
		if (m_Count + 1 > m_Size)
		{
			m_Resize(m_Count + 1, true);
		}
		m_Array[m_Count++] = *Array;
	}


	template<class T>
	void cArray<T>::Add(const T* Array, const unsigned int Count)
	{
		if (m_Count + Count >= m_Size)
		{
			m_Resize(m_Count + Count, true);
		}
		if (Count == 1)
		{
			m_Array[m_Count++] = *Array;
		}
		else
		{
			memcpy((char *)&m_Array[m_Count], Array, sizeof(T) * Count);
			m_Count += Count;
		}
	}


	template<class T>
	inline void cArray<T>::Left(const unsigned int Count)
	{
		m_Count = Count < m_Count ? Count : m_Count;
	}


	template<class T>
	inline void cArray<T>::Right(const unsigned int Count)
	{
		int tmp = (m_Count < Count? 0: m_Count - Count);
		m_Count = (m_Count < Count? m_Count: Count); 
		memmove(m_Array, (char* )&m_Array[tmp], sizeof(T) * m_Count);
	}


	template<class T>
	inline void cArray<T>::Mid(const unsigned int Left, const unsigned int Right)
	{
		if (Right >= Left && m_Count >= Right - Left)
		{
			m_Count = Right - Left;
			memmove(m_Array, (char* )&m_Array[Left], sizeof(T) * m_Count);
		}
	}



	template<class T>
	inline void cArray<T>::Append(const T& Value)
	{
		if (!(m_Count < m_Size))
		{
			m_Resize(m_Size + 1, true);
		}
		m_Array[m_Count] = Value;
	}


	template<class T>
	inline T* cArray<T>::GetArray(const unsigned int Index)
	{
		return &m_Array[Index];
	}


	template<class T>
	inline T* cArray<T>::GetArray(const unsigned int Index) const
	{
		return &m_Array[Index];
	}


	template<class T>
	void cArray<T>::Fill(const char ch, const unsigned int Count)
	{
		m_Resize(Count, false);
		memset(m_Array, ch, Count * sizeof(T));
		m_Count = Count;
	}

	template<class T>
	inline void cArray<T>::SetDelta(const unsigned int Delta)
	{
		if (m_Delta < sizeof(DELTA)/sizeof(DELTA[0]))
		{
			m_Delta = Delta;
		}
		else
		{
			m_Delta = sizeof(DELTA)/sizeof(DELTA[0]) - 1;
		}
	}

	template<class T>
	inline unsigned int cArray<T>::GetDelta(void) const
	{
		return m_Delta;
	}

	template<class T>
	inline void cArray<T>::SetDynamic(const bool Dynamic)
	{
		m_Dynamic = Dynamic;
	}

	template<class T>
	inline bool cArray<T>::GetDynamic(void) const
	{
		return m_Dynamic;
	}

	template<class T>
	void cArray<T>::m_Resize(const unsigned int Size, const bool Move)
	{
		if (Size > m_Size)
		{
			m_Size = (Size & DELTA[m_Delta]) ? Size + (DELTA[m_Delta] + 1 - (Size & DELTA[m_Delta])): Size;
			T *auxPtr = new T[m_Size];
			if (m_Array != 0)
			{
				if (Move)
				{
					memcpy(auxPtr, m_Array, sizeof(T) * m_Count);
				}
				delete [] m_Array;
			}
			m_Array = auxPtr;
		} 
		else if (m_Dynamic && Size != 0 && Size < m_Size)
		{
			unsigned int tmpSize = (Size & DELTA[m_Delta]) ? Size + (DELTA[m_Delta] + 1 - (Size & DELTA[m_Delta])): Size;
			if (tmpSize < m_Size)
			{
				m_Size = tmpSize;
				if (m_Count > m_Size)
				{
					m_Count = m_Size;
				}
				T *auxPtr = new T[m_Size];
				if (m_Array != 0)
				{
					if (Move)
					{
						memcpy(auxPtr, m_Array, sizeof(T) * m_Count);
					}
					delete [] m_Array;
				}
				m_Array = auxPtr;
			}
		}
		else if (Size == 0)
		{
			if (m_Array != 0)
			{
				delete [] m_Array;
				m_Array = 0;
			}
			m_Size = m_Count = 0;
		}
	}
}

#endif            //    __CARRAY_H__
