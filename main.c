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
//��token 
Token* newToken(){
	Token*t=malloc(sizeof(Token));
}
//��һ�� 
Token* nextToken(Token*cur){
	return 0;
}
//��һ��
Token* prevToken(Token*cur){
} 
//���� 
void add2Buf(int ch){
}
void clearBuf(){
}
//����string
String* dupStringFromBuf(){
}
String* dupString(char*str){
}
//��һ�� 
int nextChar(){
}
//�黹 
void putBackChar(int ch){
}
//���namepsace 
void markNamespace(){
}
//��ԭ 
void gotoNamespace(){
}
//���final
void markFinal(){
} 
//��һ��ɨ���¼��Ա 
void firstView(){
}
//��namespace��ʼ 
void secondView(){
} 
//�����ֵ���������ֵ�� 
void writeToken2Header(Token*tok){
}
void writeToken2Source(Token*tok){
}
void writeString2Header(char*str){
}
void writeString2Source(char*str){
}
//��ʶ������ӳ������ֲ����� 
void mapNewString(String *old,String *newer){
} 
String* getSuffix(String*s){
}

int main(int argc,char**argv){
	appName=argv[0];
	//help();
	
	return 0;
} 
