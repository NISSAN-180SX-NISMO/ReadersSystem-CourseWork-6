#include "pch.h"
#include "../Readers/Set.h"
#include "../Readers/Tree.h"
#include "../Readers/List.h"
#include "../Readers/DataBase.h"
#include "../Readers/DataSetter.h"
#include "../Readers/DataDeleter.h"


#include "../Readers/Format.h"

TEST(Set, HashTest) {
	// Arrange
	std::string key1 = "А1234-00";
	std::string key2 = "А1234-00";
	std::string key3 = "А4321-00";
	// Arrange // коллизии
	std::string coll1 = "В4241-08";
	std::string coll2 = "А8606-28"; 
	std::string coll3 = "А2981-79"; 
	// Assert
	ASSERT_EQ(Set::Hash(key1), Set::Hash(key2));
	ASSERT_NE(Set::Hash(key1), Set::Hash(key3));
	// Assert // коллизии
	ASSERT_EQ(Set::Hash(coll1), Set::Hash(coll2));
	ASSERT_EQ(Set::Hash(coll2), Set::Hash(coll3));
}

TEST(Set, SetAndRemoveTest) {
	// Arrange
	Set a;
	// Act // коллизии
	a.set(reader{ "В4241-08", "", NULL, "", "" });
	a.set(reader{ "А8606-28", "", NULL, "", "" });
	a.set(reader{ "А2981-79", "", NULL, "", "" });
	// Assert
	ASSERT_EQ(a.get_all().size(), 3);
	// Act
	a.remove("А8606-28");
	a.remove("В4241-08");
	a.remove("А2981-79");
	// Assert
	ASSERT_EQ(a.get_all().size(), 0);
}

TEST(Set, DuplicateTest) {
	// Arrange
	Set a;
	// Act // коллизии
	a.set(reader{ "В4241-08", "", NULL, "", "" });
	a.set(reader{ "А8606-28", "", NULL, "", "" });
	// Assert
	ASSERT_THROW(a.set(reader{ "А8606-28", "", NULL, "", "" }), DuplicateReader);
}

TEST(Set, FindTest) {
	// Arrange 
	Set a;
	// Act 
	a.set(reader{ "А1111-11", "Золотов Иван Петрухин", NULL, "", "" });
	// Act // коллизии
	a.set(reader{ "А8606-28", "Петров Иван Золотухин", NULL, "", "" });
	a.set(reader{ "А2981-79", "Золотов Иван Петрухин", NULL, "", "" });
	// Assert
	ASSERT_EQ(a.find("А2981-79")->name, "Золотов Иван Петрухин");
	ASSERT_EQ(a.find_all("Золотов Иван Петрухин").size(), 2);
}

TEST(Tree, HashTest) {
	// Arrange
	std::string key = "123.456";
	// Assert
	ASSERT_EQ(Tree::Hash(key), 123456);
}

TEST(Tree, SetAndRemoveTest) {
	// Arrange
	Tree a;
	// Act
	a.set(book{ "123.456", "", "", "", NULL, NULL});
	a.set(book{ "234.567", "", "", "", NULL, NULL });
	a.set(book{ "345.678", "", "", "", NULL, NULL});
	// Assert
	ASSERT_EQ(a.get_all().size(), 3);
	// Act
	a.remove("234.567");
	a.remove("123.456");
	a.remove("345.678");
	// Assert
	ASSERT_EQ(a.get_all().size(), 0);
}

TEST(Tree, DuplicateTest) {
	// Arrange
	Tree a;
	// Act
	a.set(book{ "123.456", "", "", "", NULL, NULL });
	// Assert
	ASSERT_THROW(a.set(book{ "123.456", "", "", "", NULL, NULL }), DuplicateBook);
}

TEST(Tree, FindTest) {
	// Arrange 
	Tree a;
	// Act
	a.set(book{ "123.456", "Пушкин", "Сказка о царе Салтане", "", NULL, NULL });
	a.set(book{ "234.567", "Достоевский", "А вы могли бы?", "", NULL, NULL });
	a.set(book{ "345.678", "Александр Дюма", "Три мушкетёра", "", NULL, NULL });
	// Assert
	ASSERT_EQ(a.find("234.567")->name, "А вы могли бы?");
	ASSERT_EQ(a.find_all("ушк").size(), 2);
}

TEST(List, SetAndRemoveTest) {
	// Arrange
	List a;
	// Act
	a.set(archive{ "", "123.456", "", "", GIVE::OUTT });
	a.set(archive{ "", "234.567", "", "", GIVE::OUTT });
	a.set(archive{ "", "345.678", "", "", GIVE::OUTT });
	// Assert
	ASSERT_EQ(a.get_all().size(), 3);
	// Act
	a.remove();
	a.remove();
	a.remove();
	// Assert
	ASSERT_EQ(a.get_all().size(), 0);
}

TEST(List, FindTest) {
	// Arrange 
	List a;
	// Act
	a.set(archive{ "А8606-28", "123.456", "", "", GIVE::OUTT });
	a.set(archive{ "А8606-28", "123.456", "", "", GIVE::BACK });
	a.set(archive{ "А8606-28", "345.678", "", "", GIVE::OUTT });
	// Assert
	ASSERT_EQ(a.find_all("123.456", GIVE::OUTT)[0]->event, GIVE::OUTT);
	ASSERT_EQ(a.find_all("А8606-28").size(), 3);
}

TEST(List, SortTest) {
	// Arrange 
	List a;
	std::vector<archive*> ref;
	a.set(archive{ "А8606-28", "888.888", "", "", GIVE::OUTT });
	ref.push_back(new archive{ "А8606-28", "888.888", "", "", GIVE::OUTT });
	a.set(archive{ "А8606-28", "777.777", "", "", GIVE::OUTT });
	ref.push_back(new archive{ "А8606-28", "777.777", "", "", GIVE::OUTT });
	a.set(archive{ "А8606-28", "222.222", "", "", GIVE::OUTT });
	ref.push_back(new archive{ "А8606-28", "222.222", "", "", GIVE::OUTT });
	a.set(archive{ "А8606-28", "333.333", "", "", GIVE::OUTT });
	ref.push_back(new archive{ "А8606-28", "333.333", "", "", GIVE::OUTT });
	a.set(archive{ "А8606-28", "555.555", "", "", GIVE::OUTT });
	ref.push_back(new archive{ "А8606-28", "555.555", "", "", GIVE::OUTT });
	// Act
	std::sort(ref.begin(), ref.end(), [](archive* a, archive* b)->bool {return a->code < b->code; });
	a.sort();
	auto all = a.get_all();
	// Assert
	for (int i = 0; i < all.size(); ++i) {
		ASSERT_EQ(all[i]->code, ref[i]->code);
	}
	a.set(archive{ "А8606-28", "999.999", "", "", GIVE::OUTT });
	ref.push_back(new archive{ "А8606-28", "999.999", "", "", GIVE::OUTT });
	a.set(archive{ "А8606-28", "111.111", "", "", GIVE::OUTT });
	ref.push_back(new archive{ "А8606-28", "111.111", "", "", GIVE::OUTT });
	a.set(archive{ "А8606-28", "666.666", "", "", GIVE::OUTT });
	ref.push_back(new archive{ "А8606-28", "666.666", "", "", GIVE::OUTT });
	a.set(archive{ "А8606-28", "444.444", "", "", GIVE::OUTT });
	ref.push_back(new archive{ "А8606-28", "444.444", "", "", GIVE::OUTT });
	// Act
	std::sort(ref.begin(), ref.end(), [](archive* a, archive* b)->bool {return a->code < b->code; });
	a.sort();
	all = a.get_all();
	// Assert
	for (int i = 0; i < all.size(); ++i) {
		ASSERT_EQ(all[i]->code, ref[i]->code);
	}
}

DataBase BD;

TEST(SYSTEM, GiveOutTest) {
	// Arrange
	DataSetter::SetReader(reader{ "А0369-01", "test", NULL, "test", "test" }, BD);
	DataSetter::SetReader(reader{ "Ч1470-02", "test", NULL, "test", "test" }, BD);
	DataSetter::SetReader(reader{ "В2581-03", "test", NULL, "test", "test" }, BD);
	DataSetter::SetBook(book{ "036.001","test","test" ,"test", NULL, 2 }, BD);
	DataSetter::SetBook(book{ "147.002","test","test" ,"test", NULL, 2 }, BD);
	DataSetter::SetBook(book{ "258.003","test","test" ,"test", NULL, 2 }, BD);
	// Assert
	ASSERT_TRUE(DataSetter::SetGiveOutBook(*BD.Books.find("036.001"), archive{"А0369-01","036.001", "01/01/2001", "10/10/2010", GIVE::OUTT}, BD));
	ASSERT_TRUE(DataSetter::SetGiveOutBook(*BD.Books.find("036.001"), archive{"Ч1470-02","036.001", "01/01/2001", "10/10/2010", GIVE::OUTT}, BD));
	// Должны закончиться экземпляры
	ASSERT_FALSE(DataSetter::SetGiveOutBook(*BD.Books.find("036.001"), archive{"В2581-03","036.001", "01/01/2001", "10/10/2010", GIVE::OUTT}, BD));
	// Один читатель может взять несколько книг
	ASSERT_TRUE(DataSetter::SetGiveOutBook(*BD.Books.find("147.002"), archive{ "А0369-01","147.002", "01/01/2001", "10/10/2010", GIVE::OUTT }, BD));
	ASSERT_TRUE(DataSetter::SetGiveOutBook(*BD.Books.find("147.002"), archive{ "Ч1470-02","147.002", "01/01/2001", "10/10/2010", GIVE::OUTT }, BD));
	ASSERT_TRUE(DataSetter::SetGiveOutBook(*BD.Books.find("258.003"), archive{ "Ч1470-02","258.003", "01/01/2001", "10/10/2010", GIVE::OUTT }, BD));
	// Проверка количества книг
	ASSERT_EQ(BD.Books.find("036.001")->available, 0);
	ASSERT_EQ(BD.Books.find("147.002")->available, 0);
	ASSERT_EQ(BD.Books.find("258.003")->available, 1);
}

TEST(SYSTEM, DeleteTest) {
	// Assert
	// Не получится удалить должников
	ASSERT_FALSE(DataDeleter::DeleteReader(*BD.Readers.find("А0369-01"),BD));
	ASSERT_FALSE(DataDeleter::DeleteReader(*BD.Readers.find("Ч1470-02"),BD));
	// А этого получится, так как книги на него не хватило
	ASSERT_TRUE(DataDeleter::DeleteReader(*BD.Readers.find("В2581-03"),BD));
	// Удалить книгу получится, но вернуть её потом будет нельзя
	ASSERT_TRUE(DataDeleter::DeleteBook(*BD.Books.find("258.003"), BD));
}

TEST(SYSTEM, GiveBackTest) {
	// Удаленную книгу не получится вернуть (для эмуляции создал новую книгу с таким шифром чтобы запустить функцию)
	ASSERT_FALSE(DataSetter::SetGiveBackBook(*BD.Readers.find("Ч1470-02"), book{ "258.003","test","test" ,"test", NULL, 2 }, BD));
	// Другие книги можно вернуть
	ASSERT_TRUE(DataSetter::SetGiveBackBook(*BD.Readers.find("А0369-01"), *BD.Books.find("036.001"), BD));
	ASSERT_TRUE(DataSetter::SetGiveBackBook(*BD.Readers.find("А0369-01"), *BD.Books.find("147.002"), BD));

	ASSERT_TRUE(DataSetter::SetGiveBackBook(*BD.Readers.find("Ч1470-02"), *BD.Books.find("036.001"), BD));
	ASSERT_TRUE(DataSetter::SetGiveBackBook(*BD.Readers.find("Ч1470-02"), *BD.Books.find("147.002"), BD));
	// Вернуть книгу, все экземпляры которой в наличии, не получится
	ASSERT_FALSE(DataSetter::SetGiveBackBook(*BD.Readers.find("Ч1470-02"), *BD.Books.find("147.002"), BD));
}
