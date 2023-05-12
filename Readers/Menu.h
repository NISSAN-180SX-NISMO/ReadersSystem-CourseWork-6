#pragma once
#include <fstream>
#include "DataBase.h"
#include "ConsoleInput.h"
#include "DataOutput.h"
#include "DataSetter.h"
#include "DataDeleter.h"

#define PAUSE system("pause");
#define START(file) system(("start " + file).c_str())
#define MENU_ITEM static const void
#define PRINT(a) std::cout << a << std::endl

class Menu {
private:
	static DataBase MainBase;
	static const std::string ReadersFile;
	static const std::string BooksFile;
	static const std::string ArchiveFile;
	static std::vector<archive*> GetUniqArchives(std::vector<archive*>& all)
	{
		std::vector<archive*> GIVEOUTS;
		for (auto& Archive : all)
		{
			if (Archive->event == GIVE::OUTT) {
				if (std::find_if(all.begin(), all.end(), [Archive](archive* a) {
					return (a->code == Archive->code && a->ticket == Archive->ticket && a->event == GIVE::BACK); }) == all.end())
				{
					GIVEOUTS.push_back(Archive);
				}
			}
		}
		return GIVEOUTS;
	}
public:
	static const void AheadInitialization()
	{
		DataSetter::SetReader(*new reader{ "Ч1111-01", "Зорин Дмитрий Александрович", 2001, "город Лотошино, пр. Гоголя, 33", "МБОУ СОШ №11" }, MainBase);
		DataSetter::SetReader(*new reader{ "В2222-02", "Сергеев Андрей Даниилович", 2002, "город Домодедово, ул. Ладыгина, 71", "МБОУ СОШ №22" }, MainBase);
		DataSetter::SetReader(*new reader{ "А3333-03", "Блинов Сергей Дмитриевич", 2003, "город Шатура, пр. Чехова, 83", "МБОУ СОШ №33" }, MainBase);
		DataSetter::SetReader(*new reader{ "В4444-04", "Смирнов Кирилл Данилович", 2004, "город Серпухов, спуск Домодедовский, 33", "МБОУ СОШ №44" }, MainBase);
		DataSetter::SetReader(*new reader{ "Ч5555-05", "Смирнов Лев Степанович", 2005, "город Красногорск, пл. Бухарестская, 56", "МБОУ СОШ №55" }, MainBase);
		DataSetter::SetBook(*new book{ "111.111", "Александр Пушкин","Евгений Онегин","Шиповник",1830, 1 }, MainBase);
		DataSetter::SetBook(*new book{ "222.222", "Михаил Лермонтов","Герой нашего времени","Скорпион",1839, 2 }, MainBase);
		DataSetter::SetBook(*new book{ "333.333", "Николай Гоголь","Мёртвые души","Ришар",1835, 3 }, MainBase);
		DataSetter::SetBook(*new book{ "444.444", "Иван Гончаров","Обломов","Новое время",1859, 4 }, MainBase);
		DataSetter::SetBook(*new book{ "555.555", "Иван Тургенев","Отцы и дети","Вольф",1861, 5 }, MainBase);
	}
	MENU_ITEM PrintReaders() {
		std::ofstream fout(ReadersFile);
		DataOutput::print(fout, MainBase.Readers.get_all());
		fout.close();
		START(ReadersFile);
	}
	MENU_ITEM PrintBooks() {
		std::ofstream fout(BooksFile);
		DataOutput::print(fout, MainBase.Books.get_all());
		fout.close();
		START(BooksFile);
	}
	MENU_ITEM PrintArchives() {
		std::ofstream fout(ArchiveFile);
		MainBase.Archives.sort();
		DataOutput::print(fout, MainBase.Archives.get_all());
		fout.close();
		START(ArchiveFile);
	}

	MENU_ITEM AddReader() {
		reader* Reader = ConsoleInput::RequestReader();
		try
		{
			DataSetter::SetReader(*Reader, MainBase);
			PRINT("Читатель успешно добавлен!");
		}
		catch (DuplicateReader& ex)
		{
			PRINT(ex.get_more());
		}
		PAUSE
	}
	MENU_ITEM AddBook() {
		book* Book = ConsoleInput::RequestBook();
		try
		{
			DataSetter::SetBook(*Book, MainBase);
			PRINT("Книга успешно добавлена!");
		}
		catch (DuplicateBook& ex)
		{
			PRINT(ex.get_more());
		}
		PAUSE
	}
	MENU_ITEM GiveOutBook() {
		book* Book = ConsoleInput::RequestBookFromBD(MainBase);
		if (!Book) return;
		reader* Reader = ConsoleInput::RequestReaderFromBD(MainBase);
		if (!Reader) return;
		archive* Archive = ConsoleInput::RequestArchive(Book->code, Reader->ticket);
		if (DataSetter::SetGiveOutBook(*Book, *Archive, MainBase)) PRINT("Книга успешно выдана!");
		else PRINT("Данной книги нет в наличии!");
		PAUSE
	}
	MENU_ITEM GiveBackBook() {
		reader* Reader = ConsoleInput::RequestReaderFromBD(MainBase);
		if (!Reader) return;
		book* Book = ConsoleInput::RequestBookFromBD(MainBase);
		if (!Book) return;
		if (DataSetter::SetGiveBackBook(*Reader, *Book, MainBase)) PRINT("Книга успешно возвращена!");
		else PRINT("Все экземпляры книги в наличии!");
		PAUSE
	}

	MENU_ITEM DeleteReader()
	{
		reader* Reader = ConsoleInput::RequestReaderFromBD(MainBase);
		if (!Reader) return;
		if (DataDeleter::DeleteReader(*Reader, MainBase)) PRINT("Читатель удалён!");
		else PRINT("Читатель не вернул книгу и не может быть удалён!");
		PAUSE
	}
	MENU_ITEM ClearReaders()
	{
		auto all = MainBase.Readers.get_all();
		for (auto& Reader : all) {
			if (!DataDeleter::DeleteReader(*Reader, MainBase))
				PRINT("Должник не будет удалён!");
		}
		PRINT("Все читатели удалены! ");
		PAUSE
	}
	MENU_ITEM DeleteBook() 
	{
		book* Book = ConsoleInput::RequestBookFromBD(MainBase);
		if (!Book) return;
		DataDeleter::DeleteBook(*Book, MainBase);
		PRINT("Все оставшиеся экземпляры книги удалёны!\nВыданные ранее экземпляры возврату не подлежат!"); 
		PAUSE
	}
	MENU_ITEM ClearBooks()
	{
		auto all = MainBase.Books.get_all();
		for (auto& Book : all)
			DataDeleter::DeleteBook(*Book, MainBase);
		PRINT("Все читатели удалены!\nВыданные ранее экземпляры возврату не подлежат!");
		PAUSE
	}

	MENU_ITEM FindReader()
	{
		reader* Reader = ConsoleInput::RequestReaderFromBD(MainBase);
		if (!Reader) return;
		DataOutput::print(std::cout, std::vector<reader*>{Reader});
		auto all = MainBase.Archives.find_all(Reader->ticket);
		DataOutput::print(std::cout, GetUniqArchives(all));
		PAUSE
	}
	MENU_ITEM FindReaders()
	{
		std::string name = ConsoleInput::input("Введите ФИО читателя: ", NAME_FORMAT);
		auto Readers = MainBase.Readers.find_all(name);
		DataOutput::print(std::cout, Readers);
		PAUSE
	}
	MENU_ITEM FindBook()
	{
		book* Book = ConsoleInput::RequestBookFromBD(MainBase);
		if (!Book) return;
		DataOutput::print(std::cout, std::vector<book*>{Book});
		auto all = MainBase.Archives.find_all(Book->code, GIVE::OUTT);
		DataOutput::print(std::cout, GetUniqArchives(all));
		PAUSE
	}
	MENU_ITEM FindBooks()
	{
		std::string piece = ConsoleInput::input("Введите фрагмент ФИО автора(ов) или названия: ");
		auto Books = MainBase.Books.find_all(piece);
		DataOutput::print(std::cout, Books);
		PAUSE
	}
};

DataBase Menu::MainBase;
const std::string Menu::ReadersFile = "Readers.txt";
const std::string Menu::BooksFile = "Books.txt";
const std::string Menu::ArchiveFile = "Archive.txt";