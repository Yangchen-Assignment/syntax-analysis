
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


enum symbol {
    nul,    atoken, btoken, lparen, rparen, comma, err
};


int token;    //��ǰ�ķ���
char ch;

FILE* fin;

void gettoken();
void parseS();
void parseL();
void parseR();

int main()
{
    char filename[20];

	printf("������������ļ���:");
	gets(filename);
	//scanf("%s",filename);
	do
	{
 		if((fin=fopen(filename,"r"))==NULL)
		{
			printf("���ܴ��ļ�.\n");
			return 0;
		}
		gettoken();            //����һ�����ʣ�������������token��
		parseS();                // ���ÿ�ʼ���Ŷ�Ӧ���ӳ���
		if (token==nul)
			{printf("�﷨��ȷ\n"); }
		else {printf("�﷨����\n");}
		fclose(fin);

		printf("���������������ļ���������س�");
		//scanf("%s",filename);
		gets(filename);
	} while (strlen(filename)>0);
	return 0;
}

/*
* �򻯵Ĵʷ���������ȡһ�����Ŵ浽token��
*/
void gettoken()
{
	ch=fgetc(fin);

	while (ch==' ' || ch==10 || ch==13 || ch==9)  /* ���Կո񡢻��С��س���TAB */
		ch=fgetc(fin);

	if (ch==EOF)  token=nul;

    else if (ch=='a' )
	    token = atoken;
	else if(ch == 'b')
		token = btoken;
    else if (ch == '(')
		token = lparen;
    else if (ch == ')')
		token = rparen;
    else if (ch == ',')
		token = comma;
	else {printf("�ʷ�����\n"); exit(0);}
	return;
}


/*
S->(L)|a
*/
void parseS()
{
	if(token==err) return ;
	if(token==lparen)
	{
		gettoken();
		parseL();
		if(token==rparen) {gettoken();}
		else {token=err;}
	}
	else if(token==atoken){gettoken();}
	else {token=err;}
}


/* L-->SR */
void parseL()
{
	if(token==err) return ;
	if(token==lparen || token==atoken)
	{
		parseS();
		parseR();
	}
	else {token=err;}
}


/* R-->,S R1 |�� */
void parseR()
{
	if(token==err) return ;
	if(token==comma)
	{
		gettoken();
		parseS();
		parseR();
	}
	else if(token==rparen) {}
	else {token=err;}
}
