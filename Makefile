kernel_source_files := $(shell find src/impl/kernel -name *.c)
kernel_object_files := $(patsubst src/impl/kernel/%.c, build/kernel/%.o, $(kernel_source_files))

x86_64_c_source_files := $(shell find src/impl/x86_64 -name *.c)
x86_64_c_object_files := $(patsubst src/impl/x86_64/%.c, build/x86_64/%.o, $(x86_64_c_source_files))


x86_64_cxx_source_files := $(shell find src/impl/x86_64 -name *.cpp)
x86_64_cxx_object_files := $(patsubst src/impl/x86_64/%.cpp, build/x86_64/%.o, $(x86_64_cxx_source_files))


x86_64_asm_source_files := $(shell find src/impl/x86_64 -name *.asm)
x86_64_asm_object_files := $(patsubst src/impl/x86_64/%.asm, build/x86_64/%.o, $(x86_64_asm_source_files))

x86_64_object_files := $(x86_64_asm_object_files) $(x86_64_c_object_files) $(x86_64_cxx_object_files) 

default_flags := -Wpointer-to-int-cast -fpermissive -nostdlib -pedantic -O2 -masm=att -fno-PIC -ffreestanding -mno-red-zone -nostdinc -mcmodel=kernel -mno-sse3 -mno-ssse3 -mno-sse4.1 -mno-sse4.2 -mno-sse4 -mno-sse4a -mno-3dnow -mno-avx -mno-avx2 -fno-stack-protector
USE_SYSTEM_CSL=1
$(kernel_object_files): build/kernel/%.o : src/impl/kernel/%.c
	mkdir -p $(dir $@) && \
	x86_64-elf-gcc $(default_flags) -c -I src/intf -ffreestanding $(patsubst build/kernel/%.o, src/impl/kernel/%.c, $@) -o $@

$(x86_64_cxx_object_files): build/x86_64/%.o : src/impl/x86_64/%.cpp
	mkdir -p $(dir $@) && \
	x86_64-elf-g++ $(default_flags) -c -I src/intf -ffreestanding $(patsubst build/x86_64/%.o, src/impl/x86_64/%.cpp, $@) -o $@

$(x86_64_c_object_files): build/x86_64/%.o : src/impl/x86_64/%.c
	mkdir -p $(dir $@) && \
	x86_64-elf-gcc $(default_flags) -c -I src/intf -ffreestanding $(patsubst build/x86_64/%.o, src/impl/x86_64/%.c, $@) -o $@

$(x86_64_asm_object_files): build/x86_64/%.o : src/impl/x86_64/%.asm
	mkdir -p $(dir $@) && \
	nasm -f elf64 $(patsubst build/x86_64/%.o, src/impl/x86_64/%.asm, $@) -o $@

.PHONY: build-x86_64 clean
build-x86_64: $(kernel_object_files) $(x86_64_object_files)
	mkdir -p dist/x86_64 && \
	x86_64-elf-ld -n -o dist/x86_64/kernel.bin -T targets/x86_64/linker.ld $(kernel_object_files) $(x86_64_object_files) && \
	cp dist/x86_64/kernel.bin targets/x86_64/iso/boot/kernel.bin && \
	grub-mkrescue /usr/lib/grub/i386-pc -o dist/x86_64/kernel.iso targets/x86_64/iso
clean:
	rm -r build/
