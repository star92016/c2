/**
package . a.b ;
//@
/*@*\/
import
#@
namespace
{}
final eq=block
"@" '@'
*/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#ifndef bool
#define bool int
#define false 0
#define true 1
#endif

#define CHARBUFSIZE 1024
#define INPUTBUFSIZE 16

//类型定义 
struct String;
typedef struct String String;
struct Token;
typedef struct Token Token;
typedef enum TokenType{
	TYPE_EOF=0,
	TYPE_PACKAGE,
	TYPE_IDS,
	TYPE_A,//;
	TYPE_B,//{
	TYPE_C,//}
	TYPE_IMPORT,
	TYPE_MACRO,
	TYPE_LINECOM,
	TYPE_BLOCKCOM,
	TYPE_NAMESPACE,
	TYPE_UNKNOW,
	TYPE_FINAL,
	TYPE_FINAL_BLOCK,
	TYPE_STATIC,
	TYPE_INLINE,
	TYPE_STRING,
} TokenType;

struct Token{
	TokenType type;
	String* str;
	Token*next,*prev;
};
struct String{
	int len;
	char* data;
	String *realLink; 
}; 



//应用名 
char* appName;
//Token 头尾 
Token* tokenHead,*tokenTail;
//file
FILE* inputFile=NULL;
//输出目录 
char* outDir;
//mark final
bool isFinalMarked=false;
#if 0
//mark namespace
Token* namespaceMark=NULL;
#endif
Token*currentToken=NULL;
//buf
char charBuf[CHARBUFSIZE];
int charBufPos=0;
//input buf
int inputBuf[INPUTBUFSIZE];
int inputBufPos=0;
//outfile
FILE* outC=NULL,*outH=NULL;

//extern fun
String* dupString(char*str);
String* dupStringFromBuf();
String* getLinkedString(String*s);
void writeString2Header(char*str);
void writeString2Source(char*str);
void die(const char*fmt,...);
void clearBuf();
void add2Buf(int ch);
int nextChar();
void putBackChar(int ch);
String* findString(char*str);

void* c2malloc(size_t size){
	void*s=malloc(size);
	if(!s)die("No mem!\n");
	return s;
}
void* c2realloc(void*s,size_t size){
	void*s1=realloc(s,size);
	if(!s1)die("No mem!\n");
	return s1;
}
#define malloc c2malloc
#define realloc c2realloc

void help(){
	fprintf(stderr,"usage: %s <inputfile> [-o <path>]\n",appName);
	fprintf(stderr,"    inputfile   The file you want to process\n");
	fprintf(stderr,"    path        The path of output files, default is current dir\n");
	exit(1);
}

//新token 
Token* newToken(){
	Token*t=malloc(sizeof(Token));
	t->next=NULL;
	t->prev=currentToken;
	
	tokenTail=t;
	if(tokenHead==NULL)tokenHead=t;
	
	if(currentToken)currentToken->next=t;
	currentToken=t;
	
	return t;
}
enum TokenStatusType{
	EMPTY,
	INSTRING,
	INCHARS,
	LINECOM,
	BLOCKCOM,
	ID,
	MACRO,
	MAYBECOM,
	UNKNOW,
	MAYENDBLOCKCOM,
};
int isEmptyChar(int ch){
	switch(ch){
		case ' ':
		case '\t':
		case '\n':
		case '\r':
			return 1;
		default:
			return 0;
	}
}
int isABC_(int ch){
	if((ch>='A'&&ch<='Z')||(ch>='a'&&ch<='z')||ch=='_')return 1;
	return 0;
}
int isDot(int ch){
	return ch=='.';
}
int isNum(int ch){
	if(ch>='0'&&ch<='9')return 1;
	return 0;
}
int isUnknow(int ch){
	if(isNum(ch)||isEmptyChar(ch))return 1;
	if(isABC_(ch))return 0;
	if(ch=='{'||ch==';'||ch=='}'||ch=='\''||ch=='\"') return 0;
	return 1;
}
//下一个 putBackChar nextChar
Token* nextToken(){
	if(currentToken&&currentToken->next){
		currentToken=currentToken->next;
		return currentToken;
	}
	Token*t= newToken();
	clearBuf();
	int state=EMPTY,laststate,acp_new_line=0,has_DIV=0;
	int ch=nextChar();
	if(ch<=0){
		t->type=TYPE_EOF;
		t->str=dupString("EOF");
		return t;
	}
	while(ch>0){
		switch(state){
			case EMPTY:
				if(!isEmptyChar(ch)){
					if(isABC_(ch)||isDot(ch)){
						add2Buf(ch);
						state=ID;
					}else if(ch=='#'){
						add2Buf(ch);
						state=MACRO;
					}else if(ch=='/'){
						//add2Buf(ch);
						laststate=state;
						state=MAYBECOM;
					}else if(ch=='\"'){
						state=INSTRING;
						add2Buf(ch);
					}else if(ch=='\''){
						state=INCHARS;
						add2Buf(ch);
					}else{
						if(ch=='{'||ch==';'||ch=='}'){
							add2Buf(ch);
							state=UNKNOW;
							goto endwhile;
						}else{
							putBackChar(ch);
							state=UNKNOW;
						}
					}
				}
				break;
			case ID:
				if(isABC_(ch)||isDot(ch)||isNum(ch)){
					add2Buf(ch);
				}else{
					putBackChar(ch);
					goto endwhile;
				}
				break;
			case MACRO:
				if(acp_new_line){
					acp_new_line=0;
					add2Buf(ch);
				}else if(ch=='/'){
					//add2Buf(ch);
					laststate=state;
					state=MAYBECOM;
				}else if(ch=='\\'){
					acp_new_line=1;
					add2Buf(ch);
				}else if(ch=='\r'||ch=='\n'){
					goto endwhile;
				}else{
					add2Buf(ch);
				}
				break;
			case MAYBECOM:
				if(ch=='/'||ch=='*'){
					if(laststate!=EMPTY){
						putBackChar('/');
						putBackChar(ch);
						state=laststate;
						goto endwhile;
					}else{
						if(ch=='/'){
							add2Buf('/');
							add2Buf(ch);
							state=LINECOM;
						}else{
							add2Buf('/');
							add2Buf(ch);
							state=BLOCKCOM;
						}
					}
				}else{
					if(laststate!=EMPTY){
						add2Buf('/');
						putBackChar(ch);
						state=laststate;
					}else{
						add2Buf('/');
						putBackChar(ch);
						state=UNKNOW;
					}
				}
				break;
			case UNKNOW:
				if(!isUnknow(ch)){
					putBackChar(ch);
					goto endwhile;
				}else{
					add2Buf(ch);
				}
				break;
			case LINECOM:
				if(ch=='\r'||ch=='\n'){
					goto endwhile;
				}else{
					add2Buf(ch);
				}
				break;
			case BLOCKCOM:
				if(ch=='*'){
					state=MAYENDBLOCKCOM;
				}
				add2Buf(ch);
				break;
			case MAYENDBLOCKCOM:
				if(ch=='/'){
					add2Buf(ch);
					state=BLOCKCOM;
					goto endwhile;
				}else if(ch=='*'){
					add2Buf(ch);
				}else{
					add2Buf(ch);
					state=BLOCKCOM;
				}
				break;
			case INSTRING:
				if(has_DIV){
					add2Buf(ch);
					has_DIV=0;
				}else{
					add2Buf(ch);
					if(ch=='\"')
						goto endwhile;
				}
				break;
			case INCHARS:
				if(has_DIV){
					add2Buf(ch);
					has_DIV=0;
				}else{
					add2Buf(ch);
					if(ch=='\'')
						goto endwhile;
				}
				break;
			default:
				die("bad state\n");
		}
		ch=nextChar();
	}
	if(ch<=0)putBackChar(ch);
endwhile:
	/*
	TYPE_EOF=0,

	TYPE_FINAL_BLOCK,

	,*/
	t->str=dupStringFromBuf();
	switch(state){ 
		case ID:
			t->type=TYPE_IDS;
			{
				if(t->str==findString("package"))
				    t->type=TYPE_PACKAGE;
				else if(t->str==findString("import"))
				    t->type=TYPE_IMPORT;
				else if(t->str==findString("namespace"))
				    t->type=TYPE_NAMESPACE;
				else if(t->str==findString("final"))
				    t->type=TYPE_FINAL;
				else if(t->str==findString("static"))
				    t->type=TYPE_STATIC;
				else if(t->str==findString("inline"))
				    t->type=TYPE_INLINE;
			}
			break;
		case MACRO:
			t->type=TYPE_MACRO;
			break;
		case UNKNOW:
			t->type=TYPE_UNKNOW;
			{
				 if(t->str==findString(";"))
				    t->type=TYPE_A;
				else if(t->str==findString("{"))
				    t->type=TYPE_B;
				else if(t->str==findString("}"))
				    t->type=TYPE_C;
			}
			break;
		case LINECOM:
			t->type=TYPE_LINECOM;
			break;
		case BLOCKCOM:
			t->type=TYPE_BLOCKCOM;
			break;
		case INSTRING:
		case INCHARS:
			t->type=TYPE_STRING;
			break;
		case EMPTY:
			t->type=TYPE_EOF;
		    t->str=dupString("EOF");
			break;
		default:
			printf("ch=%d\n",ch);
			die("bad state\n");
			break;
	}
	return t;
}
//上一个
Token* prevToken(){
	if(currentToken&&currentToken->prev){
		currentToken=currentToken->prev;
		return currentToken;
	}
	die("No Token Before (%s)\n",currentToken?(currentToken->str->data):"NULL");
} 
//缓存 
void add2Buf(int ch){
	if(charBufPos>=CHARBUFSIZE)die("No charBuf ...\n");
	charBuf[charBufPos++]=ch;
}
void clearBuf(){
	charBufPos=0;
}
String* newString(){
	String*s=malloc(sizeof(String));
	s->data=NULL;
	s->realLink=NULL;
}
//复制string
String* dupStringFromBuf(){
	charBuf[charBufPos]=0;
	if(charBufPos>0)return dupString(charBuf);
	return dupString("");
	//die("build String from Empty buf\n");
}
static String**cached_string=NULL;
static int cache_size=512; 
static int cache_len=0;
String* findString(char*str){
	int i;
	for(i=0;i<cache_len;i++){
		if(strcmp(cached_string[i]->data,str)==0){
			return cached_string[i];
		}
	}
	return NULL;
}
void cacheString(String*s){
	//TODO
	if(!cached_string){
		cached_string=malloc(sizeof(String*)*cache_size);
	}
	if(cache_len>=cache_size){
		cache_size*=2;
		cached_string=realloc(cached_string,cache_size);
	}else{
		cached_string[cache_len++]=s;
	}
}
String* dupString(char*str){
	String *s=findString(str);
	if(!s){
		s=newString();
		s->data=malloc(strlen(str)+1);
		strcpy(s->data,str);
		cacheString(s);
	}
	
	return s;
}
//下一个 
int nextChar(){
	if(inputBufPos>0)return inputBuf[--inputBufPos];
	int ch= getchar();
#ifdef WIN32
	if(ch=='\r'){
		ch=getchar();
		if(ch!='\n'){
			putBackChar(ch);
			ch='\r';
		}
	}
#endif
	return ch;
}
//归还 
void putBackChar(int ch){
	if(inputBufPos>=INPUTBUFSIZE)die("too Many putBackChar\n");
	inputBuf[inputBufPos++]=ch;
}
#if 0
//标记namepsace 
void markNamespace(){
	namespaceMark=currentToken;
}
//复原 
void gotoNamespace(){
	currentToken=namespaceMark;
	if(!currentToken)die("Not Mark Before it \n");
}
#endif
//标记final
void markFinal(){
	isFinalMarked=true;
} 
Token*nextTokenNoCom(){
	Token*t=nextToken();
	while(t->type==TYPE_LINECOM||t->type==TYPE_BLOCKCOM){
		t=nextToken();
	}
	return t;
}
static char* packageName=NULL;
static void fitPackage(Token*t1,Token*t2,Token*t3);
static void fitImport(){
	Token*t1=nextTokenNoCom(),*t2=nextTokenNoCom();
	if(t1->type!=TYPE_IDS||t2->type!=TYPE_A)die("import die\n");
	printf("get imports %s\n",t1->str->data);
}
static void fitImportOrMacro(){
	Token*t=nextTokenNoCom();
	while(t->type==TYPE_MACRO)t=nextTokenNoCom();
	if(t->type==TYPE_IMPORT){
		fitImport();
		fitImportOrMacro();
	}else{
		return;
	}
}
static void fitVarOrFunc(){
	//static and inline returnType IDS(PARMS) BLOCK
	//static Type IDS;
	//static Type IDS=BLOCK OR 
}
static void fitApp(){
	fitVarOrFunc();
	fitApp();
}
static void fitNamespace(){
	Token*t1,*t2,*t3;
	t1=nextTokenNoCom();
	t2=nextTokenNoCom();
	if(t1->type!=TYPE_NAMESPACE||t2->type!=TYPE_B)die("namespace die\n");
	fitAPP();
	t3=nextTokenNoCom();
	if(t3->type!=TYPE_C)die("namespace die\n");
}
static void fitProgram(){
	fitPackage(nextTokenNoCom(),nextTokenNoCom(),nextTokenNoCom());
	fitImportOrMacro();
	fitNamespace();
}
static void fitPackage(Token*t1,Token*t2,Token*t3){
	if(t1->type!=TYPE_PACKAGE||t2->type!=TYPE_IDS||t3->type!=TYPE_A)die("package wrong\n");
	packageName=t2->str->data;
}

//第一次扫描记录成员 
void firstView(){
	fitProgram();
	/*
	Token*t;
		TYPE_PACKAGE,
	TYPE_IDS,
	TYPE_A,//;
	TYPE_B,//{
	TYPE_C,//}
	TYPE_IMPORT,
	TYPE_MACRO,
	TYPE_LINECOM,
	TYPE_BLOCKCOM,
	TYPE_NAMESPACE,
	TYPE_UNKNOW,
	TYPE_FINAL,
	TYPE_FINAL_BLOCK,
	TYPE_STATIC,
	TYPE_INLINE,
	TYPE_STRING,
	int meet_package=0;
	
	for(t= nextToken();t->type!=TYPE_EOF;t= nextToken()){
		if(t->type==TYPE_LINECOM||t->type==TYPE_BLOCKCOM){
			continue;
		}
		if(t->type==TYPE_PACKAGE){
			if(meet_package)die("repeat package\n");
			meet_package=1;
			continue;
		}
		//TODO
	}*/
}
//从namespace开始 
void secondView(){
	//open out file
	//gotoNamespace();
	currentToken=tokenHead;
	//TODO
} 
//输出新值，如果有新值， 
void writeToken2Header(Token*tok){
	String *s=getLinkedString(tok->str);
	writeString2Header(s->data);
}
void writeToken2Source(Token*tok){
	String *s=getLinkedString(tok->str);
	writeString2Source(s->data);
}
void writeString2Header(char*str){
	if(outH==NULL)die("header File not open\n");
	if(fwrite(str,strlen(str),1,outH)<=0)die("header File not open\n");
}
void writeString2Source(char*str){
	if(outC==NULL)die("source File not open\n");
	if(fwrite(str,strlen(str),1,outC)<=0)die("source File not open\n");
}
//标识符名字映射包括局部变量 
void mapNewString(String *old,String *newer){
	old->realLink=newer;
}
//String.linked
String* getLinkedString(String*s){
	return s->realLink?s->realLink:s;
}
//获取后缀 
String* getSuffix(String*s){
	char*data=s->data;
	int i,pos=0;
	for(i=0;i<strlen(data);i++){
		if(data[i]=='.')pos=i;
	}
	if(data[pos]=='.')pos++;
	//copy for safe
	char buf[100];
	strcpy(buf,data+pos);
	return dupString(buf);
}
//异常退出 
void die(const char*fmt,...){
	//TODO
	puts(fmt);
	if(inputFile)fclose(inputFile);
	if(outC)fclose(outC);
	if(outH)fclose(outH);
	exit(1);
}
//解析参数，初始化全局变量 
void appInit(int argc,char**argv){
	appName=argv[0];
	//TODO
}

int main(int argc,char**argv){
#if 0
	appInit(argc,argv);
	firstView();
	secondView();
	if(inputFile)fclose(inputFile);
	if(outC)fclose(outC);
	if(outH)fclose(outH);
	return 0;
#endif
	Token*t=nextToken();
	while(t->type!=TYPE_EOF){
		printf("%d: %s\n",t->type,t->str->data);
		t=nextToken();
	}
	printf("%d: %s\n",t->type,t->str->data);
} 
