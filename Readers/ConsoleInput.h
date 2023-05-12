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
			else PRINT("�������� ������ ������!!!");
		}
	}
	static int RequestMenuItem() {
		return atoi(input("�������� ����� ����: ", NUMBER_FORMAT).c_str());
	}
	static reader* RequestReader(const std::string& ticket = "")
	{
		return new reader
		{
			ticket == "" ? input("������� ����� ������������� ������: ", TICKET_FORMAT) : ticket,
			input("������� ������ ��� ��������: ", NAME_FORMAT),
			atoi(input("������� ��� ��������: ", YEAR_FORMAT).c_str()),
			input("������� ����� ����������: "),
			input("������� ����� ������/�����: ")
		};
	}
	static reader* RequestReaderFromBD(DataBase& BD) 
	{
		while (true) {
			std::string ticket = input("������� ����� ������������� ������: ", TICKET_FORMAT);
			reader* Reader = BD.Readers.find(ticket);
			if (!Reader) { choose:
				std::cout << "������ �������� �� ���������������!" << std::endl;
				std::cout << "[1]= ��������� ����" << std::endl;
				std::cout << "[2]= ��������������� ������" << std::endl;
				std::cout << "[3]= �������� ��������" << std::endl;
				switch (ConsoleInput::RequestMenuItem()) {
				case 1: continue;
				case 2: return RequestReader(ticket);
				case 3: return nullptr;
				default:
					PRINT("��� ������ ������ ����!");
					goto choose;
				}
			}
			return Reader;
		}
	}
	static book* RequestBook() {
		return new book{
			input("������� ���� �����: ", CODE_FORMAT),
			input("������� ������(��) ����� �������: ", NAMES_FORMAT),
			input("������� �������� ������������: "),
			input("������� ������������ ��������: "),
			atoi(input("������� ��� �������: ", YEAR_FORMAT).c_str()),
			atoi(input("������� ���������� �����������: ", NUMBER_FORMAT).c_str())
		};
	}
	static book* RequestBookFromBD(DataBase& BD) 
	{
		while (true) {
			book* Book = BD.Books.find(input("������� ���� �����: ", CODE_FORMAT));
			if (!Book) { choose:
				std::cout << "��������������� ����� �� �������!" << std::endl;
				std::cout << "[1]= ��������� ����" << std::endl;
				std::cout << "[2]= �������� ��������" << std::endl;
				switch (ConsoleInput::RequestMenuItem()) {
				case 1: continue;
				case 2: return nullptr;
				default:
					PRINT("��� ������ ������ ����!");
					goto choose;
				}
			}
			return Book;
		}
	}
	static archive* RequestArchive(const std::string& code, const std::string& ticket) {
		while (true) {
			Check::dates.clear();
			std::string OUTT = input("������� ���� ������ �����: ", DATE_FORMAT);
			if (!Check::date_real(OUTT)) {
				PRINT("����� ���� �� ����������! ��������� ����!");
				continue;
			}
			std::string BACK = input("������� ���� �������� �����: ", DATE_FORMAT);
			if (!Check::date_real(BACK)) {
				PRINT("����� ���� �� ����������! ��������� ����!");
				continue;
			}
			if (Check::dates[int(GIVE::OUTT)] > Check::dates[int(GIVE::BACK)]) {
				PRINT("���� ������ �� ����� ���� ������ ���� ��������! ��������� ����!");
				continue;
			}
			else return new archive{ ticket, code, OUTT, BACK, GIVE::OUTT };
		}
	}
};