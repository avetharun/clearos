#if !defined(____clearos_cstd_iostream_h_)
#define ____clearos_cstd_iostream_h_



#include "math.h"
#include "vga.h"
#include "stdlib.h"
#include "alloc.h"


namespace std {
    
    struct cout_t {
        //bool setVGACursorToPointer;
        private:
        bool debug_mode;
        int _internal_cout_pointer = 0;
        // Runs the functions to ie. move cursor to pointer+1, set colour, etc.
        void pointer_func() {
        }

        public:
        int get_loc() {return _internal_cout_pointer;}
        // Copy N characters to cout
        void nappend(char* arr, int n) {
            if(n + _internal_cout_pointer >= std::D_TextSize + 2) {
                _internal_cout_pointer = 0;
            }
            std::prnnstr(arr, _internal_cout_pointer, n);
            _internal_cout_pointer += n;
        }
        void append(char* arr) {
            int chars_to_print = __find_null_terminator_in_arr(arr);
            if(chars_to_print + _internal_cout_pointer >= std::D_TextSize + 2) {
                _internal_cout_pointer = 0;
            }
            std::prnstr(arr, _internal_cout_pointer);
            _internal_cout_pointer += chars_to_print;
        }
        void flush() {}
        cout_t set_loc(int loc) {
            if (loc > std::D_TextSize) { return *this; }
            _internal_cout_pointer = loc;
            return *this;
        }
        // Print null-terminated string to ConsoleOut
        cout_t operator << (const char* arr) {
            int chars_to_print = __find_null_terminator_in_arr((char*)arr);
            reassign_pointer(chars_to_print);
            std::prnnstr((char*)arr, _internal_cout_pointer, chars_to_print);
            return *this;
        }
        int reassign_pointer(int _num_digits) {
            if (_num_digits + _internal_cout_pointer > std::D_TextSize +2 ) { _internal_cout_pointer = 0; }
            return _internal_cout_pointer;
        }
        cout_t operator << (int num) {
            int _num_digits = digitsInNum(num) + (isNegative(num)) ? 1 : 0;
            if (debug_mode) {
                std::prni(std::loc(0,10), _num_digits);
            }
            _internal_cout_pointer += std::prni(num, _internal_cout_pointer+1);
            return *this;
        }
        cout_t operator & (int debug_state) {
            debug_mode = debug_state;
            return *this;
        }
        cout_t operator % (int position) {
            return this->operator=(position);
        }
        cout_t operator = (int position) {
            _internal_cout_pointer = position;
            return *this;
        }
        cout_t operator += (int position) {
            _internal_cout_pointer += position;
            return *this;
        }
        cout_t operator -= (int position) {
            _internal_cout_pointer -= position;
            return *this;
        }
    };
    cout_t cout;
}






#endif // ____clearos_cstd_iostream_h_