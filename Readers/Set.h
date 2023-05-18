#pragma once
#include <string>
#include <vector>
#include <stdint.h>

class DuplicateReader : std::exception {
private:
	std::string more;
public:
	DuplicateReader(std::string key) { this->more = key; }
	const std::string get_more() const noexcept {
		return "Попытка добавить дубликат читателя! Билет: " + more;
	}
};

struct reader 
{
	std::string ticket;		// Номер читательского билета в формате ANNNN-YY
	std::string name;		// ФИО 
	int year;				// Год рождения
	std::string address;	// Адрес
	std::string work;		// Место работы или учёбы
};

class Set 
{
public:
	const static int SIZE;
	const static int Hash(std::string key) 
	{
		const int p = 31;
		const int m = 1e9 + 9;
		uint64_t hash = 0;
		uint64_t power = 1;
		for (char c : key)
		{
			hash = (hash + (c - 'a' + 1) * power) % m;
			power = (power * p) % m;
		}
		return hash % SIZE;
	}
private:
	struct node 
	{
		reader Reader;			// значение
		std::string key;		// ключ
		node* next = nullptr;	// указатель на след. эл.
	};
	std::vector<node*> Table;
	node* set(const reader& Reader, node* current) {
		if (!current) return new node{ Reader, Reader.ticket };
		if (current->key == Reader.ticket) throw DuplicateReader(Reader.ticket);
		else current->next = set(Reader, current->next);
		return current;
	}
	void remove(std::string key, node*& current) 
	{
		if (!current) return;
		if (current->key != key) remove(key, current->next);
		else {								//   
			node* tmp = current;            // 0---0
			current = current->next;
			delete tmp;
		}
	}
public:
	Set() 
	{
		Table.resize(SIZE, nullptr);
	}
	~Set()
	{
		for (auto& Node : Table) {
			while (Node)
				remove(Node->key, Node);
		}
	}
public:
	void set(reader Reader) {
		int index = Hash(Reader.ticket);
		if (!Table[index]) Table[index] = new node{ Reader, Reader.ticket };
		else Table[index] = set(Reader, Table[index]);
	}
	void remove(std::string key)
	{
		int index = Hash(key);
		remove(key, Table[index]);
	}
	reader* find(std::string ticket)
	{
		int index = Hash(ticket);
		node* current = Table[index];
		while (current) 
		{
			if (current->key == ticket) 
				return &current->Reader;
			current = current->next;
		}
		return nullptr;
	}
	std::vector<reader*> find_all(std::string name)
	{
		std::vector<reader*> all;
		for (auto& Node : Table) 
		{
			node* current = Node;
			while (current)
			{
				if (current->Reader.name == name)
					all.push_back(& current->Reader);
				current = current->next;
			}
		}
		return all;
	}
	std::vector<reader*> get_all()
	{
		std::vector<reader*> all;
		for (auto& Node : Table) {
			if (Node) {
				node* current = Node;
				while (current) {
					all.push_back(&current->Reader);
					current = current->next;
				}
			}
		}
		return all;
	}
};

const int Set::SIZE = 1024;