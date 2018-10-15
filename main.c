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
	TYPE_ss,
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
String* getLinkedString(String*s);
void writeString2Header(char*str);
void writeString2Source(char*str);
void die(const char*fmt,...);

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
//下一个 putBackChar nextChar
Token* nextToken(){
	if(currentToken&&currentToken->next){
		currentToken=currentToken->next;
		return currentToken;
	}
	Token*t= newToken();
	// TODO 
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
	die("build String from Empty buf\n");
}
String* dupString(char*str){
	//TODO
	return NULL;
}
//下一个 
int nextChar(){
	if(inputBufPos>0)return inputBuf[--inputBufPos];
	return getchar();
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
//第一次扫描记录成员 
void firstView(){
	Token*t= nextToken();
	while(t->type!=TYPE_EOF){
		//TODO
	}
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
	return dupString(data+pos);
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
	appInit(argc,argv);
	firstView();
	secondView();
	if(inputFile)fclose(inputFile);
	if(outC)fclose(outC);
	if(outH)fclose(outH);
	return 0;
} 
