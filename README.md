# c2语言
a programming language from c 一个带有包，命名空间和方便定位函数变量位置的编程语言
### 这是它的源文件，在cn/abc/Teacher.c2
```package cn.abc;
//package .;//is support

import cn.abc.Student;
#include<stdio.h> //is support
/**
教师类
*/
namespace Teacher{
	/**
	年龄类型
	*/
#ifndef WIN32
	typedef Student.AgeType AgeType;
#endif
	AgeType age;
	static char* name;
	//#define a //isok, but it will not be #define cn_abc_Teacher_a
	final /*no int*/ TYPE=4;
	final /*no int*/ equal(a)=({(a)=5;(a);});
	struct A{ int a;};
	typedef struct A A;
	static void say(){
		Student.say();
		name="hello";
	}
	inline void hello(){
		say();
		printf("%s\n",name);
		Teacher.AgeType hello;
	}
}
```
### 这是它输出的头文件在build/cn_abc_Teacher.h
```
/**
教师类
*/
#ifndef _cn_abc_Teacher
#define _cn_abc_Teacher

#include"cn_abc_Student.h"
/**
年龄类型
*/
#ifndef WIN32
typedef cn_abc_Student_AgeType cn_abc_Teacher_AgeType;
#endif
extern cn_abc_Teacher_AgeType cn_abc_Teacher_age;
//#define a //isok, but it will not be #define cn_abc_Teacher_a
#define cn_abc_Teacher_TYPE 4
#define cn_abc_Teacher_equal(a) ({a==5;a;})
void cn_abc_Teacher_hello();
#endif

```
### 这是它输出的源文件在build/cn_abc_Teacher.c
```
#include"cn_abc_Teacher.h"
cn_abc_Teacher_AgeType cn_abc_Teacher_age;
static char* cn_abc_Teacher_name;
static void cn_abc_Teacher_say();//static function at here
static void cn_abc_Teacher_say(){
	cn_abc_Student_say();
	cn_abc_Teacher_name="hello";
}
void cn_abc_Teacher_hello(){
	cn_abc_Teacher_say();
	printf("%s\n",name);
	cn_abc_Teacher_AgeType hello;
}```
