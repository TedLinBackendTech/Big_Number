#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define MAX_SIZE 100
void ReverseArray(char arr[], int size)
{
    for (int i = 0; i < size / 2; i++)
    {
        char temp = arr[i];
        arr[i] = arr[size - 1 - i];
        arr[size - 1 - i] = temp;
    }
}
void add(char RInput[], char LInput[], char Result[])
{
    int i, j, carry = 0;
    int LongerInputLength = strlen(RInput) > strlen(LInput) ? strlen(RInput) : strlen(LInput);
    int shorterInputLength = strlen(RInput) > strlen(LInput) ? strlen(LInput) : strlen(RInput);
    for (i = LongerInputLength - 1, j = shorterInputLength - 1; i >= 0; i--, j--)
    {
        int computeResult = 0;
        computeResult = (LongerInputLength == strlen(RInput)) ? ((RInput[i] - '0') + (j >= 0 ? (LInput[j] - '0') : 0) + carry) : ((LInput[i] - '0') + (j >= 0 ? (RInput[j] - '0') : 0) + carry);
        carry = computeResult / 10;
        Result[LongerInputLength - i - 1] = computeResult % 10 + '0';
    }
    if (carry != 0)
        Result[LongerInputLength] = carry + '0'; //首位補上位數相同的進位
    ReverseArray(Result, strlen(Result));
}
void sub(char RInput[], char LInput[], char Result[])
{
    int i, j, complement = 0;
    int RInputLength = strlen(RInput);
    for (i = RInputLength - 1, j = strlen(LInput) - 1; i >= 0; i--, j--)
    {
        int computeResult = (RInput[i] - '0') - (j >= 0 ? (LInput[j] - '0') : 0) + complement;
        if (computeResult < 0)
        {
            computeResult += 10;
            complement = -1;
        }
        Result[RInputLength - i - 1] = computeResult + '0';
    }
    ReverseArray(Result, strlen(Result));
}
void mul(char RInput[], char LInput[], char Result[])
{
    int i, j, k, carry = 0, zerocomplement = -1, tmplength;
    int LongerInputLength = strlen(RInput) > strlen(LInput) ? strlen(RInput) : strlen(LInput);
    int shorterInputLength = strlen(RInput) > strlen(LInput) ? strlen(LInput) : strlen(RInput);
    char tmpResult[MAX_SIZE];
    char tmpResult1[MAX_SIZE];
    memset(tmpResult1, 0x00, MAX_SIZE);
    for (j = shorterInputLength - 1; j >= 0; j--)
    {
        memset(tmpResult, 0x00, MAX_SIZE);
        zerocomplement++;
        for (i = LongerInputLength - 1; i >= 0; i--)
        {
            int product_tmp = 0;
            product_tmp = (LongerInputLength == strlen(RInput)) ? ((RInput[i] - '0') * (LInput[j] - '0') + carry) : ((LInput[i] - '0') * (RInput[j] - '0') + carry);
            carry = product_tmp / 10;
            tmpResult[LongerInputLength - i - 1] = product_tmp % 10 + '0';
        }
        if (carry != 0)
            tmpResult[LongerInputLength] = carry + '0';
        tmplength = strlen(tmpResult);
        ReverseArray(tmpResult, tmplength);
        if (zerocomplement >= 0)
        {
            for (k = 0; k < zerocomplement; k++)
            {
                tmpResult[tmplength + k] = '0';
            }
        }
        add(tmpResult, tmpResult1, Result);
        strcpy(tmpResult1, Result);
    }
}
int CompareInputBigger(char A[], char B[])
{
    int state = 0;
    if (strlen(A) == strlen(B))
    {
        if (strcmp(A, B) == 0)
            state = 0;
        else
            state = strcmp(A, B) > 0 ? 1 : -1;
    }
    else
        state = strlen(A) > strlen(B) ? 1 : -1;
    return state;
}
void MakeAbsArray(char Input[], char subInput[])
{
    int i, count = 0;
    for (i = 0; i <= strlen(Input); i++)
    {
        if ('-' == Input[i])
            continue;
        subInput[count] = Input[i];
        count++;
    }
    subInput[i - 1] = '\0';
}
bool isNegative(char Input[MAX_SIZE])
{
    return ('-' == Input[0]) ? true : false;
}
bool isZero(char Input[MAX_SIZE])
{
    return ('0' == Input[0]) ? true : false;
}
bool isSameSign(char Input1[MAX_SIZE], char Input2[MAX_SIZE])
{
    if ((Input1[0] == Input2[0]) && ('-' == Input1[0] || Input1[0] > 0)) // 首位相同 且為 - 或 0
        return true;
    else if (strchr(Input1, '-') == NULL && strchr(Input2, '-') == NULL) //首位不同 且不包含負號 即同位
        return true;

    return false;
}
void ExecuteBIOperate(char RInput[MAX_SIZE], char LInput[MAX_SIZE], char Operator[1])
{
    char ResultSign = ' ';
    char subRInput[MAX_SIZE], subLInput[MAX_SIZE];
    char Result[2 * MAX_SIZE];
    int BiggerOne = 0;
    memset(Result, 0x00, sizeof(Result));
    MakeAbsArray(RInput, subRInput);
    MakeAbsArray(LInput, subLInput);
    BiggerOne = CompareInputBigger(subRInput, subLInput);
    if (strncmp(Operator, "+", 1) == 0)
    {
        if (isSameSign(RInput, LInput))
        {
            ResultSign = isNegative(RInput) ? '-' : '+';
            add(subRInput, subLInput, Result);
        }
        else
        {
            if (BiggerOne > 0) //左邊大
            {
                ResultSign = isNegative(RInput) ? '-' : '+';
                sub(subRInput, subLInput, Result);
            }
            else //左邊小 右減左
            {
                ResultSign = isNegative(RInput) ? '+' : '-';
                sub(subLInput, subRInput, Result);
            }
        }
    }
    else if (strncmp(Operator, "-", 1) == 0)
    {
        if (isSameSign(RInput, LInput))
        {
            if (BiggerOne > 0) //左邊大
            {
                ResultSign = isNegative(RInput) ? '-' : '+';
                sub(subRInput, subLInput, Result);
            }
            else //左邊小 右減左
            {
                ResultSign = isNegative(RInput) ? '+' : '-';
                sub(subLInput, subRInput, Result);
            }
        }
        else
        {
            ResultSign = isNegative(RInput) ? '-' : '+';
            add(subRInput, subLInput, Result);
        }
    }
    else if (strncmp(Operator, "*", 1) == 0)
    {
        if (isZero(RInput) || isZero(LInput))
            Result[0] = '0';
        else
        {
            if (isSameSign(RInput, LInput))
            {
                ResultSign = '+';
                mul(subRInput, subLInput, Result);
            }
            else
            {
                ResultSign = '-';
                mul(subRInput, subLInput, Result);
            }
        }
    }
    printf("%c%s\n", ResultSign, Result);
}

int main()
{

    //add
    ExecuteBIOperate("9999999", "987", "+");   //+10000986
    ExecuteBIOperate("-9999999", "-987", "+"); //-10000986
    ExecuteBIOperate("1111333", "333", "+");   //+1111666
    ExecuteBIOperate("333", "-1111333", "+");  //-1111000
    ExecuteBIOperate("-333", "1111333", "+");  //+1111000
    ExecuteBIOperate("333", "-1111333", "+");  //-1111000
    ExecuteBIOperate("333", "1111333", "+");   //+1111666

    // // //sub
    ExecuteBIOperate("-9999999", "987", "-");  //-10000986
    ExecuteBIOperate("9999999", "-987", "-");  //+10000986
    ExecuteBIOperate("9999999", "987", "-");   //+9999012
    ExecuteBIOperate("-9999999", "-987", "-"); //-9999012

    ExecuteBIOperate("333", "1111333", "-");   //-1111000
    ExecuteBIOperate("-333", "-1111333", "-"); //+1111000
    ExecuteBIOperate("333", "-1111333", "-");  //+1111666
    ExecuteBIOperate("-333", "1111333", "-");  //-1111666
    // //mul
    ExecuteBIOperate("0", "123", "*");     //0
    ExecuteBIOperate("12345", "45", "*");  //555525
    ExecuteBIOperate("123", "123", "*");   // 15129
    ExecuteBIOperate("-123", "123", "*");  // -15129
    ExecuteBIOperate("123", "-123", "*");  // -15129
    ExecuteBIOperate("-123", "-123", "*"); // 15129
    ExecuteBIOperate("1231", "123", "*");  // 151413
    ExecuteBIOperate("-123", "1231", "*"); // 151,413

    ExecuteBIOperate("-2", "9999999999999999999999999999", "*"); //-19999999999999999999999999998
    ExecuteBIOperate("9999999999999999999999999999", "-2", "*"); //-19999999999999999999999999998

    return 0;
}
