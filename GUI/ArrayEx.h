/************************************************************

  ArrayEx.h

  "CArrayEx", a template class that does exactly what "CArray"
  does except that:

  a) No need to link MFC libraries, can be used in non-MFC
     applications.
  b) Added member function "Sort", which sorts stored
     elements using the "quick sort" algorithm(requiring
	 operators ">" being defined for stored element data type.
  c) Added member function "Find", which searches for given
     element(s) and returns result index(-1 if not found).
  d) The constant operator[] now returns "const TYPE&" instead of
     a "TYPE" instance, which was the case in "CArray".
  e) Overloaded operator "=".

  Special Note: The "QuickSort" method was developed by Martin Ziacek.
 
  2002-03-20		Initial release.
  2002-06-11		Changed "__QuickSortRecursive" member function into
					static so that operator ">" need to be defined only if
					member "Sort" is called.
  2003-02-16		Almost a complete rewrite, now elements(objects) are
					automatically allocated in heap upon insertion, and m_pData
					stores pointers of objects instead of objects themselves,
					this change should bring a performance boost while doing
					element-shift operation. 
  
  Bin Liu, abinn32@yahoo.com

*************************************************************/

#ifndef __CARRAYEX_H__
#define __CARRAYEX_H__

#include <assert.h>
#include <memory.h>

#ifndef NULL
	#define NULL	(0)
#endif

const size_t PTRSIZE = sizeof(void*);

template <class TYPE, class ARG_TYPE>
class CArrayEx
{
public:

	// Constructions
	CArrayEx();
	CArrayEx(const CArrayEx& rhs);
	virtual ~CArrayEx();

	// Searching and sorting
	int Find(ARG_TYPE tData, int nStartIndex = 0) const;
	int SortedFind(ARG_TYPE tData, int nStartIndex = 0) const; // find quicker if sorted
	int Find(const TYPE* segment, int nSegmentSize, int nStartIndex = 0) const;
	int Find(const CArrayEx& segment, int nStartIndex = 0) const;
	enum { SORT_NONE = 0, SORT_ASCENDING, SORT_DESCENDING }; // Sort stats
	void Sort(bool bAscending = true);
	int GetSortedStats() const { return m_nCurrentSort; }

	// Attributes
	int GetSize() const	{ return m_nElementCount; }
	int GetUpperBound() const { return m_nElementCount - 1; }
	void SetSize(int nNewSize);
	bool IsEmpty() const { return m_nElementCount == 0; }
	bool IsIndexValid(int nIndex) const { return nIndex >= 0 && nIndex < m_nElementCount; }

	// Operations
	void FreeExtra();
	void RemoveAll() { m_nElementCount = 0; }

	// Element Access
	const TYPE& GetAt(int nIndex) const;
	void SetAt(int nIndex, ARG_TYPE tData);
	TYPE& ElementAt(int nIndex);

	// Growing the Array
	void SetAtGrow(int nIndex, ARG_TYPE tData);
	int Add(ARG_TYPE tData);
	int Append(const CArrayEx& src);
	void Copy(const CArrayEx& src);

	// Insertion/Removal
	void InsertAt(int nIndex, ARG_TYPE tData, int nCount = 1);
	void InsertAt(int nStartIndex, const CArrayEx* pNewArray);
	void RemoveAt(int nIndex, int nCount = 1);

	// Operators
	TYPE& operator[](int nIndex) { return ElementAt(nIndex); }
	const TYPE& operator[](int nIndex) const { return GetAt(nIndex); }
	const CArrayEx& operator=(const CArrayEx& src);

protected:

	// Protected members for internal call only.

	// frees element(s) memory
	void __FreeMemory(int nIndex);
	void __FreeAllMemory();

	// right-shifts elements
	void __RightShift(int nStartAt, int nCount = 1);

	// dynamically grow array size
	void __GrowSize(int nAllocSize =-1);

	// quick sort algorithm
	static void __QuickSortRecursive(TYPE** pArray, int nLow, int nHigh, bool bAscending);

	// searching methods
	static int __QuickFind(ARG_TYPE tData, TYPE** pArray, int nLow, int nHigh, bool bAscending);
	static int __LinearFind(ARG_TYPE tData, TYPE** pArray, int nLow, int nHigh);
	
	// initial array size and growing multiplier
	enum { __INIT_SIZE = 32, __GROW_RATE = 2 };

	// member data
	int m_nCurrentSort; // current sort stats
	int m_nElementCount; // element count
	int m_nAllocSize; // allocated size
	TYPE** m_pData; // pointers to stored data elements
};

// the "quick sort" method developed by Martin Ziacek
template <class TYPE, class ARG_TYPE>
void CArrayEx<TYPE, ARG_TYPE>::__QuickSortRecursive(TYPE** pArray, int nLow, int nHigh, bool bAscending)
{
	// recursive quick sort method, by Martin Ziacek.
	int i,j;
	TYPE* str;

	i = nHigh;
	j = nLow;

	str = pArray[((int) ((nLow+nHigh) / 2))];

	do
	{
		if (bAscending)
		{
			while (*str > *(pArray[j]))
				j++;
			while (*(pArray[i]) > *str)
				i--;
		}
		else
		{
			while (*(pArray[j]) > *str)
				j++;
			while (*str > *(pArray[i]))
				i--;
		}

		if (i >= j)
		{

			if ( i != j )
			{
				TYPE* zal = pArray[i];
				pArray[i] = pArray[j];
				pArray[j] = zal;
			}

			i--;
			j++;
		}

	} while (j <= i);

	if (nLow < i) __QuickSortRecursive(pArray, nLow,i,bAscending);
	if (j < nHigh) __QuickSortRecursive(pArray, j,nHigh,bAscending);
}

template <class TYPE, class ARG_TYPE>
void CArrayEx<TYPE, ARG_TYPE>::Sort(bool bAscending)
{
	if ((bAscending && m_nCurrentSort == SORT_ASCENDING)
		|| (!bAscending && m_nCurrentSort == SORT_DESCENDING))
		return; // Already sorted

	if (m_nElementCount > 1)
	{
		int low = 0, high = m_nElementCount - 1;
		__QuickSortRecursive(m_pData, low, high, bAscending);
	}

	m_nCurrentSort = bAscending ? SORT_ASCENDING : SORT_DESCENDING;
}

template <class TYPE, class ARG_TYPE>
CArrayEx<TYPE, ARG_TYPE>::CArrayEx()
{
	m_nElementCount = 0;
	m_nAllocSize = 0;
	m_nCurrentSort = SORT_NONE;
	m_pData = NULL;	
};

template <class TYPE, class ARG_TYPE>
CArrayEx<TYPE, ARG_TYPE>::CArrayEx(const CArrayEx& rhs)
{
	m_nCurrentSort = rhs.m_nCurrentSort;
	m_nElementCount = rhs.m_nElementCount;
	m_nAllocSize = rhs.m_nAllocSize;
	m_pData = new TYPE*[m_nAllocSize];
	memset(m_pData, NULL, m_nAllocSize * PTRSIZE);

	for (int i = 0; i < m_nElementCount; i++)
		m_pData[i] = new TYPE(rhs[i]);
}

template <class TYPE, class ARG_TYPE>
CArrayEx<TYPE, ARG_TYPE>::~CArrayEx()
{
	__FreeAllMemory();
};

template <class TYPE, class ARG_TYPE>
void CArrayEx<TYPE, ARG_TYPE>::__FreeMemory(int nIndex)
{
	assert(nIndex >= 0 && nIndex < m_nAllocSize);
	if (m_pData[nIndex] != NULL)
	{
		delete m_pData[nIndex];
		m_pData[nIndex] = NULL;
	}
}

template <class TYPE, class ARG_TYPE>
void CArrayEx<TYPE, ARG_TYPE>::__FreeAllMemory()
{
	for (int i = 0; i < m_nAllocSize; i++)
	{
		if (m_pData[i] != NULL)
		{
			delete m_pData[i];
			m_pData[i] = NULL;
		}
	}

	delete [] m_pData;
	m_pData = NULL;
}

template <class TYPE, class ARG_TYPE>
void CArrayEx<TYPE, ARG_TYPE>::SetSize(int nNewSize)
{
	assert (nNewSize >= 0);
	if (nNewSize > m_nElementCount)
	{	
		if (nNewSize > m_nAllocSize)
			__GrowSize(nNewSize);
	
		for (int i = m_nElementCount; i < nNewSize; i++)
		{
			if (m_pData[i] == NULL)
				m_pData[i] = new TYPE; // allocate objects with default constructor
		}

		m_nCurrentSort = SORT_NONE; // who knows what are in those newly allocated memory?
	}

	m_nElementCount = nNewSize;
};

template <class TYPE, class ARG_TYPE>
int CArrayEx<TYPE, ARG_TYPE>::Add(ARG_TYPE tData)
{
	// append at the end
	m_nCurrentSort = SORT_NONE;
	InsertAt(m_nElementCount, tData, 1);
	return m_nElementCount - 1;
}


template <class TYPE, class ARG_TYPE>
int CArrayEx<TYPE, ARG_TYPE>::Find(ARG_TYPE tData, int nStartIndex/*=0*/) const
{
	if (!IsIndexValid(nStartIndex))
		return -1;

	return __LinearFind(tData, m_pData, nStartIndex, m_nElementCount - 1);
}

template <class TYPE, class ARG_TYPE>
int CArrayEx<TYPE, ARG_TYPE>::SortedFind(ARG_TYPE tData, int nStartIndex/*=0*/) const
{
	if (!IsIndexValid(nStartIndex))
		return -1;

	if (m_nCurrentSort == SORT_NONE)
		return __LinearFind(tData, m_pData, nStartIndex, m_nElementCount - 1);
	else
		return __QuickFind(tData, m_pData, nStartIndex, m_nElementCount - 1, (m_nCurrentSort == SORT_ASCENDING));
}

template <class TYPE, class ARG_TYPE>
int CArrayEx<TYPE, ARG_TYPE>::Find(const TYPE* segment, int nSegmentSize, int nStartIndex /*=0*/) const
{
	if (m_nElementCount < nSegmentSize || !IsIndexValid(nStartIndex))
		return -1;

	bool bFind = false;

	for (int i = nStartIndex; i <= m_nElementCount - nSegmentSize; i++)
	{
		for (int j = 0, bFind = true; j < nSegmentSize; j++)
		{
			if (segment[j] != *(m_pData[i + j]))
			{
				// not match, stop right here
				bFind = false;
				break;
			}
		}

		if (bFind)
			return i; // the first match position
	}

	return -1;
}

template <class TYPE, class ARG_TYPE>
int CArrayEx<TYPE, ARG_TYPE>::Find(const CArrayEx& segment, int nStartIndex/*=0*/) const
{
	if (m_nElementCount < segment->GetSize() || !IsIndexValid(nStartIndex))
		return -1;
	
	bool bFind = false;

	for (int i = nStartIndex; i <= m_nElementCount - segment->GetSize(); i++)
	{
		for (int j = 0, bFind = true; j < segment->GetSize(); j++)
		{
			if (segment[j] != *(m_pData[i + j]))
			{
				// not match, stop right here
				bFind = false;
				break;
			}
		}

		if (bFind)
			return i; // the first match position
	}

	return -1;	
}

// insert single element
template <class TYPE, class ARG_TYPE>
void CArrayEx<TYPE, ARG_TYPE>::InsertAt(int nIndex, ARG_TYPE tData, int nCount)
{
	assert(nIndex >= 0 && nCount >= 0);
	m_nCurrentSort = SORT_NONE;

	int nNewSize = nCount;
	if (nIndex < m_nElementCount)
		nNewSize += m_nElementCount;
	else
		nNewSize += nIndex;

	if (nNewSize > m_nAllocSize)
        __GrowSize(nNewSize);
	
	if (nIndex < m_nElementCount)
		__RightShift(nIndex, nCount);

	for (int i = 0; i < nCount; i++)
	{
		__FreeMemory(nIndex + i);
		m_pData[nIndex + i] = new TYPE(tData);	
	}

	m_nElementCount = nNewSize;
}

// insert a whole array
template <class TYPE, class ARG_TYPE>
void CArrayEx<TYPE, ARG_TYPE>::InsertAt(int nStartIndex, const CArrayEx* pNewArray)
{
	assert(nStartIndex >= 0);
	m_nCurrentSort = SORT_NONE;
	
	const int nCount = pNewArray->GetSize();
	const int nNewSize = nStartIndex + nCount + 1;

	if (nNewSize > m_nAllocSize)
        __GrowSize(nNewSize);
	
	if (nStartIndex < m_nElementCount)
		__RightShift(nStartIndex, nCount);

	for (int i = 0; i < nCount; i++)
	{
		__FreeMemory(nStartIndex + i);
		m_pData[nStartIndex + i] = new TYPE(pNewArray->GetAt(i));	
	}

	m_nElementCount = nNewSize;
}

template <class TYPE, class ARG_TYPE>
void CArrayEx<TYPE, ARG_TYPE>::RemoveAt(int nIndex, int nCount)
{
	if (nCount <= 0)
		return;

	assert(IsIndexValid(nIndex));
	assert(m_nElementCount >= nIndex + nCount);	
	int i = 0;

	for (i = 0; i < nCount; i++)
		__FreeMemory(nIndex + i);

	const int nSegmentSize = m_nElementCount - nIndex - nCount;
	for (i = 0; i < nSegmentSize; i++)
		m_pData[nIndex + i] = m_pData[nIndex + i + nCount];

	for (i = 0; i < nCount; i++)
		m_pData[m_nElementCount - i - 1] = NULL;

	m_nElementCount -= nCount;
}

template <class TYPE, class ARG_TYPE>
void CArrayEx<TYPE, ARG_TYPE>::FreeExtra()
{
	if (m_nAllocSize == m_nElementCount)
		return;

	// reallocate data array with optimized size
	m_nAllocSize = (m_nElementCount < __INIT_SIZE) ? __INIT_SIZE : m_nElementCount;
	TYPE** p = new TYPE*[m_nAllocSize];
	memset(p, NULL, m_nAllocSize * PTRSIZE);
	memcpy(p, m_pData, m_nElementCount * PTRSIZE);
	delete [] m_pData;
	m_pData = p;
}

template <class TYPE, class ARG_TYPE>
void CArrayEx<TYPE, ARG_TYPE>::Copy(const CArrayEx& src)
{
	__FreeAllMemory();
	m_nCurrentSort = src.m_nCurrentSort;
	m_nElementCount = src.m_nElementCount;
	m_nAllocSize = src.m_nAllocSize;
	m_pData = new TYPE*[m_nAllocSize];
	memset(m_pData, NULL, m_nAllocSize * PTRSIZE);

	for (int i = 0; i < m_nElementCount; i++)
		m_pData[i] = new TYPE(src[i]);
}

template <class TYPE, class ARG_TYPE>
int CArrayEx<TYPE, ARG_TYPE>::Append(const CArrayEx& src)
{
	int nRet = src.m_nElementCount > 0 ? m_nElementCount : -1;

	if (nRet == -1)
		return nRet;

	m_nCurrentSort = SORT_NONE;	
	__GrowSize(m_nElementCount + src.m_nElementCount);

	// append at the end
	for (int i = 0; i < src.m_nElementCount; i++)
		m_pData[m_nElementCount++] = new TYPE(src[i]);
	
	return nRet;
}

template <class TYPE, class ARG_TYPE>
void CArrayEx<TYPE, ARG_TYPE>::__RightShift(int nStartAt, int nCount /*= 1*/)
{
	if (m_nElementCount == 0 || nCount <= 0)
		return;

	 assert(IsIndexValid(nStartAt));

	 for (int i = m_nElementCount - 1; i >= nStartAt; i--)
		 m_pData[i + nCount] = m_pData[i];
	 
	 memset(m_pData + nStartAt * PTRSIZE, NULL, nCount * PTRSIZE);
}

template <class TYPE, class ARG_TYPE>
void CArrayEx<TYPE, ARG_TYPE>::__GrowSize(int nAllocSize /*=-1*/)
{
	// determine the new allocate array size
	if (m_nAllocSize == 0)
		m_nAllocSize = __INIT_SIZE;
	do
	{
		m_nAllocSize *= __GROW_RATE;
	} while (m_nAllocSize < nAllocSize);

	TYPE** p = new TYPE*[m_nAllocSize];
	memset(p, NULL, m_nAllocSize * PTRSIZE);

	if (m_pData != NULL)
	{
		memcpy(p, m_pData, m_nElementCount * PTRSIZE);
		delete [] m_pData;
	}

	m_pData = p;
}

template <class TYPE, class ARG_TYPE>
void CArrayEx<TYPE, ARG_TYPE>::SetAt(int nIndex, ARG_TYPE tData)
{
	assert(IsIndexValid(nIndex));
	m_nCurrentSort = SORT_NONE;
	if (m_pData[nIndex] == NULL)
		m_pData[nIndex] = new TYPE(tData);
	else
		*(m_pData[nIndex]) = tData;
}

template <class TYPE, class ARG_TYPE>
void CArrayEx<TYPE, ARG_TYPE>::SetAtGrow(int nIndex, ARG_TYPE tData)
{
	assert(nIndex >= 0);
	m_nCurrentSort = SORT_NONE;
	if (nIndex < m_nElementCount)
		SetAt(nIndex, tData);
	else
		InsertAt(nIndex, tData, 1);
}

template <class TYPE, class ARG_TYPE>
TYPE& CArrayEx<TYPE, ARG_TYPE>::ElementAt(int nIndex)
{
	assert(IsIndexValid(nIndex));
	m_nCurrentSort = SORT_NONE;
	if (m_pData[nIndex] == NULL)
		m_pData[nIndex] = new TYPE;
	return *(m_pData[nIndex]);
}

template <class TYPE, class ARG_TYPE>
const TYPE& CArrayEx<TYPE, ARG_TYPE>::GetAt(int nIndex) const
{
	assert(IsIndexValid(nIndex));
	assert(m_pData[nIndex] != NULL);
	return *(m_pData[nIndex]);
}

template <class TYPE, class ARG_TYPE>
const CArrayEx<TYPE, ARG_TYPE>& CArrayEx<TYPE, ARG_TYPE>::operator=(const CArrayEx& src)
{
	Copy(src);
	return *this;
}

template <class TYPE, class ARG_TYPE>
int CArrayEx<TYPE, ARG_TYPE>::__QuickFind(ARG_TYPE tData, TYPE** pArray, int nLow, int nHigh, bool bAscending)
{
	if (nHigh > nLow + 1) // the most likely case: three or more elements unchecked
	{
		const int POS = (nLow + nHigh) / 2;
		if (*(pArray[POS]) > tData)
		{
			if (bAscending)
			{
				return __QuickFind(tData, pArray, nLow, POS, true);
			}
			else
			{
				return __QuickFind(tData, pArray, POS, nHigh, false);
			}
		}
		else if (tData > *(pArray[POS]))
		{
			if (bAscending)
			{
				return __QuickFind(tData, pArray, POS, nHigh, true);
			}
			else
			{
				return __QuickFind(tData, pArray, nLow, POS, false);
			}
		}
		else
		{
			return POS;
		}
	}
	else if (nHigh == nLow + 1) // two elements unchecked
	{
		if (*(pArray[nLow]) == tData)
			return nLow;
		else if (*(pArray[nHigh]) == tData)
			return nHigh;
		else
			return -1;
	}
	else if (nHigh == nLow) // one element unchecked
	{
		return (*(pArray[nLow]) == tData) ? nLow : -1;
	}
	else // zero element unchecked
	{
		return -1;
	}
}

template <class TYPE, class ARG_TYPE>
int CArrayEx<TYPE, ARG_TYPE>::__LinearFind(ARG_TYPE tData, TYPE** pArray, int nLow, int nHigh)
{
	for (int i = nLow; i <= nHigh; i++)
	{
		if (*(pArray[i]) == tData)
			return i;
	}
	return -1;
}

#endif
