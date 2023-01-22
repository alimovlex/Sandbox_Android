/*
    Tutorial.c
    The module represents my demo functions and its assessment.

    Sandbox

    Created by alimovlex.
    Copyright (c) 2020 alimovlex. All rights reserved.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>
#include <errno.h>
#include <math.h>
#include <tgmath.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <float.h>
#include <pthread.h>
#include <ctype.h>
#include "smart_ptr.h"
//#include <Block.h>
//#include "OOStd.h"
#include "Tutorial.h"
#include "Logger.h"

//Preprocessing section-------------------------------
#define MIN(a,b) (((a)<(b)) ? a : b)
#define MAX(a,b) (((a)>(b)) ? a : b)
#define MULTIPLY(a, b) a*b 
#define merge(a, b) a##b 
#define get(a) #a 
#define MKSTR( x ) #x
#define B(x) StringToBinary_(#x)
unsigned long long StringToBinary_(const char *s)
{
	unsigned long long i = 0;
	while (*s)
	{
		i <<= 1;
		i += *s++ - '0';
	}
	return i;
}
//----------------------------ENDING---------------------

int characterSetTest()
{
	srand((unsigned)time(NULL));
	int hex = rand() % 0xFFF;
	int dec = rand() % 20;
	int bin = rand() % B(111);
	int oct = rand() % 020;
	int i = 0;
	LOGI("Hexadecimal number:0x%02X", hex); //hexadecimal output
	LOGI("Decimal RND number: %d", dec);
	LOGI("Binary RND number: %d", bin);
	LOGI("Octal RND number: %o", oct);
	LOGI("Oct and Hex numbers: %o 0x%02X", oct, hex);
	LOGI("~hex = 0x%02X", ~hex);
	LOGI("bin<<1 = %d", bin << 2);
	LOGI("bin>>1 = %d", bin >> 2);
	LOGI("postfix = %i", i++);//i=1
	LOGI("prefix = %i", ++i);
	/*
	LOGI("In C programming All graphic "
		"characters are: \n");
	for (i = 0; i <= 127; ++i)
		if (isgraph(i) != 0)
			LOGI("%c ", i);
	LOGI("\n");
	*/
    return 0;
	//Fn();
	//free(sizeof(struct numbers));
}

int listFiles()
{
	DIR *dp;
	struct dirent *ep;

	dp = opendir("/storage/");
	if (dp != NULL)
	{
		while (ep = readdir(dp))
			puts(ep->d_name);
		(void)closedir(dp);
	}
	else {
        LOGI("Couldn't open the directory");
        return -1;
    }
    return 0;
}

int testingPointers(int *p, void *ptr, int(funcPtr)())
{
    //Dynamic structure initialization
    Box *m = (Box*)malloc(sizeof(Box));
    m->count = 100.0;
    m->tax_percent = 200;
    m->tax_percent = 100;
    Box num =  { 100.0, 200,100 };// C11 struct init
    LOGI("The size of the structure box has on the heap %ld bytes and "
           "%ld bytes on the stack",sizeof(m),sizeof(num));//sizes
    free(m);
    //------------------Ending---------------------
	int  i = 0,a=10;
	float y = 5.5;
	int **z = &p;
	**z = 70;
	LOGI("p contains address %p", p);
	LOGI("p is pointing to the value %d", *p); //error creating threads.p is pointing to value 70
	//(int*)ptr - does type casting of void  
	//*((int*)ptr) dereferences the typecasted  
	// void pointer variable. 
	LOGI("Integer variable is = %d", *((int*)ptr));
	// void pointer is now float 
	ptr = &y;
	LOGI("Float variable is= %.2f", *((float*)ptr));
    LOGI("funcPtr contains address %p", funcPtr);
    return 0;
}

int pointersTest()
{
	static int sa = 10; //example of static variable (it saves its value over each function calling)
	int j = 63, x = 4, i = 0, a = 10;
	a += 5;
	sa += 5;
	LOGI("a = %d, sa = %d", a, sa);
	LOGI("The address of j is %p", &j);
	LOGI("The value of j is %d", j);
    LOGI("address of function pointersTest is :%p", pointersTest);
    int *some_int = unique_ptr(int, rand()%10);//Using smart pointers library
    int(*func)() = unique_ptr(int, localTimeCheck()); //CHECK THIS STRING
    LOGI("Smart Pointer Address %p has %d Smart Pointer value", some_int, *some_int);
	testingPointers(&j, &x, func);
    return 0;
}

int fileTest(struct android_app *app)
{

	char *asset_txt = NULL;

    AAsset *file = AAssetManager_open(app->activity->assetManager, "text.txt", AASSET_MODE_BUFFER);
    if (file) {
        size_t fileLength = AAsset_getLength(file);
        char *temp = malloc(fileLength + 1);
        memcpy(temp, AAsset_getBuffer(file), fileLength);
        temp[fileLength] = '\0';
        asset_txt = temp;
    }
    
    LOGI("HolyShit I did it!");
    LOGI("Asset \"text.txt\" data: %s", asset_txt ? asset_txt : "Not found");
	
    free(asset_txt);

    return 0;
	/*while (!feof(fp))
	{
	fgets(buff, 255, (FILE*)fp);
	//fscanf(fp, "%s", buff);
	printf("%s\n", buff);
	}
	*/
}

int preprocessingTest()
{
	LOGI("%s", __func__);//print the name of the function
	LOGI("Minimum of 20 and 30 = %d", MIN(20, 30));
	LOGI("Maximum of 20 and 30 = %d", MAX(20, 30));
	LOGI(MKSTR(HELLO C++));
	LOGI("Value of __LINE__ : %d", __LINE__);
	LOGI("Value of __FILE__ : %s", __FILE__);
	LOGI("Value of __DATE__ : %s", __DATE__);
	LOGI("Value of __TIME__ : %s", __TIME__);
	LOGI("Multiplication = %d", MULTIPLY(20, 30));
	//printf(merge("Hello ", "World\n"));
	LOGI("%s", get(GeeksQuiz));
	//printf("\a"); signal exclamation
    return 0;
}

int dataTypeSizeTest()
{
	float x = 0.1;
	long int a = 1;
	__int8_t z = 2;
	char b = 'G';
	long double c = 3.14;
	struct SizeOfTypes;
	//-------------------------------printing the variables defined above along with their sizes 
	LOGI("Size of char %ld",sizeof(b));
	LOGI("Size of long int %ld",sizeof(a));
	LOGI("Size of long double %ld", sizeof(c));
	LOGI("Size of float %ld", sizeof(x));
	LOGI("Size of int %ld", sizeof(z));
	LOGI("Size of structure of data types %ld", sizeof(TypeSize));
    LOGI("Value of INT_MAX %d", INT_MAX);
    LOGI("Value of INT_MIN %d", INT_MIN);
    LOGI("Value of FLT_MAX %f", FLT_MAX);
    LOGI("Value of FLT_MIN %f", FLT_MIN);
    return 0;
}

void timerFunction(int(func)())
{
    clock_t t;
    t = clock();
    func();
    t = clock() - t;
    double time_taken = ((double)t) / CLOCKS_PER_SEC;
    LOGI("function %s took %f seconds to execute ", __FUNCTION__, time_taken);
}

int localTimeCheck()
{
	time_t t;
	struct tm *tmp;
	char MY_TIME[50];
	time(&t);
	//localtime() uses the time pointed by t , 
	// to fill a tm structure with the  
	// values that represent the  
	// corresponding local time. 
	tmp = localtime(&t);
	// using strftime to display time 
	strftime(MY_TIME, sizeof(MY_TIME), "%x - %I:%M%p", tmp);
	LOGI("Formatted date & time : %s", MY_TIME);
	return 0;
}

int argumentsTest(int args,...)
{
    va_list ap;
    va_start(ap,args);
    int summ = va_arg(ap,int);
    int i;
    for(i=0;i<args;i++)
        LOGI("Arguments Summ: %i\n", va_arg(ap,int));
    va_end(ap);
    return summ;
}

int sandbox()
{
	pthread_t t0;
    if(pthread_create(&t0,NULL,characterSetTest,NULL)==-1)
        LOGI("Unable to create a thread for sandbox function");
	void *result;
    if(pthread_join(t0,&result)==-1) {
        LOGI("Can't join thread sandbox");
        return -1;
    }
    else
        return 0;
}




