#include <stddef.h>
#include <stdint.h>
#include "libunwind/include/libunwind.h"

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

void unwind_wrapper_get_layout(uint64_t *context_size, uint64_t *context_align, uint64_t *cursor_size, uint64_t *cursor_align) {
    if (context_size) *context_size = sizeof(unw_context_t);
    if (context_align) *context_align = __alignof__(unw_context_t);
    if (cursor_size) *cursor_size = sizeof(unw_cursor_t);
    if (cursor_align) *cursor_align = __alignof__(unw_cursor_t);
}

int unwind_wrapper_get_registers(unw_cursor_t *cursor, registers_dump_t *registers_dump) {
    int rc;

    rc = unw_get_reg(cursor, UNW_X86_64_RIP, &registers_dump->rip);
    if (rc != 0) return rc;
    rc = unw_get_reg(cursor, UNW_X86_64_RBP, &registers_dump->rbp);
    if (rc != 0) return rc;
    rc = unw_get_reg(cursor, UNW_X86_64_RSP, &registers_dump->rsp);
    if (rc != 0) return rc;
    rc = unw_get_reg(cursor, UNW_X86_64_RAX, &registers_dump->rax);
    if (rc != 0) return rc;
    rc = unw_get_reg(cursor, UNW_X86_64_RBX, &registers_dump->rbx);
    if (rc != 0) return rc;
    rc = unw_get_reg(cursor, UNW_X86_64_RCX, &registers_dump->rcx);
    if (rc != 0) return rc;
    rc = unw_get_reg(cursor, UNW_X86_64_RDX, &registers_dump->rdx);
    if (rc != 0) return rc;
    rc = unw_get_reg(cursor, UNW_X86_64_RSI, &registers_dump->rsi);
    if (rc != 0) return rc;
    rc = unw_get_reg(cursor, UNW_X86_64_RDI, &registers_dump->rdi);
    if (rc != 0) return rc;
    rc = unw_get_reg(cursor, UNW_X86_64_R8,  &registers_dump->r8);
    if (rc != 0) return rc;
    rc = unw_get_reg(cursor, UNW_X86_64_R9,  &registers_dump->r9);
    if (rc != 0) return rc;
    rc = unw_get_reg(cursor, UNW_X86_64_R10, &registers_dump->r10);
    if (rc != 0) return rc;
    rc = unw_get_reg(cursor, UNW_X86_64_R11, &registers_dump->r11);
    if (rc != 0) return rc;
    rc = unw_get_reg(cursor, UNW_X86_64_R12, &registers_dump->r12);
    if (rc != 0) return rc;
    rc = unw_get_reg(cursor, UNW_X86_64_R13, &registers_dump->r13);
    if (rc != 0) return rc;
    rc = unw_get_reg(cursor, UNW_X86_64_R14, &registers_dump->r14);
    if (rc != 0) return rc;
    rc = unw_get_reg(cursor, UNW_X86_64_R15, &registers_dump->r15);
    if (rc != 0) return rc;

    return 0;
}

int unwind_wrapper_get_frame_info(unw_cursor_t *cursor, uint64_t *rip, char *buffer, int buffer_size, uint64_t *offset) {
    int rc = 0;
    if (rip) {
        rc = unw_get_reg(cursor, UNW_REG_IP, rip);
        if (rc != 0) return rc;
    }

    return unw_get_proc_name(cursor, buffer, buffer_size, offset);
}

char *unwind_wrapper_strerror(int error_code) {
    switch (error_code) {
        case UNW_ESUCCESS:     return "success";
        case UNW_EUNSPEC:      return "unspec";
        case UNW_ENOMEM:       return "nomem";
        case UNW_EBADREG:      return "badreg";
        case UNW_EREADONLYREG: return "readonlyreg";
        case UNW_ESTOPUNWIND:  return "stopunwind";
        case UNW_EINVALIDIP:   return "invalidip";
        case UNW_EBADFRAME:    return "badframe";
        case UNW_EINVAL:       return "inval";
        case UNW_EBADVERSION:  return "badversion";
        case UNW_ENOINFO:      return "noinfo";
    }

    return "<unknown>";
}
