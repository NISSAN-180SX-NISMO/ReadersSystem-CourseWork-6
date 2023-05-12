#pragma once
#include "Set.h"
#include "Tree.h"
#include "List.h"

struct DataBase {
	Set Readers;
	Tree Books;
	List Archives;
};