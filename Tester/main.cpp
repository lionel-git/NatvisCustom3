#include <iostream>
#include <format>
#include <vector>

// Cf: https://stackoverflow.com/questions/56717088/algorithm-for-converting-serial-date-excel-to-year-month-day-in-c
void ExcelSerialDateToDMY(int nSerialDate, int& nDay, int& nMonth, int& nYear)
{
    // Modified Julian to DMY calculation with an addition of 2415019
    int l = nSerialDate + 68569 + 2415019;
    int n = int((4 * l) / 146097);
    l = l - int((146097 * n + 3) / 4);
    int i = int((4000 * (l + 1)) / 1461001);
    l = l - int((1461 * i) / 4) + 31;
    int j = int((80 * l) / 2447);
    nDay = l - int((2447 * j) / 80);
    l = int(j / 11);
    nMonth = j + 2 - (12 * l);
    nYear = 100 * (n - 49) + i + l;
}

int DMYToExcelSerialDate(int nDay, int nMonth, int nYear)
{
    // DMY to Modified Julian calculated with an extra subtraction of 2415019.
    return int((1461 * (nYear + 4800 + int((nMonth - 14) / 12))) / 4) +
        int((367 * (nMonth - 2 - 12 * ((nMonth - 14) / 12))) / 12) -
        int((3 * (int((nYear + 4900 + int((nMonth - 14) / 12)) / 100))) / 4) +
        nDay - 2415019 - 32075;
}

namespace E
{
    class ExcelDate
    {
    public:
        ExcelDate(int n) { value_ = n; }

    public:

        int value_;
    };
}

class Conversion
{
public:
   

    static std::string to_string(int value)
    {
        int day, month, year;
        ExcelSerialDateToDMY(value, day, month, year);
        return std::format("{}/{}/{}", year, month, day);
    }

};

class ExcelDate2
{
public:
    ExcelDate2(int n) { value_ = n; }

    std::string to_string() { return Conversion::to_string(value_); }

public:

    int value_;
};


class MyClass
{
public:
    virtual int getValue() { return value1_; }

    virtual int getValue2() { return value2_; }

private:
    int value1_{-171};
    int value2_{ -1237 };
};

void test()
{
    for (int n = 25741; n < 62000; ++n)
    {
        int y, m, d;
        ExcelSerialDateToDMY(n, d, m, y);
        int n2 = DMYToExcelSerialDate(d, m, y);
        if (n != n2)
            std::cerr << "Error for : " << n << std::endl;
        //std::cout << y << "/" << m << "/" << d << std::endl;
    }
}

class MyData
{
public:
    MyData(double d) { value_ = d; }

    double value_;
};

int main(int argc, char** argv)
{
    std::cout << "Hello world" << std::endl;
    E::ExcelDate d1(43638);  // 2019 - 06 - 22
    E::ExcelDate d2(41455);  // 2013 - 06 - 30
    E::ExcelDate d3(32577);  // 1989 - 03 - 10
    E::ExcelDate d4(43666);  // 2019 - 07 - 20
    E::ExcelDate d5(47321);  // 2029 - 07 - 22

    ExcelDate2 z2(43638);
    std::cout << z2.to_string() << std::endl;

    std::vector<E::ExcelDate> v;
    for (int i = 0; i < 1000; ++i)
        v.push_back(E::ExcelDate(43638 + i));

    MyData drange(42000.0);
    MyData dinf(40000.0);
    MyData dsup(60000.0);

    test(); return 0;



    MyClass c1;
    std::cout << c1.getValue() << std::endl;
}
