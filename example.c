/* Compile command:
```
musl-gcc -fno-omit-frame-pointer -ggdb -static example.c -o example -L./src/.libs/ -lunwind-x86_64 -lunwind
```
 */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include "wrapper.h"

#define INSPECT(v)                                                      \
    do {                                                                \
        uint8_t *p = (uint8_t *) &v;                                    \
        int64_t s = sizeof(v);                                          \
        printf("sizeof(%s) = %ld\n", #v, s);                            \
        for (int i = 0; i < s; i++) {                                   \
            if (i > 0 && i % 8 == 0) printf("\n");                      \
            printf("%02x ", p[i]);                                      \
        }                                                               \
        printf("\n");                                                   \
    } while(0)

int check_libunwind() {
    printf("check_libunwind in\n");

    uint64_t context_size, context_align, cursor_size, cursor_align;
    unwind_wrapper_get_layout(&context_size, &context_align, &cursor_size, &cursor_align);

    unw_context_t *context = aligned_alloc(context_align, context_size);
    assert(context != 0);
    int rc = _Ux86_64_getcontext(context);
    assert(rc == 0);

    unw_cursor_t *cursor = aligned_alloc(cursor_align, cursor_size);
    assert(cursor != 0);
    rc = _Ux86_64_init_local(cursor, context);
    assert(rc == 0);

    /* INSPECT(*context); */
    /* INSPECT(*cursor); */

    registers_dump_t registers;
    rc = unwind_wrapper_get_registers(cursor, &registers);
    assert(rc == 0);

    printf("rip: 0x%016lx %ld\n", registers.rip, registers.rip);
    printf("rbp: 0x%016lx %ld\n", registers.rbp, registers.rbp);
    printf("rsp: 0x%016lx %ld\n", registers.rsp, registers.rsp);
    printf("rax: 0x%016lx %ld\n", registers.rax, registers.rax);
    printf("rbx: 0x%016lx %ld\n", registers.rbx, registers.rbx);
    printf("rcx: 0x%016lx %ld\n", registers.rcx, registers.rcx);
    printf("rdx: 0x%016lx %ld\n", registers.rdx, registers.rdx);
    printf("rsi: 0x%016lx %ld\n", registers.rsi, registers.rsi);
    printf("rdi: 0x%016lx %ld\n", registers.rdi, registers.rdi);
    printf("r8:  0x%016lx %ld\n", registers.r8,  registers.r8);
    printf("r9:  0x%016lx %ld\n", registers.r9,  registers.r9);
    printf("r10: 0x%016lx %ld\n", registers.r10, registers.r10);
    printf("r11: 0x%016lx %ld\n", registers.r11, registers.r11);
    printf("r12: 0x%016lx %ld\n", registers.r12, registers.r12);
    printf("r13: 0x%016lx %ld\n", registers.r13, registers.r13);
    printf("r14: 0x%016lx %ld\n", registers.r14, registers.r14);
    printf("r15: 0x%016lx %ld\n", registers.r15, registers.r15);

    char buffer[1024];
    int i = 0;
    do {
        uint64_t ip, offset;
        rc = unwind_wrapper_get_frame_info(cursor, &ip, buffer, sizeof(buffer), &offset);
        assert(rc == 0);

        printf("%d: %s+%ld (rip = 0x%lx)\n", i, buffer, offset, ip);
        i += 1;
    } while (_Ux86_64_step(cursor) > 0);

    printf("check_libunwind out\n");

    return 0;
}


void baz() {
    printf("baz in\n");
    check_libunwind();
    printf("baz out\n");
}
void bar() {
    printf("bar in\n");
    baz();
    printf("bar out\n");
}
void foo() {
    printf("foo in\n");
    bar();
    printf("foo out\n");
}

int main() {
    printf("hello\n");
    foo();

    return 0;
}
