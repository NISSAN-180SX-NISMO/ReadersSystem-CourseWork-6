#pragma once
#include <iostream>
#include "DataBase.h"

struct DataSetter {
	static const void SetReader(reader& Reader, DataBase& BD) 
	{
		BD.Readers.set(Reader);
	}
	static const void SetBook(book& Book, DataBase& BD)
	{
		BD.Books.set(Book);
	}
	static const bool SetGiveOutBook(book& Book, archive& Archive, DataBase& BD) {
		if (!Book.available) return false;
		Book.available--;
		BD.Archives.set(Archive);
		return true;
	}
	static const bool SetGiveBackBook(reader& Reader, book& Book, DataBase& BD) {
		if (!BD.Books.find(Book.code)) return false;
		if (Book.available == Book.total) return false;
		archive* GIVEOUT = BD.Archives.find(Reader.ticket, Book.code, GIVE::OUTT);
		Book.available++;
		BD.Archives.set(archive
			{ 
				GIVEOUT->ticket, 
				GIVEOUT->code, 
				GIVEOUT->give_out_date, 
				GIVEOUT->give_back_date, 
				GIVE::BACK 
			});
		return true;
	}
};