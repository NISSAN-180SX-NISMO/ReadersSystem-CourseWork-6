#pragma once
#include <string>
#include <vector>
#include <stdexcept>

enum class GIVE {OUTT, BACK};

struct archive 
{
	std::string ticket;			// Номер читательского билета в формате ANNNN-YY
	std::string code;			// Шифр в формате NNN.MMM
	std::string give_out_date;	// Дата выдачи
	std::string give_back_date;	// Дата возврата
	GIVE event;
};

class List 
{
public: 
	static const int Hash(std::string key) {
		key.erase(3, 1);
		return atoi(key.c_str());
	}
private:
	struct node 
	{
		archive Archive;
		int key;
		node* next = nullptr;
	};
	node* Head = nullptr;
	node* Tail = nullptr;
	void insert(int prev_index, node* current) {
		if (prev_index == -1)
		{
			current->next = Head;
			Tail->next = current;
			Head = current;
		}
		else
		{
			node* prev = this->operator[](prev_index);
			if (prev == Tail)
			{
				current->next = Head;
				Tail->next = current;
				Tail = Tail->next;
			}
			else {
				current->next = prev->next == current ? current->next : prev->next;
				prev->next = current;
			}
		}
		
	}
	void swap(node* l, node* r)
	{
		if (l == r) return;
		node* new_l, * new_r;
		if (l->next == r) 
		{
			new_r = new node{ l->Archive, l->key, r->next };
			new_l = new node{ r->Archive, r->key, new_r };
		}
		else
		{
			new_r = new node{ l->Archive, l->key, r->next };
			new_l = new node{ r->Archive, r->key, l->next };
		}
		node* current = Head;
		do 
		{
			if (!current) break;
			if (current->next == l) {
				if (l->next == r) {
					if (current->next->next == Tail)
					{
						current->next = new_l;
						Tail = current->next->next;
					}
					if (current->next == Tail) 
					{
						Tail = new_l;
						current->next = Tail;
						Head = new_r;
					}
					else if (current->next == Head) 
					{
						Head = new_l;
					}
					else 
					{
						current->next = new_l;
					}
				}
				else
				{
					if (l == Head && r == Tail)
					{
						Head = new_l;
						Tail = new_r;
					}
					else if (l == Head)
					{
						Head = new_l;
						Tail->next = Head;
						while (current->next != r)
							current = current->next;
						current->next = new_r;
					}
					else if (r == Tail)
					{
						current->next = new_l;
						while (current->next != r)
							current = current->next;
						Tail = new_r;
						current->next = Tail;
					}
					else
					{
						current->next = new_l;
						while (current->next != r)
							current = current->next;
						current->next = new_r;
					}
				}
			}
			current = current->next;
		} while (current != Head);
	}
	node* operator[](int index) {
		node* current = Head;
		while (index) {
			current = current->next;
			--index;
		}
		return current;
	}
	int size = 0;
public:
	~List() {
		while (Head)
			remove();
	}
public:
	
	void sort()
	{
		int index_min;                                       //индекс для минимального элемента

		//Сортировка массива "вставкой"
		for (int i = 0; i < size - 1; i++) {
			index_min = i;                    //За минимальный берем первый из неотсортированных
			for (int j = i + 1; j < size; j++)  //Начинаем проверять с последующего за ним элемента все оставшиеся
			{
				if (this->operator[](j)->key < this->operator[](index_min)->key)      //если какой-то из них меньше,...
				{
					index_min = j;             //...то запоминаем его номер
				}
			}
			if (index_min != i) {              //если в ходе проверки индекс минимального элемента поменялся, меняем местами жлементы
				swap(this->operator[](i), this->operator[](index_min));
			}
		}
	}
	void set(archive Archive) 
	{
		if (!Head)
			Head = Tail = new node{ Archive, Hash(Archive.code), Head };
		else if (Head->next == Head)
			Head->next = Tail = new node{ Archive, Hash(Archive.code), Head };
		else {
			Tail->next = new node{ Archive, Hash(Archive.code), Head };
			Tail = Tail->next;
		}
		++size;
	}
	void remove()	// Удаляет голову
	{
		if (Head->next == Head)
			Head = nullptr;
		else {
			Tail->next = Head->next;
			delete Head;
			Head = Tail->next;
		}
	}
	archive* find(const std::string& ticket, const std::string& key, GIVE event)
	{
		if (!Head) return nullptr;
		node* current = Head;
		do {
			if (current->Archive.ticket == ticket && current->key == Hash(key) && current->Archive.event == event)
				return & current->Archive;
			current = current->next;
		} while (current != Head);
		return nullptr;
	}
	std::vector<archive*> find_all(const std::string& key, GIVE event)
	{
		std::vector<archive*> all;
		if (!Head) return all;
		node* current = Head;
		do {
			if (current->key == Hash(key) && current->Archive.event == event)
				all.push_back(&current->Archive);
			current = current->next;
		} while (current != Head);
		return all;
	}
	std::vector<archive*> find_all(const std::string& ticket)
	{
		std::vector<archive*> all;
		if (!Head) return all;
		node* current = Head;
		do {
			if (current->Archive.ticket == ticket)
				all.push_back(&current->Archive);
			current = current->next;
		} while (current != Head);
		return all;
	}
	std::vector<archive*> get_all()
	{
		std::vector<archive*> all;
		if (!Head) return all;
		node* current = Head;
		do {
			all.push_back(&current->Archive);
			if (current->next)
				current = current->next;
		} while (current != Head);
		return all;
	}
};