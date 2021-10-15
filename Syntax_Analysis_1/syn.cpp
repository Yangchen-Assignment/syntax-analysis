
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//���嵥�ʵ�����
enum symbol
{
	nul,	eofsym,	ident,	addsym,	mulsym,	lparen,	rparen,	ltcsym,	
	gtcsym,	eqcsym,	divsym,	subsym,	leqsym,	geqsym,	becsym,	errsym,
};

enum symbol sym; //��ǰ���ʵĴʷ��ǺŻ��ߵ��ʵ����
char token[20];	 //��ǰ�ı�ʶ���ַ���

char ch;		//��ǰ���ַ�
FILE *fin;		//������ļ�
#define al 20	/* ���ŵ���󳤶� */
void getword(); //�ʷ�����
bool flag;

void bexpr();
void expr();
void factor();
void term();

int main()
{
	char filename[20];
	printf("������������ļ���:");
	gets(filename);
	while (strlen(filename) > 0)
	{
		if ((fin = fopen(filename, "r")) == NULL)
		{
			printf("���ܴ��ļ�.\n");
			return 0;
		}
		flag=0;
		getword(); //����һ�����ʣ�������������token��
		bexpr();	   // ���ÿ�ʼ���Ŷ�Ӧ���ӳ���
		if (sym == eofsym)
		{
			printf("�Ϸ����ʽ\n");
			//exit(0);
		}
		else
		{
			printf("�Ƿ����ʽ\n");
		}
		fclose(fin);

		printf("���������������ļ���:");
		gets(filename);
	} 
	return 0;
}

/*
* �򻯵Ĵʷ���������ȡһ�����Ŵ浽token��
*/
void getword()
{
	int k;
	ch = fgetc(fin);
	while (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t')/* �������пո� */
		ch = fgetc(fin); 

	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) // ��¼��ʶ��
	{
		k = 0;
		do
		{
			if (k < al - 1)
				token[k++] = ch; // ���ŵ���󳤶�Ϊal �������ͽ�β����
			ch = fgetc(fin);
		} while ((ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'Z'));
		token[k] = '\0';
		if(!(ch==EOF))
			fseek(fin, -1, 1);
		sym = ident;
	}

	else if (ch == '+')
		sym = addsym;
	else if (ch == '*')
		sym = mulsym;
	else if (ch == '/')
		sym = divsym;
	else if (ch == '-')
		sym = subsym;
	else if (ch == '(')
		sym = lparen;
	else if (ch == ')')
		sym = rparen;
	else if (ch == '<')
	{
		ch = fgetc(fin);
		if(ch=='=') sym=leqsym;
		else
		{
			fseek(fin, -1, 1);
			sym = ltcsym;
		}
	}	
	else if (ch == '>')
	{
		ch = fgetc(fin);
		if(ch=='=') sym=geqsym;
		else
		{
			fseek(fin, -1, 1);
			sym = gtcsym;
		}
	}
	else if (ch == '=')
	{
		ch = fgetc(fin);
		if(ch=='=') sym=eqcsym;
		else
		{
			fseek(fin, -1, 1);
			sym = becsym;
		}
	}
	else if (ch == EOF)
		sym = eofsym;
	else
	{
		printf("�ʷ�����\n");
		flag=1;
	}
	return;
}

/*
factor ->(expr)| id
*/
void factor()
{
	if(flag) return ;
	if (sym == lparen) // factor -> (expr)
	{
		getword();
		expr();if(flag) return ;
		if (sym == rparen) //�����ǰ����Ϊrparen����,�ж��Ƿ�Ϊ')'
		{
			getword(); //��ȡһ�����Ŵ浽token��
		}
		else
		{
			printf("�﷨����:ȱ������\n");
			flag=1;
			return ;
		}
	}
	else if (sym == ident) // factor -> id
	{
		getword();
	}
	else 
	{
		printf("�﷨����:ȱ��ʶ��\n");
		flag=1;
		return ;
	}
}

/*
term �� factor { * or /  factor}
���� term = factor *|/ factor *|/ factor *|/ factor ...
*/
void term()
{
	if(flag) return ;
	factor(); //����һ��facotor
	while (sym == mulsym || sym == divsym)
	{
		getword(); //��ȡһ������
		factor();
	}
}

/*
expr->term{ + or -  term}
����expr=term +|- term +|- term +|- term +|- term ...
*/
void expr()
{
	if(flag) return ;
	term(); //����һ��term
	while (sym == addsym || sym == subsym) //ѭ���жϣ�ֱ��term���ú���һλ���ǼӼ�����
	{
		getword(); //��ȡһ�����Ŵ浽token��
		term();
	}
}

/* bexpr->expr| expr < expr | expr > expr | expr <= expr | expr >= expr |expr == expr
 */
void bexpr()
{
	expr();
	if(flag) {sym=errsym;return;}
	//while(sym!=eofsym) {
	if(sym == ltcsym || sym == leqsym || sym == gtcsym || sym == geqsym || sym == becsym || sym == eqcsym)
	{
		getword(); //��ȡһ�����Ŵ浽token��
		expr();
	}
	else
	{
		printf("�﷨����\n");
		flag=1;
	}
	//}
	if(flag) {sym=errsym;return;}
}
