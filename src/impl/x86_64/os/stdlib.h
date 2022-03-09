
#ifndef stdlib_memcpysetetc__base
#define stdlib_memcpysetetc__base

#include "types.h"
#include "alloc.h"
namespace std {
    bool isNegative(int i) {
        return (i < 0);
    }


    int __find_match_in_arr(char* src, char match) {
        int n_t_offset = 0;
        while (*src++ != match) {n_t_offset++;}
        return n_t_offset;
    }
    int __find_null_terminator_in_arr(const char* src) {
        int n_t_offset = 0;
        while (*src++ != '\0') {n_t_offset++;}
        return n_t_offset;
    }
    // First value is the amount of digits. 255 digits should be sufficient. 
    char* __num_to_char_array_(int num) {
        bool _isNegative = 0;
        if (num < 0) {
            // Negative number, draw a '-' before numbers.
            num = num*-1; // (-)i * (-)i = (+)i   | Do this so it's easier to understand what's being printed.
            _isNegative = 1;
        }
        int amt_digits = digitsInNum(num, 10);
        char* digits = (char*)calloc(1 + amt_digits + (_isNegative )? 1:0, sizeof(num));
        int digits_in_output = amt_digits + (_isNegative )? 1:0; // Add the '-' sign if it's negative, otherwise set it to the amount of digits.
        

        if (_isNegative) {
            digits[1] = '-';
        }

        // We go in the reverse order, since c++ and math is mean on my poor head
        int i = amt_digits;
        while( num != 0 )
        {
            const int least_significant_digit = num % 10 ;
            digits[i] = (unsigned char)( least_significant_digit + 48 ); 
            num /= 10;
            i--;
        }

        return digits;
    }

    // Copies string to [dest]. Stops once it gets 0x00 or \0
    void strcpy(char *dest, const char *src)
    {
        while ((*dest++ = *src++) != '\0'); // Iterate over dest & copy the same value of src[i] into it
    }
    // Copy N bytes of src into dest
    void strncpy(char *dest, const char *src, size_t n)
    {
        while ((*dest++ = *src++) > n); // Iterate over dest & copy the same value of src[i] into it
    }
}



static unsigned long int __next_seed = 1;  // NB: "unsigned long int" is assumed to be 32 bits wide
int RAND_MAX = 32767;
long long rand(void)  // RAND_MAX assumed to be 32767
{
    __next_seed = __next_seed * 1103515245 + 12345;
    return (unsigned long long) (__next_seed / 65536) % RAND_MAX;
}
 

void srand(unsigned int seed)
{
    __next_seed = seed;
}

#endif