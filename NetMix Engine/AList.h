#ifndef __AList_h__
#define __AList_h__

//#include "Lock.h"

namespace Atom {

template <class T>
class AListItem
{
public:
	T m_data;
	AListItem *m_next;
};

template <class T>
class AList
{
public:
	AList(): m_begin(0)
		, m_end(0)
		, m_numItems(0) {}

	T operator[](int i) const { return at(i); }
	
	void append(T data)
	{
		//Lock lock((CriticalSection*)&criticalSection);

		AListItem<T> *newItem = new AListItem<T>;
		newItem->m_data = data;
		newItem->m_next = 0;

		if(m_end) {
			m_end->m_next = newItem;
		} else {
			m_begin = newItem;
		}
		m_end = newItem;

		m_numItems++;
	}

	T at(int position) const
	{
		//Lock lock((CriticalSection*)&criticalSection);

		//printf("List<T>::at(%d) %d\n", position, m_numItems);
		T data;
		AListItem<T> *item = m_begin;
		for(int i=0; i<m_numItems; i++) {
			if(position == i) {
				data = item->m_data;
				break;
			}
			item = item->m_next;
		}
		return data;
	}

	void insertAt(int position, T data)
	{
		//Lock lock((CriticalSection*)&criticalSection);

		//printf("List<T>::insertAt(%d)\n", position);
		AListItem<T> *newItem = new AListItem<T>;
		newItem->m_data = data;
		newItem->m_next = 0;

		if(!m_begin || position >= m_numItems) {
			//printf("Append\n");
			append(data);
		} else {
			//printf("Insert\n");
			AListItem<T> *previousItem = 0;
			AListItem<T> *item = m_begin;
			for(int i=0; i<m_numItems; i++) {
				if(i==position) {
					if(previousItem) {
						previousItem->m_next = newItem;
					} else {
						m_begin = newItem;
					}
					newItem->m_next = item;

					m_numItems++;
					break;
				}
				previousItem = item;
				item = item->m_next;
			}
		}
		//printf("EXIT: List<T>::insertAt()\n");
	}

	void removeAt(int position)
	{
		//Lock lock((CriticalSection*)&criticalSection);

		if(position <= m_numItems) {
			AListItem<T> *previousItem = 0;
			AListItem<T> *item = m_begin;
			for(int i=0; i<m_numItems; i++) {
				if(position == i) {

					if(previousItem) {
						previousItem->m_next = item->m_next;
					} else {
						m_begin = item->m_next;
					}

					if(m_end == item) {
						if(previousItem) {
							m_end = previousItem;
						} else { 
							m_end = 0;
						}
					}

					delete item;

					m_numItems--;
					break;
				}
				previousItem = item;
				item = item->m_next;
			}
		}
	}

	void clear(void)
	{
		//Lock lock((CriticalSection*)&criticalSection);

		AListItem<T> *item = m_begin;
		while(item) {
			AListItem<T> *nextItem = item->m_next;
			delete item;
			m_numItems--;
			item = nextItem;
		}
		m_begin = 0;
		m_end = 0;
	}

	//T begin (
	int size(void) const
	{
		//Lock lock((CriticalSection*)&criticalSection);
		
		return m_numItems;
	}

private:
	AListItem<T> *m_begin;
	AListItem<T> *m_end;
	int m_numItems;
	//CriticalSection criticalSection;
};

}

#endif