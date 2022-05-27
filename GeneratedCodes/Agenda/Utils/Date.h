#ifndef __DATE_H__
#define __DATE_H__
class Date
{
	int day;
	int month;
	int year;

public: 
	const int& getDay() const;
	void setDay(const int& day_);

	const int& getMonth() const;
	void setMonth(const int& month_);

	const int& getYear() const;
	void setYear(const int& year_);


	Date(const int& day_, const int& month_, const int& year_);
	virtual ~Date() {}

	virtual void showInfo();

	const bool operator<(Date& other) const;
	const bool operator>(Date& other) const;
	const bool operator==(Date& other) const;
	const bool operator!=(Date& other) const;
};

#endif

