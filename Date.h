#ifndef DATE_H
#define DATE_H

#include <iostream>
#include <fstream>

using namespace std;

class Date
{
private:
    int date;
    int month;
    int year;
    static Date today;

public:
    Date();
    Date(int, int, int);

    static void settodaysdate(int d, int m, int y);
    static void settodaysdate(const Date &d);
    static Date gettodaysdate();

    void LoadDate(ifstream &in);
    void Print();

    bool operator<(const Date &rhs);
    bool operator>(const Date &rhs);
    bool operator>=(const Date &rhs);
    bool operator<=(const Date &rhs);
    bool operator==(const Date &rhs);

    int getDate();
    int getMonth();
    int getYear();

    friend istream &operator>>(istream &, Date &);
};

#endif