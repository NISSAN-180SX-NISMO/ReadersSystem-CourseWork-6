#pragma once
#include <string>
#include <fstream>
#include "DataBase.h"

struct DataOutput {
private:
	static std::string draw(int size, const char* symbol)
	{
		std::string line;
		for (uint16_t i = 0; i < size; ++i)
			line += symbol;
		return line;
	}
	static const void printReaderTitle(std::ostream& out)
	{
		out << " " << draw(217, "=") << std::endl;
		out << "[ Номер читательского билета "
			<< "][ Фамилия Имя Отчество " << draw(15, " ")
			<< "][ Год рождения "
			<< "][ Адрес проживания " << draw(49, " ")
			<< "][ Место рабты / учёбы " << draw(42, " ") << "]" << std::endl;
		out << " " << draw(217, "=") << std::endl;
	}
	static const void printBookTitle(std::ostream& out)
	{
		out << " " << draw(217, "=") << std::endl;
		out << "[ Шифр    "
			<< "][ Автор(ы) " << draw(57, " ")
			<< "][ Название " << draw(31, " ")
			<< "][ Издательство " << draw(51, " ")
			<< "][ Год выпуска "
			<< "][ Количество " << "]" << std::endl;
		out << " " << draw(217, "=") << std::endl;
	}
	static const void printArchiveTitle(std::ostream& out) 
	{
		out << " " << draw(82, "=") << std::endl;
		out << "[         "
			<< "][ Шифр    "
			<< "][ Номер читательского билета "
			<< "][ Дата выдачи " 
			<< "][ Дата возврата " << "]" << std::endl;
		out << " " << draw(82, "=") << std::endl;
	}
	static const void printReader(std::ostream& out, reader* Reader)
	{
		out << "[ " << Reader->ticket << draw(19, " ")
			<< "][ " << Reader->name << draw(std::abs(int(36 - Reader->name.size())), " ")
			<< "][ " << Reader->year << draw(9, " ")
			<< "][ " << Reader->address << draw(std::abs(int(66 - Reader->address.size())), " ")
			<< "][ " << Reader->work << draw(std::abs(int(62 - Reader->work.size())), " ") << "]" << std::endl;
		out << " " << draw(217, "=") << std::endl;
	}
	static const void printBook(std::ostream& out, book* Book)
	{
		out << "[ " << Book->code << " "
			<< "][ " << Book->author << draw(std::abs(int(66 - Book->author.size())), " ")
			<< "][ " << Book->name << draw(std::abs(int(40 - Book->name.size())), " ")
			<< "][ " << Book->agency << draw(std::abs(int(64 - Book->agency.size())), " ")
			<< "][ " << Book->year << draw(8, " ")
			<< "][ " << Book->available << "/" << Book->total 
			<< (Book->available < 10?"  ": Book->available < 100 ? " ":"") 
			<< (Book->total < 10 ? "  " : Book->total < 100 ? " " : "") << "    ]" << std::endl;
		out << " " << draw(217, "=") << std::endl;
	}
	static const void printArchive(std::ostream& out, archive* Archive)
	{
		out << "[ " << (Archive->event == GIVE::OUTT ? "Выдача  ": "Возврат ")
		    << "][ " << Archive->code << " "
			<< "][ " << Archive->ticket << draw(19, " ")
			<< "][ " << Archive->give_out_date << "  "
			<< "][ " << Archive->give_back_date << "    " << "]" << std::endl;
		out << " " << draw(82, "=") << std::endl;
	}
public:
	static const void print(std::ostream& out, std::vector<reader*> all)
	{
		printReaderTitle(out);
		for (auto& element : all)
			printReader(out, element);
	}
	static const void print(std::ostream& out, std::vector<book*> all)
	{
		printBookTitle(out);
		for (auto& element : all)
			printBook(out, element);
	}
	static const void print(std::ostream& out, std::vector<archive*> all)
	{
		printArchiveTitle(out);
		for (auto& element : all)
			printArchive(out, element);
	}
};