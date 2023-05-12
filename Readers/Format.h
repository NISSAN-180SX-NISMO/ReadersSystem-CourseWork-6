#pragma once
#include <regex>
#include <string>
#include <ATLComTime.h>

#define TICKET_FORMAT	Check::ticket
#define NAME_FORMAT		Check::name
#define NAMES_FORMAT	Check::names
#define YEAR_FORMAT		Check::year
#define CODE_FORMAT		Check::code
#define NUMBER_FORMAT	Check::number
#define DATE_FORMAT		Check::date

static std::vector<int> DateToVector(const std::string& date)
{
#define DAY 0
#define MONTH 1
#define YEAR 2
	// נאחהוכÿוע סענמךף םא לאססטג טח הה/לל/דדדד
	std::vector<int> DATE;
	std::string buff = "";
	for (auto i = date.begin(); i <= date.end(); ++i) {
		if (i == date.end()) { DATE.push_back(atoi(buff.c_str())); break; }
		else if (*i == '.') { DATE.push_back(atoi(buff.c_str())); buff = ""; }
		else { buff += *i; }
	}
	return DATE;
}

struct Check {
	static bool ticket(const std::string& value) {
		if (value.size() != 8) return false;
		if (regex_match(value.c_str(), std::regex("[׳ְֲ][0-9]{4}-[0-9]{2}"))) return true;
		else return false;
	}
	static bool name(const std::string& value) {
		if (regex_match(value.c_str(), std::regex("^[א-ÿְ-ß¸¨a-zA-Z]{1,}'?-?[א-ÿְ-ß¸¨a-zA-Z]{2,} ?[א-ÿְ-ß¸¨a-zA-Z]{2,} ?([א-ÿְ-ß¸¨a-zA-Z]{1,})?"))) return true;
		else return false;
	}
	static bool names(const std::string& value) {
		if (regex_match(value.c_str(), std::regex("([א-ÿְ-ß¸¨a-zA-Z]{1,}'?-?[א-ÿְ-ß¸¨a-zA-Z]{2,} ?[א-ÿְ-ß¸¨a-zA-Z]{2,} ?([א-ÿְ-ß¸¨a-zA-Z]{1,})?,? ?)+"))) return true;
		else return false;
	}
	static bool year(const std::string& value) {
		if (value.size() != 4) return false;
		if (regex_match(value.c_str(), std::regex("^(19[0-9]{2}|200[0-9]|201[0-9]|202[0-3])$"))) return true;
		else return false;
	}
	static bool code(const std::string& value) {
		if (value.size() != 7) return false;
		if (regex_match(value.c_str(), std::regex("[0-9]{3}(\\.)[0-9]{3}"))) return true;
		else return false;
	}
	static bool number(const std::string& value) {
		if (regex_match(value.c_str(), std::regex("[0-9]+"))) return true;
		else return false;
	}
	static bool date(const std::string& value) {
		if (regex_match(value.c_str(), std::regex("(0[1-9]|[12][0-9]|3[01])(\\.)(0[1-9]|1[0-2])(\\.)(19[0-9]{2}|200[0-9]|201[0-9]|202[0-3])"))) return true;
		else return false;
	}

	static std::vector<ATL::COleDateTime> dates;

	static bool date_real(const std::string& date) {
		ATL::COleDateTime atl_date;
		std::vector<int> arr_date = DateToVector(date);
		atl_date.SetDate(arr_date[YEAR], arr_date[MONTH], arr_date[DAY]);
		if (atl_date.m_status) return false;
		dates.push_back(atl_date);
		return true;
	}
};

std::vector<ATL::COleDateTime> Check::dates;