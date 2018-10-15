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

//���Ͷ��� 
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



//Ӧ���� 
char* appName;
//Token ͷβ 
Token* tokenHead,*tokenTail;
//file
FILE* inputFile=NULL;
//���Ŀ¼ 
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

//��token 
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
//��һ�� putBackChar nextChar
Token* nextToken(){
	if(currentToken&&currentToken->next){
		currentToken=currentToken->next;
		return currentToken;
	}
	Token*t= newToken();
	// TODO 
	return t;
}
//��һ��
Token* prevToken(){
	if(currentToken&&currentToken->prev){
		currentToken=currentToken->prev;
		return currentToken;
	}
	die("No Token Before (%s)\n",currentToken?(currentToken->str->data):"NULL");
} 
//���� 
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
//����string
String* dupStringFromBuf(){
	charBuf[charBufPos]=0;
	if(charBufPos>0)return dupString(charBuf);
	die("build String from Empty buf\n");
}
String* dupString(char*str){
	//TODO
	return NULL;
}
//��һ�� 
int nextChar(){
	if(inputBufPos>0)return inputBuf[--inputBufPos];
	return getchar();
}
//�黹 
void putBackChar(int ch){
	if(inputBufPos>=INPUTBUFSIZE)die("too Many putBackChar\n");
	inputBuf[inputBufPos++]=ch;
}
#if 0
//���namepsace 
void markNamespace(){
	namespaceMark=currentToken;
}
//��ԭ 
void gotoNamespace(){
	currentToken=namespaceMark;
	if(!currentToken)die("Not Mark Before it \n");
}
#endif
//���final
void markFinal(){
	isFinalMarked=true;
} 
//��һ��ɨ���¼��Ա 
void firstView(){
	Token*t= nextToken();
	while(t->type!=TYPE_EOF){
		//TODO
	}
}
//��namespace��ʼ 
void secondView(){
	//open out file
	//gotoNamespace();
	currentToken=tokenHead;
	//TODO
} 
//�����ֵ���������ֵ�� 
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
//��ʶ������ӳ������ֲ����� 
void mapNewString(String *old,String *newer){
	old->realLink=newer;
}
//String.linked
String* getLinkedString(String*s){
	return s->realLink?s->realLink:s;
}
//��ȡ��׺ 
String* getSuffix(String*s){
	char*data=s->data;
	int i,pos=0;
	for(i=0;i<strlen(data);i++){
		if(data[i]=='.')pos=i;
	}
	if(data[pos]=='.')pos++;
	return dupString(data+pos);
}
//�쳣�˳� 
void die(const char*fmt,...){
	//TODO
	puts(fmt);
	if(inputFile)fclose(inputFile);
	if(outC)fclose(outC);
	if(outH)fclose(outH);
	exit(1);
}
//������������ʼ��ȫ�ֱ��� 
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
