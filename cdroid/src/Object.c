/*
    Object.c
    The module represents my demo functions from OC-GCC C Library.
    For more, please see the OOStd.h file

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
#include <setjmp.h>
#include <unistd.h>
#include <dirent.h>
#include <limits.h>
#include <float.h>
#include <pthread.h>
#include "OOStd.h"
#include "Object.h"
#include "Logger.h"
//--------------------------------------------------------FIRST TUTORIAL----------------------------------------

static int Animal_Load(Animal *THIS,void *name)
{
    THIS->name = name;
    return 0;
}
ASM(Animal, Animal_Load, NULL, NULL, NULL) //initializer
static void Meow(Animal *THIS)
{
    LOGI("Meow!My name is %s!", THIS->name);
}

static int Cat_loadSt(StAnimal *THIS, void *PARAM)
{
    THIS->talk = (void *)Meow;
    return 0;
}
ASM_EX(Cat,Animal, NULL, NULL, Cat_loadSt, NULL) //initializer
static void Woof(Animal *THIS)
{
    LOGI("Woof!My name is %s!", THIS->name);
}

static int Dog_loadSt(StAnimal *THIS, void *PARAM)
{
    THIS->talk = (void *)Woof;
    return 0;
}
ASM_EX(Dog, Animal, NULL, NULL, Dog_loadSt, NULL)

int test_Animals_Class()
{
    Animal *animals[4];
    StAnimal *f;
    int i = 0;
    for (i=0; i<4; i++)
    {
        if(i%2==0)
            animals[i] = NEW(Dog,"Jack");
        else
            animals[i] = NEW(Cat,"Lily");
    };
    f = ST(animals[0]);
    for(i=0; i<4; ++i) {
        f->talk(animals[i]);
    }
    for (i=0; i<4; ++i) {
        DELETE0(animals[i]);
    }
    return 0;
}