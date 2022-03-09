#if !defined(clearos_os_cstd_alloc_h__)
#define clearos_os_cstd_alloc_h__
#include "types.h"

#define ALIB_NO_INLINE_RUNNERS
#include "ave_libs.hpp" // Include utility library
#undef  ALIB_NO_INLINE_RUNNERS

typedef struct s_bloc
{
    s_bloc	    *next;
    s_bloc	    *prev;
    bool	       	isFree;
    size_t       	size;
    void	       	*data;
    // Used to signify the end of data, used by checks that look for nullptrs
    private: void            * _padding;public:
    char		    __unused_used_for_header;
    s_bloc(int size) {
        this->size = size;
    }
}	       	   m_bloc;

#define __HEADER_SIZE__ sizeof(struct s_bloc)
#define __ALIGN_ADDR__(size, power) (((size - 1) / power + 1) * power)


// Do this so the user only has to define what they need
#if !defined(__HEADER_SIZE__)
#define __HEADER_SIZE__ sizeof(struct s_bloc)
#endif // __HEADER_SIZE
#if !defined(__ALIGN_ADDR__)
#define __ALIGN_ADDR__(size, power) (((size - 1) / power + 1) * power)
#endif // __ALIGN_ADDR__

m_bloc * base_bloc = nullptr;


int __internal_page_size = 32; // bytes

uintptr_t * __internal_brk = 0;
uintptr_t * __internal_data_seg_end = (uintptr_t * )0xffffffff;

int __get_pagesize_() { return __internal_page_size; }
void __set_pagesize_(int size) { __internal_page_size = size; }

// Increase data space by <increment> bytes. 
// Pass 0 through this function to get current limit
void* sbrk(intptr_t incrememt) {
    if (incrememt > 0) { __internal_brk += incrememt; }
    return __internal_brk; 
}
// Set program data segment upper limit (used for malloc)
uintptr_t * brk(void * addr) {
    uintptr_t * last = __internal_brk;
    __internal_brk = (uintptr_t * )addr;
    return last;
}

// Find first free block of memory, or return nullptr if none is found
m_bloc * __find_free_bloc_(m_bloc * block, size_t req_amt) {
    while (block != nullptr) {
        if (block->isFree == true && block->size >= req_amt) {
            return block;
        }
        block = block->next;
    }
    return nullptr;
}

m_bloc * get_bloc(void *ptr) {
    if (ptr < sbrk(0) && ptr >= base_bloc)
        return (m_bloc * ) (ptr = (char*)ptr - __HEADER_SIZE__);
    return (nullptr);
}

void create_rest_bloc(m_bloc *bloc, size_t size, int pagesize) {
    m_bloc	*rest;
    rest = (m_bloc*)((size_t)bloc->data + bloc->size);
    rest->size = (__ALIGN_ADDR__(size + __HEADER_SIZE__, pagesize)
		    - bloc->size - (__HEADER_SIZE__ * 2));
    rest->isFree = true;
    rest->data = (char*)rest + __HEADER_SIZE__;
    rest->next = nullptr;
    rest->prev = bloc;
    if (bloc == nullptr) { return; }
    bloc->next = rest;
}
m_bloc __dummy_block = m_bloc(16);
m_bloc * create_dummy_bloc() {
    sbrk(64);
    m_bloc	*rest = &__dummy_block;
    rest->size = 16;
    rest->size = (__ALIGN_ADDR__(16 + sizeof(struct s_bloc), 16)
		    - 16 - (sizeof(struct s_bloc) * 2));
    rest->isFree = true;
    rest->data = (char*)rest + __HEADER_SIZE__;
    rest->next = nullptr;
    rest->prev = rest;
    return rest;
}



m_bloc * extend_heap(m_bloc *last_bloc, size_t size) {
    m_bloc *bloc;
    int    pagesize;
  
    pagesize = __get_pagesize_();
    
    if ((bloc = (m_bloc * )
        sbrk(__ALIGN_ADDR__(size + __HEADER_SIZE__, pagesize))) == (void*)-1) {
            return (nullptr);
        }
    
    if (last_bloc != nullptr) {
        last_bloc->next = bloc;
        bloc->prev = last_bloc;
      }
    else {
        bloc->prev = nullptr;
    }
    bloc->size = size;
    bloc->isFree = false;
    bloc->data = (char*)bloc + __HEADER_SIZE__;
    //create_rest_bloc(bloc, size, pagesize);
    return (bloc);
}



int split_bloc(m_bloc * this_bloc, size_t size) {
    m_bloc * half_bloc;
  
    if (this_bloc->size <= (size + __HEADER_SIZE__)) {
        return (1);
    }
    half_bloc = (m_bloc*)((char*)this_bloc + __HEADER_SIZE__ + size);
    half_bloc->size = this_bloc->size - size - __HEADER_SIZE__;
    half_bloc->next = this_bloc->next;
    half_bloc->prev = this_bloc;
    half_bloc->isFree = true;
    half_bloc->data = (char*)half_bloc + __HEADER_SIZE__;
    this_bloc->size = size;
    this_bloc->next = half_bloc;
    this_bloc->isFree = false;
    return (0);
}

m_bloc * fusion_free_bloc(m_bloc *bloc) {
    m_bloc	*tmp;
  
    tmp = bloc;
    if (bloc->prev != nullptr && bloc->prev->isFree == true) {
        bloc->prev->next = bloc->next;
        bloc->next->prev = bloc->prev;
        bloc->size += bloc->prev->size + __HEADER_SIZE__;
        tmp = bloc->prev;
    }
    if (tmp->next != nullptr && tmp->next->isFree == true) {
        tmp->size += tmp->next->size + __HEADER_SIZE__;
        tmp->next = tmp->next->next;
    }
    return (tmp);
}



// Copy N bytes from src to dest
void memcpy(void* dest, void* src, size_t n) {
    char *chsrc = (char *)src;
    char *chdest = (char *)dest;
    while ((*chdest++ = *chsrc++) < n);
}
// Set n bytes of src to value v (char)
void memset(void* src, char v, size_t n) {
    char *chptr = (char *)src;
    while ((*chptr++ = v) < n);
}

void * malloc( size_t size, char _b = 0x00) {
    m_bloc *this_bloc;
    size = __ALIGN_ADDR__( size, 16 );
    if (base_bloc == nullptr) {
        // First malloc() call, create a "dummy" block
        base_bloc = create_dummy_bloc();
        base_bloc->next = this_bloc;
    }
    // Find first free bloc of size. If none is found, create a new one.
    if ((this_bloc = __find_free_bloc_(base_bloc, size) ) == nullptr) {
        this_bloc = extend_heap(base_bloc, size);
    }
    if (_b != 0x00) {
        memset(this_bloc->data, _b, size);
    }
    return this_bloc->data;
}
// Create array of N members of size S
void * calloc(size_t num_members, size_t size){
    return malloc( num_members * size );
}
void free( void * ptr) {
    if (ptr == nullptr) {}
    m_bloc * this_bloc;
    if ((this_bloc = get_bloc(ptr)) == nullptr) { return; }
    this_bloc->isFree = true;
    this_bloc->data = nullptr;
}

void *memmove(void *dst, const void *src, size_t n)
{
    __u8 *p = (__u8 *)src;
    __u8 *q = (__u8 *)dst;
    __u8 *end = p + n;
    if (q > p && q < end) {
        p = end;
        q += n;
        while (p != src) {
            *--q = *--p;
        }
    }
    else {
        while (p != end) {
            *q++ = *p++;
        }
    }
    return dst;
}

char *strcpy(char *dst, const char *src)
{
    char c;
    char *p = dst;

    while ((c = *src++)) {
        *p++ = c;
    }

    *p = '\0';
    return dst;
}
char *strncpy(char *dst, const char *src, size_t n)
{
    size_t i;

    for (i = 0 ; i < n && src[i] != '\0' ; i++) {
        dst[i] = src[i];
    }

    for ( ; i < n ; i++) {
        dst[i] = '\0';
    }

    return dst;
}
int strcmp(const char *s1, const char *s2)
{
    while (*s1 == *s2) {
        if (*s1 == '\0') {
            return 0;
        }
        ++s1;
        ++s2;
    }
    return *s1 - *s2;
}
size_t strlen(const char *str)
{
    const char *s = str;
    while (*s++){}

    return s - str - 1;
}

// ignore if compiler says this has an error. It is fine
void* operator new[](size_t _sz) {
    return malloc(_sz);
}
void operator delete[](void* ptr) {
    free(ptr);
}
void operator delete[](void* ptr, long unsigned int _unused) {
    free(ptr);
}

#endif // clearos_os_cstd_alloc_h__
