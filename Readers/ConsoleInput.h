#pragma once
#include <iostream>
#include <functional>
#include <vector>
#include "DataBase.h"
#include "Format.h"
#define PRINT(a) std::cout << a << std::endl

struct ConsoleInput {
	static std::string input(const std::string& title, std::function<bool(std::string)> Format = [](std::string) { return true; }) {
		while (true) {
			std::string input;
			std::cout << title; std::getline(std::cin, input);
			FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
			if (Format(input)) return input;
			else PRINT("Неверный формат записи!!!");
		}
	}
	static int RequestMenuItem() {
		return atoi(input("Выберите пункт меню: ", NUMBER_FORMAT).c_str());
	}
	static reader* RequestReader(const std::string& ticket = "")
	{
		return new reader
		{
			ticket == "" ? input("Введите номер читательского билета: ", TICKET_FORMAT) : ticket,
			input("Введите полное ФИО читателя: ", NAME_FORMAT),
			atoi(input("Введите год рождения: ", YEAR_FORMAT).c_str()),
			input("Введите адрес проживания: "),
			input("Введите место работы/учёбы: ")
		};
	}
	static reader* RequestReaderFromBD(DataBase& BD) 
	{
		while (true) {
			std::string ticket = input("Введите номер читательского билета: ", TICKET_FORMAT);
			reader* Reader = BD.Readers.find(ticket);
			if (!Reader) { choose:
				std::cout << "Данный читатель не зарегистрирвоан!" << std::endl;
				std::cout << "[1]= Повторить ввод" << std::endl;
				std::cout << "[2]= Зрегистрировать сейчас" << std::endl;
				std::cout << "[3]= Отменить операцию" << std::endl;
				switch (ConsoleInput::RequestMenuItem()) {
				case 1: continue;
				case 2: return RequestReader(ticket);
				case 3: return nullptr;
				default:
					PRINT("Нет такого пункта меню!");
					goto choose;
				}
			}
			return Reader;
		}
	}
	static book* RequestBook() {
		return new book{
			input("Введите шифр книги: ", CODE_FORMAT),
			input("Укажите автора(ов) через запятую: ", NAMES_FORMAT),
			input("Введите название произведения: "),
			input("Укажите издательское агенство: "),
			atoi(input("Введите год выпуска: ", YEAR_FORMAT).c_str()),
			atoi(input("Укажите количество экземпляров: ", NUMBER_FORMAT).c_str())
		};
	}
	static book* RequestBookFromBD(DataBase& BD) 
	{
		while (true) {
			book* Book = BD.Books.find(input("Введите шифр книги: ", CODE_FORMAT));
			if (!Book) { choose:
				std::cout << "Соответствующая книга не найдена!" << std::endl;
				std::cout << "[1]= Повторить ввод" << std::endl;
				std::cout << "[2]= Отменить операцию" << std::endl;
				switch (ConsoleInput::RequestMenuItem()) {
				case 1: continue;
				case 2: return nullptr;
				default:
					PRINT("Нет такого пункта меню!");
					goto choose;
				}
			}
			return Book;
		}
	}
	static archive* RequestArchive(const std::string& code, const std::string& ticket) {
		while (true) {
			Check::dates.clear();
			std::string OUTT = input("Введите дату выдачи книги: ", DATE_FORMAT);
			if (!Check::date_real(OUTT)) {
				PRINT("Такой даты не существует! Повторите ввод!");
				continue;
			}
			std::string BACK = input("Введите дату возврата книги: ", DATE_FORMAT);
			if (!Check::date_real(BACK)) {
				PRINT("Такой даты не существует! Повторите ввод!");
				continue;
			}
			if (Check::dates[int(GIVE::OUTT)] > Check::dates[int(GIVE::BACK)]) {
				PRINT("Дата выдачи не может быть больше даты возврата! Повторите ввод!");
				continue;
			}
			else return new archive{ ticket, code, OUTT, BACK, GIVE::OUTT };
		}
	}
};