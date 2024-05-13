#include <iostream>
#include "Date.h"

using namespace std;
Date::Date() {}
Date Date::today = Date();
void Date::settodaysdate(int d, int m, int y)
{
    today.date = d;
    today.month = m;
    today.year = y;
}

void Date::settodaysdate(const Date &d)
{
    today = d;
}

Date Date::gettodaysdate()
{
    return today;
}

Date::Date(int d, int m, int y) : date(d), month(m), year(y) {}

void Date::LoadDate(ifstream &myfile)
{
    myfile >> date;
    myfile >> month;
    myfile >> year;
    myfile.ignore();
}

bool Date::operator<=(const Date &D)
{
    if (year != D.year)
    {
        return year < D.year;
    }
    if (month != D.month)
    {
        return month < D.month;
    }
    return date <= D.date;
}

bool Date::operator<(const Date &D)
{
    if (year != D.year)
    {
        return year < D.year;
    }
    if (month != D.month)
    {
        return month < D.month;
    }
    return date < D.date;
}

bool Date::operator>=(const Date &D)
{
    if (year != D.year)
    {
        return year > D.year;
    }
    if (month != D.month)
    {
        return month > D.month;
    }
    return date >= D.date;
}

bool Date::operator>(const Date &D)
{
    if (year != D.year)
    {
        return year > D.year;
    }
    if (month != D.month)
    {
        return month > D.month;
    }
    return date > D.date;
}

bool Date::operator==(const Date &D)
{
    return year == D.year && month == D.month && date == D.date;
}

void Date::Print()
{
    cout << '(' << date << '/' << month << '/' << year << ')';
}

istream &operator>>(istream &in, Date &d)
{
    in >> d.date;
    in >> d.month;
    in >> d.year;
    in.ignore();

    return in;
}

int Date::getDate()
{
    return date;
}

int Date::getMonth()
{
    return month;
}

int Date::getYear()
{
    return year;
}