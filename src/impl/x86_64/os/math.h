#if !defined(STDLIB_MATH_H_CLEAROS)
#define STDLIB_MATH_H_CLEAROS


// 75% as fast as stdmath, if using recursion
int log(int base, int n) {
    return (n > base - 1)
            ? 1 + log(n / base, base)
            : 0;
}
int floor(double x) {return int(x)-(x<0 && int(x)%1!=0);}
int ceil(double x)  {return int(x)+(x>0 && int(x)%1!=0);}
int round(double x) {return floor(x)+(x>0&&int(x)%1>=0.5)+(x<0&&(1+int(x)%1)%1>=0.5);}
int pow(int base, int exponent) {
    for (int i = 0; i < exponent; i++) {
        base *= base;
    }
    return base;
}
float pi = 3.141592f;
// translation table for number printing (hex)
const char TBL[] = "0123456789ABCDEF";



int digitsInNum(long n, int base = 10)
{
    if (n == 0) {return 1;}
    if (n < 0) {
        n *= -1;
    }
    // log using base, then
    // taking it to the lowest int then add 1
    return floor(log(base,n)) + 1;
}

// arr[0] is the amount of digits in the array
int* getDigitsOfNumber(int num) {
    int amt_digits = digitsInNum(num, 10);
    if (amt_digits <= 0) {
        amt_digits = 1;
    }
    int* digits = new int[ amt_digits + 1] ;
    digits[0] = amt_digits;
    int i = 1;
    while( num != 0 )
    {

        // What even is math?
        const int least_significant_digit = num % 10;
        digits[i] = ( least_significant_digit + 48 ); 
        num /= 10;
        i++;
    }
    return digits;
}
#endif // STDLIB_MATH_H_CLEAROS
