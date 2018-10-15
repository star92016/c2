#include<stdio.h>
#include<stdlib.h>

char* appName;

void help(){
	fprintf(stderr,"usage: %s <inputfile> [-o <path>]\n",appName);
	fprintf(stderr,"    inputfile   The file you want to process\n");
	fprintf(stderr,"    path        The path of output files, default is current dir\n");
	exit(1);
}
typedef struct Token{
	int type;
	char* str;
} Token;
//新token 
Token* newToken(){
	Token*t=malloc(sizeof(Token));
}
//下一个 
Token* nextToken(Token*cur){
	return 0;
}
//上一个
Token* prevToken(Token*cur){
} 
//缓存 
void add2Buf(int ch){
}
void clearBuf(){
}
//复制string
String* dupStringFromBuf(){
}
String* dupString(char*str){
}
//下一个 
int nextChar(){
}
//归还 
void putBackChar(int ch){
}
//标记namepsace 
void markNamespace(){
}
//复原 
void gotoNamespace(){
}
//标记final
void markFinal(){
} 
//第一次扫描记录成员 
void firstView(){
}
//从namespace开始 
void secondView(){
} 
//输出新值，如果有新值， 
void writeToken2Header(Token*tok){
}
void writeToken2Source(Token*tok){
}
void writeString2Header(char*str){
}
void writeString2Source(char*str){
}
//标识符名字映射包括局部变量 
void mapNewString(String *old,String *newer){
} 
String* getSuffix(String*s){
}

int main(int argc,char**argv){
	appName=argv[0];
	//help();
	
	return 0;
} 
