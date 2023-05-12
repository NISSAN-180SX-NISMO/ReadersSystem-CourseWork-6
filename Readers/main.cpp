
#include <Windows.h>
#include "Menu.h"
#define PRINT(a) std::cout << a << std::endl


int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(LC_ALL, "");
	Menu::AheadInitialization();
	while (true) {
		system("cls");
		PRINT("[1]= Показать");
		PRINT("[2]= Добавить");
		PRINT("[3]= Удалить");
		PRINT("[4]= Найти");
		PRINT("[0]= Выход");
		switch (ConsoleInput::RequestMenuItem()) {
		case 1: 
			PRINT("[1]=[1]= Вывести список читателей");
			PRINT("[1]=[2]= Вывести список книг");
			PRINT("[1]=[3]= Вывести список записей");
			PRINT("[1]=[0]= Назад");
			switch (ConsoleInput::RequestMenuItem()) {
			case 1: Menu::PrintReaders(); break;
			case 2: Menu::PrintBooks(); break;
			case 3: Menu::PrintArchives(); break;
			case 0: break;
			default: PRINT("Таккого пункта меню нет!"); PAUSE
			} break;
		case 2: 
			PRINT("[2]=[1]= Добавить читателя");
			PRINT("[2]=[2]= Добавить книгу");
			PRINT("[2]=[3]= Выдать книгу");
			PRINT("[2]=[4]= Вернуть книгу");
			PRINT("[2]=[0]= Назад");
			switch (ConsoleInput::RequestMenuItem()) {
			case 1: Menu::AddReader(); break;
			case 2: Menu::AddBook(); break;
			case 3: Menu::GiveOutBook(); break;
			case 4: Menu::GiveBackBook(); break;
			case 0: break;
			default: PRINT("Таккого пункта меню нет!"); PAUSE
			} break;
		case 3: 
			PRINT("[3]=[1]= Удалить читателя");
			PRINT("[3]=[2]= Удалить всех читателей");
			PRINT("[3]=[3]= Удалить все экземпляры книги");
			PRINT("[3]=[4]= Удалить все книги");
			PRINT("[3]=[0]= Назад");
			switch (ConsoleInput::RequestMenuItem()) {
			case 1: Menu::DeleteReader(); break;
			case 2: Menu::ClearReaders(); break;
			case 3: Menu::DeleteBook(); break;
			case 4: Menu::ClearBooks(); break;
			case 0: break;
			default: PRINT("Таккого пункта меню нет!"); PAUSE
			} break;
		case 4: 
			PRINT("[4]=[1]= Найти читателя по номеру билета");
			PRINT("[4]=[2]= Найти читателей по ФИО");
			PRINT("[4]=[3]= найти книгу по шифру");
			PRINT("[4]=[4]= Найти книги по фрагментам ФИО автора(ов) или названия");
			PRINT("[4]=[0]= Назад");
			switch (ConsoleInput::RequestMenuItem()) {
			case 1: Menu::FindReader(); break;
			case 2: Menu::FindReaders(); break;
			case 3: Menu::FindBook(); break;
			case 4: Menu::FindBooks(); break;
			case 0: break;
			default: PRINT("Таккого пункта меню нет!"); PAUSE
			} break;
		case 0: return 0;
		default: PRINT("Таккого пункта меню нет!"); PAUSE
		}
	}
}


