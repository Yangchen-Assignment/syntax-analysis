
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//定义单词的类型
enum symbol
{
	nul,	eofsym,	ident,	addsym,	mulsym,	lparen,	rparen,	ltcsym,	
	gtcsym,	eqcsym,	divsym,	subsym,	leqsym,	geqsym,	becsym,	errsym,
};

enum symbol sym; //当前单词的词法记号或者单词的类别
char token[20];	 //当前的标识符字符串

char ch;		//当前的字符
FILE *fin;		//输入的文件
#define al 20	/* 符号的最大长度 */
void getword(); //词法分析
bool flag;

void bexpr();
void expr();
void factor();
void term();

int main()
{
	char filename[20];
	printf("请输入分析的文件名:");
	gets(filename);
	while (strlen(filename) > 0)
	{
		if ((fin = fopen(filename, "r")) == NULL)
		{
			printf("不能打开文件.\n");
			return 0;
		}
		flag=0;
		getword(); //读第一个单词，将单词类别放入token中
		bexpr();	   // 调用开始符号对应的子程序
		if (sym == eofsym)
		{
			printf("合法表达式\n");
			//exit(0);
		}
		else
		{
			printf("非法表达式\n");
		}
		fclose(fin);

		printf("请继续输入分析的文件名:");
		gets(filename);
	} 
	return 0;
}

/*
* 简化的词法分析，获取一个符号存到token中
*/
void getword()
{
	int k;
	ch = fgetc(fin);
	while (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t')/* 跳过所有空格 */
		ch = fgetc(fin); 

	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) // 记录标识符
	{
		k = 0;
		do
		{
			if (k < al - 1)
				token[k++] = ch; // 符号的最大长度为al ，超长就截尾处理
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
		printf("词法错误\n");
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
		if (sym == rparen) //如果当前单词为rparen类型,判断是否为')'
		{
			getword(); //获取一个符号存到token中
		}
		else
		{
			printf("语法错误:缺右括号\n");
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
		printf("语法错误:缺标识符\n");
		flag=1;
		return ;
	}
}

/*
term → factor { * or /  factor}
形如 term = factor *|/ factor *|/ factor *|/ factor ...
*/
void term()
{
	if(flag) return ;
	factor(); //至少一个facotor
	while (sym == mulsym || sym == divsym)
	{
		getword(); //获取一个单词
		factor();
	}
}

/*
expr->term{ + or -  term}
形如expr=term +|- term +|- term +|- term +|- term ...
*/
void expr()
{
	if(flag) return ;
	term(); //至少一个term
	while (sym == addsym || sym == subsym) //循环判断，直到term调用后下一位不是加减类型
	{
		getword(); //获取一个符号存到token中
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
		getword(); //获取一个符号存到token中
		expr();
	}
	else
	{
		printf("语法错误\n");
		flag=1;
	}
	//}
	if(flag) {sym=errsym;return;}
}
