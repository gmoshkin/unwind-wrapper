#include <stdint.h>

struct registers_dump {
    uint64_t rip;
    uint64_t rbp;
    uint64_t rsp;
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
};
typedef struct registers_dump registers_dump_t;

struct unw_cursor;
typedef struct unw_cursor unw_cursor_t;

struct unw_context;
typedef struct unw_context unw_context_t;

void unwind_wrapper_get_layout(uint64_t *context_size, uint64_t *context_align, uint64_t *cursor_size, uint64_t *cursor_align);
char *unwind_wrapper_strerror(int error_code);

typedef struct unw_cursor unw_cursor_t;

int unwind_wrapper_get_registers(unw_cursor_t *cursor, registers_dump_t *registers_dump);
int unwind_wrapper_get_frame_info(unw_cursor_t *cursor, uint64_t *rip,
                                  char *buffer, int buffer_size, uint64_t *offset);

// You will also need these from libunwind itself
int _Ux86_64_step(unw_cursor_t *cursor);
int _Ux86_64_getcontext(unw_context_t *context);
int _Ux86_64_init_local(unw_cursor_t *cursor, unw_context_t *context);
