# c2语言

C语言以其独特的特性发挥着它的作用，但是C语言没有包和类的概念，导致所有的函数和全局变量都在一个命名空间下，极不方便。如果能利用java包的特性，而又不丢失其原本的语言特性，就很极大方便开发者定义和查找变量以及函数。就像导入包一样import `cn.abc.Student`而不用#include"student.h"；使用Student.say()调用函数，而不是StudentSay()这种开发者自己定义的全局函数；使用全局变量Student.isruning而不是Student_isrunning。也就是说让c语言编程只有静态函数，静态变量的java语言，尽管没有类的概念，但还是能方便不少。

定义这种新语言，语法规则不需要太大的调整，c语言所有的语法都能支持，因为最后还是转成c语言代码编译执行。引入package,import,final,namespace等新的关键词，一个c2文件可以有多个namespace ABC{},但只能有一个package .;所有的c2文件需要组织成java树目录结构。最终生成两个文件，一个头文件，一个c源文件，import 会转换成#include语句，所有的函数变量都会在生成头文件自动声明，包括内联函数只能在头文件定义，类似的每个变量、函数前有文档注释（方便后期生成帮助文档），final a 会转换成带有命令空间的宏定义。

尽管如此简单，但是语言的转换，仍然需要经过词法、句法分析等步骤，除此之外还要让程序不能出现package,import,final,namespace这些关键词作为变量名函数名。所有的以#开头的宏指令，要全部跳过分析，最终原样输出。

永远记住c2语言，它生成的目标仍然是c语言源代码，它的主要目的是方便拥有大量变量函数的c语言代码的组织，因此一个简单的程序不需要c2语言。因此，程序员可以故意定义一个符合命名规则的cn_abc_Teacher_name的全局变量，让其编译不能通过，当然这就要看他怎么玩了。

### 这是它的源文件，在cn/abc/Teacher.c2
```c
package cn.abc;
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
}
```
