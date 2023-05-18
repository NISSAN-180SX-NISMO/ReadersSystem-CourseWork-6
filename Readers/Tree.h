#pragma once
#include <string>
#include <vector>
#include <stdexcept>

class DuplicateBook : std::exception {
private: 
	std::string more;
public:
	DuplicateBook(std::string key) { this->more = key; }
	const std::string get_more() const noexcept {
		return "Попытка добавить дубликат книги! Шифр: " + more;
	}
};

struct book {
	std::string code;	// Шифр в формате NNN.MMM
	std::string author;	// Автор(ы)
	std::string name;	// Название
	std::string agency;	// Издательство
	int year;			// Год выпуска
	int total;			// Общее количество
	int available;		// Количество в наличии
	book(std::string code, std::string author, std::string name, std::string agency, int year, int total)
	{
		this->code = code;
		this->author = author;
		this->name = name;
		this->agency = agency;
		this->year = year;
		this->total = total;
		this->available = total;
	}
};

bool find_piece(const std::string& piece, const std::string& text) {
	int piece_size = piece.size();
	int text_size = text.size();

	// Создаем таблицу смещений
	std::vector<int> offsets(256, piece_size);
	for (int i = 0; i < piece_size - 1; i++) {
		offsets[abs(piece[i])] = piece_size - i - 1;
	}
	// Ищем подстроку
	int i = piece_size - 1;
	while (i < text_size) {
		int j = piece_size - 1;
		while (text[i] == piece[j]) {
			if (j == 0) {
				return true;
			}
			i--;
			j--;
		}
		i += offsets[abs(text[i])] > piece_size - j ? offsets[abs(text[i])] : piece_size - j;
	}
	return false;
}

class Tree {
public:
	static const int Hash(std::string key) {
		key.erase(3, 1);
		return atoi(key.c_str());
	}
private:
	struct node {
		book Book;		
		int key;
		node* left = nullptr, * right = nullptr;
		int height = 1;
	};
	node* Root = nullptr;
	int height(node* current)
	{
		return current ? current->height : 0;
	} 
	int bfactor(node* current)
	{
		return height(current->right) - height(current->left);
	} 
	void fixheight(node* current) 
	{	// фиксит разность высот после вращений
		uint8_t hLeft = height(current->left);
		uint8_t hRight = height(current->right);
		current->height = (hLeft > hRight ? hLeft : hRight) + 1;
	}
	node* rotateright(node* current) {
		node* temp = current->left;
		current->left = temp->right;
		temp->right = current;
		fixheight(current);
		fixheight(temp);
		return temp;
	}
	node* rotateleft(node* current) {
		node* temp = current->right;
		current->right = temp->left;
		temp->left = current;
		fixheight(current);
		fixheight(temp);
		return temp;
	}
	node* balance(node* current) 
	{	// балансировка узла current
		fixheight(current);
		if (bfactor(current) == 2)
		{
			if (bfactor(current->right) < 0)
				current->right = rotateright(current->right);
			return rotateleft(current);
		}
		if (bfactor(current) == -2)
		{
			if (bfactor(current->left) > 0)
				current->left = rotateleft(current->left);
			return rotateright(current);
		}
		return current; // балансировка не нужна
	}
	node* findmin(node* current) 
	{	// поиск узла с минимальным ключом в дереве current
		return current->left ? findmin(current->left) : current;
	}
	node* removemin(node* current)
	{	 // удаление узла с минимальным ключом из дерева current
		if (current->left == nullptr) return current->right;
		current->left = removemin(current->left);
		return balance(current);
	}
	node* remove(int key, node* current) 
	{	// удаление ключа key из дерева current
		if (!current) return nullptr;
		if (key < current->key) current->left = remove(key, current->left);
		else if (key > current->key) current->right = remove(key, current->right);
		else //  key == current->key 
		{
			node* left_copy = current->left;
			node* right_copy = current->right;
			delete current;
			if (right_copy == nullptr) {
				return left_copy;
			}
			node* min = findmin(right_copy);
			min->right = removemin(right_copy);
			min->left = left_copy;
			return balance(min);

		}
		return balance(current);
	}
	node* set(const book& Book, node* current) 
	{	// добавление
		if (!current) return new node{ Book, Hash(Book.code)};
		if (current->key == Hash(Book.code)) throw DuplicateBook(Book.code);
		if (Hash(Book.code) < current->key) { current->left = set(Book, current->left); }
		else { current->right = set(Book, current->right); }
		return balance(current);
	}
	book* find(int key, node* current) {
		if (!current) return nullptr;
		if (current->key == key) return &current->Book;
		if (key < current->key) return find(key, current->left);
		else return find(key, current->right);
	}
	void find_all(const std::string& piece, std::vector<book*>& all, node* current) {
		if (!current) return;		// симметричный обход 
		if (current->left) find_all(piece, all, current->left);
		if (find_piece(piece, current->Book.author) || find_piece(piece, current->Book.name))
			all.push_back(&current->Book);
		if (current->right) find_all(piece, all, current->right);
	}
	void get_all(std::vector<book*>& all, node* current) {
		if (!current) return;
		if (current->left) get_all(all, current->left);
		if (current) all.push_back(&current->Book);
		if (current->right) get_all(all, current->right);
	}
public:
	~Tree() {
		while (Root) {
			Root = remove(Root->key, Root);
		}
	}
public:
	void set(book Book) {
		Root = set(Book, Root);
	}
	void remove(const std::string& key) {
		Root = remove(Hash(key), Root);
	}
	book* find(const std::string& key) {
		return find(Hash(key), Root);
	}
	std::vector<book*> find_all(const std::string& piece) {
		std::vector<book*> all;
		find_all(piece, all, Root);
		return all;
	}
	std::vector<book*> get_all() {
		std::vector<book*> all;
		get_all(all, Root);
		return all;
	}
};