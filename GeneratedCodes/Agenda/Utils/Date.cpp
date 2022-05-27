#include "Date.h"

const int& Date::getDay() const {
	return day;
}

void Date::setDay(const int& day_) {
	day = day_;
}

const int& Date::getMonth() const {
	return month;
}

void Date::setMonth(const int& month_) {
	month = month_;
}

const int& Date::getYear() const {
	return year;
}

void Date::setYear(const int& year_) {
	year = year_;
}


Date::Date(const int& day_, const int& month_, const int& year_)
:day(day_)
,month(month_)
,year(year_)
{}

void Date::showInfo()
{

}


const bool Date::operator<(Date& other) const {
	return false;
}

const bool Date::operator>(Date& other) const {
	return false;
}

const bool Date::operator==(Date& other) const {
	return false;
}

const bool Date::operator!=(Date& other) const {
	return !( *this == other );
}

