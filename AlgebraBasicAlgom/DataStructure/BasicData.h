#pragma once

template<class T>
class MyStack
{
public:
	MyStack()
	{
		m_nReserve = 1;
		m_pData = new T[m_nReserve]();
		m_nTop = -1;
	}

	~MyStack()
	{
		if (m_pData != nullptr)
		{
			delete m_pData;
			m_pData = nullptr;	
		}
	}

	void clear()
	{
		m_nTop = -1;
	}

	bool empty()
	{
		return m_nTop < 0;
	}

	int size()
	{
		return m_nTop + 1;
	}

	T& top()
	{
		assert(!empty());
		return m_pData[m_nTop];
	}

	void push(const T& elem)
	{
		if (m_nTop < m_nReserve - 1)
		{
			m_pData[++m_nTop] = elem;
		}
		else
		{
			m_nReserve <<= 1;
			T* data = new T[m_nReserve]();
			memcpy(data, m_pData, ++m_nTop * sizeof(T));
			delete m_pData;
			m_pData = data;
			m_pData[m_nTop] = elem;
		}
	}

	T pop()
	{
		assert(m_nTop >= 0);
		return m_pData[m_nTop--];
	}

private:
	T* m_pData;
	int m_nReserve;
	int m_nTop;
};

template<class T>
class MyQueue
{
public:
	MyQueue()
	{
		m_nReserve = 2;
		m_pData = new T[m_nReserve]();
		m_nHead = 0;
		m_nTail = 0;
	}
	~MyQueue()
	{
		if (m_pData != nullptr)
		{
			delete m_pData;
			m_pData = nullptr;
		}
	}

	void clear()
	{
		m_nHead = 0;
		m_nTail = 0;
	}

	bool empty()
	{
		return m_nHead == m_nTail;
	}

	bool full()
	{
		return m_nHead == (m_nTail + 1) % m_nReserve;
	}

	int size()
	{
		return (m_nTail - m_nHead + m_nReserve) % m_nReserve;
	}

	T& front()
	{
		assert(!empty());
		return m_pData[m_nHead];
	}

	T& back()
	{
		assert(!empty());
		m_pData[(m_nTail - 1 + m_nReserve) % m_nReserve];
	}

	T dequeue()
	{
		assert(!empty());

		if (m_nHead == m_nReserve - 1)
		{
			m_nHead = 0;
			return m_pData[m_nReserve - 1];
		}
		else
			return m_pData[m_nHead++];
	}

	void enqueue(const T& elem)
	{
		if (full())
		{
			int reserve = m_nReserve << 1;
			T* data = new T[reserve]();
			for (int i = 0; i < m_nReserve - 1; i++)
				data[i] = dequeue();
			m_nHead = 0;
			m_nTail = m_nReserve;
			data[m_nTail - 1] = elem;
			delete m_pData;
			m_pData = data;
			m_nReserve = reserve;
		}
		else
		{
			m_pData[m_nTail] = elem;
			++m_nTail %= m_nReserve;
		}
	}

private:
	T* m_pData;
	int m_nReserve;
	int m_nHead;
	int m_nTail;
};

template<class T>
class MyDQueue
{
public:
	MyDQueue()
	{
		m_nReserve = 2;
		m_pData = new T[m_nReserve]();
		m_nHead = 0;
		m_nTail = 0;
	}

	~MyDQueue()
	{
		if (m_pData != nullptr)
		{
			delete m_pData;
			m_pData = nullptr;
		}
	}

	void clear()
	{
		m_nHead = 0;
		m_nTail = 0;
	}

	bool empty()
	{
		return m_nHead == m_nTail;
	}

	bool full()
	{
		return m_nHead == (m_nTail + 1) % m_nReserve;
	}

	int size()
	{
		return (m_nTail - m_nHead + m_nReserve) % m_nReserve;
	}

	T& front()
	{
		assert(!empty());
		return m_pData[m_nHead];
	}

	T& back()
	{
		assert(!empty());
		return  m_pData[(m_nTail - 1 + m_nReserve) % m_nReserve];
	}

	T headDequeue()
	{
		assert(!empty());

		T& rtn = m_pData[m_nHead];
		m_nHead = (m_nHead + 1) % m_nReserve;
		return rtn;
	}

	T tailDequeue()
	{
		assert(!empty());

		m_nTail = (m_nTail - 1 + m_nReserve) % m_nReserve;
		return m_pData[m_nTail];
	}

	void headEnqueue(const T& elem)
	{
		if (full())
		{
			int reserve = m_nReserve << 1;
			T* data = new T[reserve]();
			for (int i = 0; i < m_nReserve - 1; i++)
				data[reserve - 1 - i] = tailDequeue();
			m_nTail = 0;
			m_nHead = m_nReserve;
			data[m_nHead] = elem;
			delete m_pData;
			m_pData = data;
			m_nReserve = reserve;
		}
		else
		{
			m_nHead = (m_nHead - 1 + m_nReserve) % m_nReserve;
			m_pData[m_nHead] = elem;
		}
	}

	void tailEnqueue(const T& elem)
	{
		if (full())
		{
			int reserve = m_nReserve << 1;
			T* data = new T[reserve]();
			for (int i = 0; i < m_nReserve - 1; i++)
				data[i] = headDequeue();
			m_nHead = 0;
			m_nTail = m_nReserve;
			data[m_nTail - 1] = elem;
			delete m_pData;
			m_pData = data;
			m_nReserve = reserve;
		}
		else
		{
			m_pData[m_nTail] = elem;
			++m_nTail %= m_nReserve;
		}
	}

private:
	T* m_pData;
	int m_nReserve;
	int m_nHead;
	int m_nTail;
};


template<class T>
class MyQueue_
{
public:

	bool empty()
	{
		return m_MainStack.empty();
	}

	T dequeue()
	{
		return m_MainStack.pop();
	}

	void enqueue(const T& elem)
	{
		while (!m_MainStack.empty())
			m_MinorStack.push(m_MainStack.pop());

		m_MainStack.push(elem);
		while (!m_MinorStack.empty())
			m_MainStack.push(m_MinorStack.pop());
	}

private:
	MyStack<T> m_MainStack;
	MyStack<T> m_MinorStack;
};

template<class T>
class MyStack_
{
public:
	MyStack_() :m_nValidIdx(0) {}

	bool empty()
	{
		return m_Queue[m_nValidIdx].empty();
	}

	void push(const T& elem)
	{
		m_Queue[m_nValidIdx].enqueue(elem);
	}

	T pop()
	{
		m_nValidIdx = 1 - m_nValidIdx;
		while (m_Queue[1 - m_nValidIdx].size() > 1)
			m_Queue[m_nValidIdx].enqueue(m_Queue[1 - m_nValidIdx].dequeue());
		return m_Queue[1 - m_nValidIdx].dequeue();
	}

private:
	MyQueue<T> m_Queue[2];
	int m_nValidIdx;
};

template<class T>
struct MyListElem
{
	MyListElem(const T& value_ = T(), MyListElem* prev_ = nullptr, MyListElem* next_ = nullptr) :
		value(value_), prev(prev_), next(next_) {};

	MyListElem* prev;
	T value;
	MyListElem* next;
};


template<class T>
class MyList
{
public:
	MyList() :m_pHead(nullptr), m_pTail(nullptr), m_nSize(0) {}
	
	~MyList()
	{
		while (!empty())
			tailDelete();
	}

	void clear()
	{
		while (m_pHead != nullptr)
			tailDelete();
	}

	bool empty()
	{
	//	return m_pHead == nullptr;
		return m_nSize == 0;
	}

	void headInsert(const T& val)
	{
		MyListElem<T>* elem = new MyListElem<T>(val);
		elem->next = m_pHead;
		if (m_pHead != nullptr)
			m_pHead->prev = elem;
		else
			m_pTail = elem;
		m_pHead = elem;
		m_nSize++;
	}

	void tailInsert(const T& val)
	{
		MyListElem<T>* elem = new MyListElem<T>(val);
		elem->prev = m_pTail;
		if (m_pTail != nullptr)
			m_pTail->next = elem;
		else
			m_pHead = elem;
		m_pTail = elem;
		m_nSize++;
	}

	void midInsert(MyListElem<T>* tgtElem, const T& val)
	{
		if (tgtElem != nullptr)
		{
			MyListElem<T>* elem = new MyListElem<T>(val);
			elem->prev = tgtElem;
			elem->next = tgtElem->next;
			tgtElem->next = elem;
			if (elem->next != nullptr)
				elem->next->prev = elem;
			else
				m_pTail = elem;
		}
		else
		{
			MyListElem<T>* elem = new MyListElem<T>(val);
			m_pHead = elem;
			m_pTail = elem;
		}
		m_nSize++;
	}

	int headDelete()
	{
		if (m_pHead != nullptr)
		{
			MyListElem<T>* dlt = m_pHead;
			m_pHead = m_pHead->next;
			if (m_pHead != nullptr)
				m_pHead->prev = nullptr;
			else
				m_pTail = nullptr;
			delete dlt;
			m_nSize--;
			return 1;
		}
		else
			return 0;
	}

	int tailDelete()
	{
		if (m_pTail != nullptr)
		{
			MyListElem<T>* dlt = m_pTail;
			m_pTail = m_pTail->prev;
			if (m_pTail != nullptr)
				m_pTail->next = nullptr;
			else
				m_pHead = nullptr;
			delete dlt;
			m_nSize--;
			return 1;
		}
		else
			return 0;
	}

	bool contain(MyListElem<T>* tgtElem)
	{
		MyListElem<T>* iter = m_pHead;
		while (iter != nullptr)
		{
			if (iter == tgtElem)
				return true;
			iter = iter->next;
		}
		return false;
	}

	int midDelete(MyListElem<T>* tgtElem)
	{
		if (tgtElem != nullptr)
		{
			if (m_pHead == tgtElem)
			{
				if (m_pTail == tgtElem)
				{
					m_pTail = nullptr;
					m_pHead = nullptr;
				}
				else
				{
					m_pHead = tgtElem->next;
					m_pHead->prev = nullptr;
				}
			}
			else
			{
				if (m_pTail == tgtElem)
				{
					m_pTail = tgtElem->prev;
					m_pTail->next = nullptr;
				}
				else
				{
					if (contain(tgtElem))
					{
						tgtElem->next->prev = tgtElem->prev;
						tgtElem->prev->next = tgtElem->next;
					}
					else
						return -1;
				}
			}
			delete tgtElem;
			tgtElem = nullptr;
			m_nSize--;
			return 1;
		}
		else
			return 0;
	}

	MyListElem<T>* search(const T& val)
	{
		MyListElem<T>* iter = m_pHead;
		while (iter != nullptr && iter->value != val)
			iter = iter->next;
		return iter;
	}

	MyListElem<T>* ptrAt(int idx)
	{
		MyListElem<T>* iter = m_pHead;
		while (idx > 0)
		{
			iter = iter->next;
			idx--;
		}
		return iter;
	}
	
	void insertAt(int idx, const T& val)
	{
		midInsert(ptrAt(idx), val);
	}

	int deleteAt(int idx)
	{
		if (idx >= m_nSize || idx < 0)
			return 0;
		
		return midDelete(ptrAt(idx));
	}

	T& at(int idx)
	{
		MyListElem<T>* iter = m_pHead;
		while (idx > 0)
		{
			iter = iter->next;
			idx--;
		}
		return iter->value;
	}

	MyListElem<T>* head()
	{
		return m_pHead;
	}

	MyListElem<T>* tail()
	{
		return m_pTail;
	}

	int size()
	{
		return m_nSize;
	}

private:
	MyListElem<T>* m_pHead;
	MyListElem<T>* m_pTail;
	int m_nSize;
};

template<class T>
class MyCList
{
public:
	MyCList() :m_nSize(0)
	{
		m_pSentinel = new MyListElem<T>();
		m_pSentinel->prev = m_pSentinel;
		m_pSentinel->next = m_pSentinel;
	}

	~MyCList()
	{
		while (!empty())
			tailDelete();
		delete m_pSentinel;
	}

	void clear()
	{
		while (m_nSize > 0)
			tailDelete();
	}

	bool empty()
	{
		return m_nSize == 0;
	}

	int size()
	{
		return m_nSize;
	}

	bool contain(MyListElem<T>* tgtElem)
	{
		MyListElem<T>* iter = m_pSentinel->next;
		while (iter != m_pSentinel)
		{
			if (iter == tgtElem)
				return true;
			iter = iter->next;
		}
		return false;
	}
	
	void headInsert(const T& val)
	{
		MyListElem<T>* elem = new MyListElem<T>(val);
		elem->next = m_pSentinel->next;
		elem->prev = m_pSentinel;
		m_pSentinel->next->prev = elem;
		m_pSentinel->next = elem;
		m_nSize++;
	}

	void tailInsert(const T& val)
	{
		MyListElem<T>* elem = new MyListElem<T>(val);
		elem->prev = m_pSentinel->prev;
		elem->next = m_pSentinel;
		m_pSentinel->prev->next = elem;
		m_pSentinel->prev = elem;
		m_nSize++;
	}

	void midInsert(MyListElem<T>* tgtElem, const T& val)
	{
		assert(contain(tgtElem));

		MyListElem<T>* elem = new MyListElem<T>(val);
		elem->next = tgtElem->next;
		elem->prev = tgtElem;
		tgtElem->next->prev = elem;
		tgtElem->next = elem;
		m_nSize++;
	}

	int headDelete()
	{
		if (m_nSize == 0)
			return 0;

		MyListElem<T>* dlt = m_pSentinel->next;
		dlt->next->prev = m_pSentinel;
		m_pSentinel->next = dlt->next;
		delete dlt;
		m_nSize--;
		return 1;
	}

	int tailDelete()
	{
		if (m_nSize == 0)
			return 0;

		MyListElem<T>* dlt = m_pSentinel->prev;
		dlt->prev->next = m_pSentinel;
		m_pSentinel->prev = dlt->prev;
		delete dlt;
		m_nSize--;
		return 1;
	}

	int midDelete(MyListElem<T>* tgtElem)
	{
		if (!contain(tgtElem))
			return 0;

		tgtElem->next->prev = tgtElem->prev;
		tgtElem->prev->next = tgtElem->next;
		delete tgtElem;
		m_nSize--;
	}

	MyListElem<T>* search(const T& val)
	{
		if (m_nSize == 0)
			return nullptr;
		MyListElem<T>* iter = m_pSentinel->next;
		while (iter->value != val)
		{
			iter = iter->next;
			if (iter == m_pSentinel)
				return nullptr;
		}
		return iter;
	}

	MyListElem<T>* ptrAt(int idx)
	{
		assert(idx >= 0 && idx < m_nSize);

		MyListElem<T>* iter = m_pSentinel->next;
		while (idx > 0)
		{
			iter = iter->next;
			idx--;
		}
		return iter;
	}

	void insertAt(int idx, const T& val)
	{
		midInsert(ptrAt(idx), val);
	}

	int deleteAt(int idx)
	{
		return midDelete(ptrAt(idx));
	}

	T& at(int idx)
	{
		assert(idx >= 0 && idx < m_nSize);

		MyListElem<T>* iter = m_pSentinel->next;
		while (idx > 0)
		{
			iter = iter->next;
			idx--;
		}
		return iter->value;
	}

private:
	MyListElem<T>* m_pSentinel;
	int m_nSize;
};

class TestClass0
{
public:
	TestClass0(int a_ = 0, int b_ = 0) :a(a_), b(b_) {}
	~TestClass0()
	{
		a = 0;
		b = 0;
	}

	int sum() { return a + b; }

	int diff() { return a - b; }

	void setA(int a_) { a = a_; }

private:
	int a;
	int b;
};


template <class T>
class SmartPointer
{
public:
	~SmartPointer() { delete _p; }
	T* operator->() { return _p; }
	const T* operator->() const { return _p; }

	SmartPointer& operator=(const SmartPointer& ptr)
	{
		delete _p;
		_p = ptr.release();
	}

	T* release()
	{
		T* temp = _p;
		_p = nullptr;
		return temp;
	}

//protected:
	SmartPointer() : _p(0) {}
	SmartPointer(const SmartPointer& ptr) { _p = ptr.release(); }
	explicit SmartPointer(T* p) : _p(p) {}

	T* _p;
};
