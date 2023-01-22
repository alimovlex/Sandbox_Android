/*
    Tutorial.cpp
    The module represents my demo functions and its assessment.

    Sandbox

    Created by alimovlex.
    Copyright (c) 2020 alimovlex. All rights reserved.
*/
#include <ctime>
#include <cmath>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <ctime>
#include <exception>
#include <algorithm>
#include <iterator>
#include <array>
#include <functional>
#include <numeric>
#include <chrono>
#include <iomanip>
#include <memory>
#include <cstdarg>
#include "Tutorial.hpp"
#include "Logger.hpp"

//----------------------------------------MACROSES
#define MIN(a,b) (((a)<(b)) ? a : b)
#define MAX(a,b) (((a)>(b)) ? a : b)
#define MULTIPLY(a, b) a*b 
#define merge(a, b) a##b 
#define RECEIVE(a) #a
#define MKSTR( x ) #x

//----------------------------------------END
using namespace std;
using namespace chrono;

int fileTest()
{
	fstream MyFile;
	MyFile.open("bin.txt");
	MyFile << "Hello" << endl;
	if (MyFile.is_open()) 
	{
		MyFile << "This is awesome!"<<endl;
	}
	else
	{
		LOGI("Something went wrong");
        return -1;
	}
	MyFile.close();
	MyFile.open("bin.txt");
	if (MyFile.is_open())
	{
		string line;
		while (!MyFile.eof())
		{
			getline(MyFile, line);
			cout << " "<< line;
		}
		cout << endl;
	}
	MyFile.close();
    return 0;

}
int testingPointers()
{
	int x = 1;
	int *pointer_to_x = &x;
	int & nickname_x = x; //the link to x variable
	LOGI("x=%d", x);
	int y = *pointer_to_x;
	y = 3;
	*pointer_to_x = 4;
	nickname_x = *pointer_to_x;
	LOGI("y = %d", *pointer_to_x);
	LOGI("reference to x = %p", nickname_x);
	int *p = &x;
	p = &y;
	*p = 0;
	int j = *p;
	p = nullptr;
	//Another program
	int  var;
	int  *ptr;
	int  **pptr;

	var = 3000;
	// take the address of var
	ptr = &var;
	// take the address of ptr using address of operator &
	pptr = &ptr;
    cout<<hex;//hexadecimal output mode
	// take the value using pptr
	LOGI("Value of var : %d", var);
	LOGI("Value available at *ptr : %p", *ptr); 
	LOGI("Value available at **pptr : %p", **pptr);

    function<int()> fcnPtr;
    fcnPtr = localTimeCheck;
    fcnPtr();
    return 0;
	//j = *p; ERROR
	//now x==xref==3
}

int argumentsTest(int args,...)
{
    va_list ap;
    va_start(ap,args);
    int i, summ;
    for(i=0;i<args;i++)
        printf("Summ: %i\n",va_arg(ap,int));
    summ = va_arg(ap,int);
    va_end(ap);
    return summ;
}

int localTimeCheck()
{
	// current date/time based on current system
	time_t now = time(0);

	// convert now to string form
	char* dt = ctime(&now);

	LOGI("The local date and time is: %s", dt);

	// convert now to tm struct for UTC
	tm *gmtm = gmtime(&now);
	dt = asctime(gmtm);
	LOGI("The UTC date and time is: %s", dt);

    return 0;
}

int dataTypeSizeTest()
{
	LOGI("Size of char : %d byte", sizeof(char));
	LOGI("Size of int : %d bytes", sizeof(int));
	LOGI("Size of short int : %d bytes", sizeof(short int));
	LOGI("Size of long int : %d bytes", sizeof(long int));
	LOGI("Size of signed long int : %d bytes", sizeof(signed long int));
	LOGI("Size of unsigned long int : %d bytes", sizeof(unsigned long int));
	LOGI("Size of float : %d bytes", sizeof(float));
	LOGI("Size of double : %d bytes", sizeof(double));
	LOGI("Size of wchar_t : %d bytes", sizeof(wchar_t));

    return 0;
}
//--------------------------------time measurement functions
int timerFunction(int(func)())
{
	auto start = high_resolution_clock::now();

	// unsync the I/O of C and C++. 
	ios_base::sync_with_stdio(false);

	func();

	auto end = high_resolution_clock::now();

	// Calculating total time taken by the program. 
	double time_taken = duration_cast<nanoseconds>(end - start).count();

	time_taken *= 1e-9;

	cout << "Time taken by function is : " << fixed << time_taken << setprecision(9);
	cout << " sec" << endl;
    return 0;
}

int preprocessingTest()
{
	LOGI("%s", __func__);
	LOGI("Minimum of 20 and 30 = %d", MIN(20, 30));
	LOGI("Maximum of 20 and 30 = %d", MAX(20, 30));
	LOGI(MKSTR(HELLO C++));
	LOGI("Value of __LINE__ : %d", __LINE__);
	LOGI("Value of __FILE__ : %s", __FILE__);
	LOGI("Value of __DATE__ : %s", __DATE__);
	LOGI("Value of __TIME__ : %s", __TIME__);
	LOGI("Multiplication = %d", MULTIPLY(20, 30));
	LOGI("%s", RECEIVE(GeeksQuiz));
    return 0;
}