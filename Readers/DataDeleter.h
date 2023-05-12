#pragma once
#include "DataBase.h"

struct DataDeleter {
	static const bool DeleteReader(reader& Reader, DataBase& BD)
	{
		auto Archives = BD.Archives.find_all(Reader.ticket);
		for (auto& arch : Archives) {
			if (!BD.Archives.find(Reader.ticket, arch->code, GIVE::BACK)) 
				return false;
		}
		BD.Readers.remove(Reader.ticket);
		return true;
	}

	static const bool DeleteBook(book& Book, DataBase& BD)
	{
		BD.Books.remove(Book.code);
		return true;
	}
};