/**
 * @file vector.h
 * @author Avetharun
 * @brief A rough implementation of C++ std::vector<> library, 
 *        for integrated CPUs while providing as much control over the library as possible.
 * @version 0.1
 * @date 2021-12-23
 */

#include "../alloc.h"
namespace std{
    template <typename V_T>
    struct vector{

        int __prealloc_size_b; // Bytes currently allocated
        void* __prealloc_ptr;  // Pointer to previous allocation of vector
                                // (used for dalloc)
        void* __alloc_ptr_first;
        int   __alloc_mem_reserved;
        // Pointer to first allocated object
        void* _callocated_ptr;
        int __sizeOfV_T;
        int __n_indicies;
        // Allocated bytes OF ALL OBJECTS
        int _alloc_bytes;
        // An array of individual pointers. We aren't storing the object, rather referencing it.
        V_T* data; // Array of pointers pointing to specific V_T instances

        int size() { return __n_indicies; }
        int get_end() {
            return size();
        }
        int get_start() {
            return 0;
        }

        int check_allocator() {
            // Returns the amount of bytes free in the vector memory (todo)
            return __alloc_mem_reserved - _alloc_bytes;
        }
        // Reserve N amount of <V_T?> for vector
        void reserve(int N){
            __n_indicies += N;
            // Add the new allocation into the total bytes allocated
            _alloc_bytes = N * __sizeOfV_T + _alloc_bytes;
            int alloc_mem_requested = (__n_indicies * __sizeOfV_T) + (__sizeOfV_T * N);
            void* tmp = malloc(alloc_mem_requested);
            // Requested memory has been reserved, update the reservation amount
            __alloc_mem_reserved = alloc_mem_requested;
            

            // Copy the array to a temporary array
            memcpy(tmp, data, _alloc_bytes);

            // Update internal array to fit new reservation
            free(data);
            data = (V_T*)malloc(alloc_mem_requested);
            memcpy(data, tmp, alloc_mem_requested);
            _callocated_ptr = &data;
        }
        // Removes N amount of <V_T?> in vector
        void revoke(int N) {
            __n_indicies -= N; // Just modify the number of objects. The allocator will fill in the needed things.
        }
        // By default, reserve 1 of type V_T
        vector() { 
            __sizeOfV_T = sizeof(V_T);
            reserve(1);
        }
        // Optional N paramater in initializer
        vector(int N) {
            __sizeOfV_T = sizeof(V_T);
            reserve(N);
        }
        // Shift values around to remove all null objects (removed objects, or undefined.)
        // nullptr values when using [N] are undefined behaviour.
        void __repopulate() {
            void* first;
            void* next;
            void* empty;
            int empty_index;
            void* free;
            for (int i = 0; i < size(); i++) {
                
                if (data[i] == nullptr) {
                    empty_index = i;
                    empty = &data[i];

                    // todo: figure this mess out. Malloc was hard enough today.
                }
            }
        }
        void remove(V_T val) {
            for (int i = 0; i < size(); i++) {
                if (&val == data[i]) {
                    &data[i] = nullptr;
                }
            }
        }
        void remove_at(int N) {
            if (N > size()) { return; }
            &data[N] = nullptr;
        }
        void push_back(V_T val) {
            // Amount of allocated V_Ts is less than the size of array. Use the free space instead of reserving a new one.
            if(__alloc_mem_reserved < _alloc_bytes + __sizeOfV_T) {
                reserve(2);
            } else {
                __n_indicies++;
            }
            data[__n_indicies] = val;
        }



        V_T at(int N) {
            if (N > __n_indicies) {
                return data[size()-1];
            }
            return data[N];
        }
        


    };
}