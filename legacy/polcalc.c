/*--------------------------------------------------------------------------
Program Name:  POLish CALCulator (actually RPN!)

Created By:    David Bernazzani

               BCOM1114       Tuesday Section  11:45     Fredriksen

Create Date:   08/12/1989

Purpose:       To create a simple calulator based on the HP model of
               PUSHing and POPing...

Calling Sequence: POLCALC <cr>

        Note:  Main program is from page 74-77 in K&R C Programming book.
               I added most of the commands beyond the 4 basic operations.
===========================================================================
                          Revision History
===========================================================================
Version     Date                   Description of Change
-------   --------   ------------------------------------------------------
 1.00     08/12/89   Initial Release.
 1.00a    09/05/90   Cleanup, a few new functions. Commenting.
--------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>     /* If any ancient compiler doesn't have this, just define atof() */
#include <ctype.h>

#define MAXOP  16       /* max size of any single operand, operator, etc */
#define NUMBER '0'      /* signal that number found */
#define REG    '1'      /* signal register selection */
#define TOOBIG '9'      /* signal that string is too big */
#define VERSION "1.00a" /* Version number to display on startup */

#define MAX_STK  100    /* maximum depth of val stack */
#define MAX_REGS 4      /* registers a-d */
int stackIdx = 0;       /* stack pointer : zero indicates empty stackl*/
double val[MAX_STK];    /* this is our main floating point stack */
double regs[MAX_REGS];  /* register set a,b,c,d */


void printHelp(void)
{
    printf("Reverse POL-CALC version %s - implemented by David Bernazzani\n\n", VERSION);
    printf("+  Addition         ~  Unary Minus\n");
    printf("-  Subtraction      ^  Pop Top Stack\n");
    printf("*  Multiplication   %%  Modulus\n");
    printf("/  Division         s  Show All Stack\n");
    printf("?  Show Help        =  Show Stack Top\n");
    printf("a  Select Reg A     b  Select Reg B\n");
    printf("c  Select Reg C     d  Select Reg D\n");
    printf(">  Store to Reg     <  Recall from Reg\n");
    printf("y  Swap Stack       z  Clear Stack and Regs\n");
    printf("x  Clear Stack      q  Quit Program\n");
    printf("\n");
}

void printStack(void)
{
    int i;
    
    for (i=0; i < MAX_REGS; i++)
    {
        printf("Reg   %c = %f\n", 'a'+i, regs[i]);
    }
    
    if (stackIdx == 0)
    {
        printf("[stack empty]\n");
    }
    else
    {
        for (i=0; i < stackIdx; i++)
        {
            if (i==(stackIdx-1))
            {
                printf("Stack %d = %f (top)\n", stackIdx-i, val[i]);
            }
            else
            {
                printf("Stack %d = %f\n", stackIdx-i, val[i]);
            }
        }
    }
}

double push(double f)  /* push f onto value stack */
{
    if (stackIdx < MAX_STK)
    {
        return(val[stackIdx++] = f);
    }
    else
    {
        printf("[stack full]\n");
        return(0);
    }
}

double pop(void)  /* pop top value from stack */
{
    if (stackIdx > 0)
    {
        return(val[--stackIdx]);
    }
    else
    {
        printf("[stack empty]\n");
        return(0);
    }
}

void printStackTop(void)
{
    if (stackIdx > 0)
    {
        printf("Stack = %f\n", push(pop()));
    }
    else
    {
        printf("[stack empty]\n");
    }
}


#define BUFSIZE 100

char buf[BUFSIZE];    /* buffer for ungetch */
int bufp = 0;         /* next free position in buf */

int getch(void)    /* get a (possibly pushed back) character */
{
    return((bufp > 0) ? buf[--bufp] : getchar());
}

int ungetch(int c)    /* push character back on input */
{
    if (bufp > BUFSIZE)
        printf("ungetch: too many characters\n");
    else
      buf[bufp++] = c;
    return 0;
}


int getop(char s[], int lim)  /* get next operator or numeric operand */
{
    int i, c;

    while ((c = getch()) == ' ' || c == '\t' || c == '\n')
        ;
        
    c = tolower(c);
    if (c >= 'a' && c <= 'd')
    {
        s[0] = c | 0x20;
        s[1] = '\0';
        return('1');
    }
    
    if (c != '.' && (c < '0' || c > '9')) /* Not a number or dp */
    {
        return(c);
    }
    
    s[0] = c;
    
    for (i = 1; (c = getchar()) >= '0' && c <= '9'; i++)
    {
        /* collect integer part */
        if (i < lim)
        {
            s[i] = c;
        }
    }
    
    if (c == '.')
    {    /* collect fraction */
        if (i < lim)
            s[i] = c;
        for (i++; (c=getchar()) >= '0' && c <= '9'; i++)
        {
            if (i < lim)
            {
                s[i] = c;
            }
        }
    }
    
    if (i < lim)
    {    /* number is ok */
        ungetch(c);
        s[i] = '\0';
        return(NUMBER);
    }
    else
    {    /* it's too big; skip rest of line */
        while (c != '\n' && c != EOF)
        {
            c = getchar();
        }
        s[lim-1] = '\0';
        return(TOOBIG);
    }
}

int main()    /* reverse Polish calculator */
{
    float temp;
    int i, op, regsel;
    char s[MAXOP];
    double op1, op2;

    regsel = 0;
    for (i = 0; i < MAX_REGS; i++)
    {
        regs[i] = 0.0;
    }
    
    printHelp();

    while ((op = getop(s, MAXOP)) != EOF)
    {
        switch (tolower(op))
        {
            case NUMBER:       /* numeric entry */
                push(atof(s));
                break;
            case '+':          /* add */
                push(pop() + pop());
                printStackTop();
                break;
            case '*':          /* multiply */
                push(pop() * pop());
                printStackTop();
                break;
            case '-':          /* subtract */
                op2 = pop();
                push(pop() - op2);
                printStackTop();
                break;
            case '/':          /* divide */
                op2 = pop();
                if (op2 != 0.0)
                    push(pop() / op2);
                else
                    printf("zero divisor popped\n");
                printStackTop();
                break;
            case '%':          /* modulus */
                op2 = pop();
                if (op2 != 0.0)
                {
                    temp = pop();
                    temp = (int)temp % (int)op2;
                    push(temp);
                    printStackTop();
                }
                else
                {
                    printf("zero divisor popped\n");
                }
                break;
            case '~':          /* change sign (unary minus) */
                printf("Stack = %f\n", push(-pop()));
                break;
            case 's':
                printStack();
                break;
            case '?':
            case 'h':
                printHelp();
                break;
            case 'q':   /* Quit*/
            case 27:    /* ESCAPE */
                return(0);
                break;
            case 'y':          /* swap stack - exchange top with second position */
                op1 = pop();
                op2 = pop();
                push(op1);
                push(op2);
                printStack();
                break;
            case 'x':          /* clear stack */
                printf("Stack Cleared...\n");
                stackIdx = 0;
                val[0] = 0.0;
                break;
            case 'z':          /* clear stack and registers*/
                printf("Stack and Registers Cleared...\n");
                stackIdx = 0;
                val[0] = 0.0;
                regsel = 0;
                for (i = 0; i < MAX_REGS; i++)
                {
                    regs[i] = 0.0;
                }
                break;
            case REG:          /* select register a thru d */
                regsel = (s[0] - 'a');
                break;
            case '>':          /* store selected register */
                regs[regsel] = push(pop());
                printStackTop();
                break;
            case '<':          /* recall selected register */
                push(regs[regsel]);
                printStackTop();
                break;
            case '^':          /* pop stack and discard */
                (void)pop();
                printStackTop();
                break;
            case '=':          /* print top of stack and last selected register */
                printf("Reg %c = %f\n", 'a'+regsel, regs[regsel]);
                printStackTop();
                break;
            default:           /* illegal command */
               printf("unknown command %c\n", op);
               break;
        }
    }
    return 0;
}

/* End of line... */
