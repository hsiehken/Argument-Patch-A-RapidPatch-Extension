#pragma once

#include <linux/kernel.h>
typedef struct ExecutionContext {
    size_t rcx;
    size_t rdx;
    size_t rsi;
    size_t r15;
    size_t r14;
    size_t r13;
    size_t r12;
    size_t r11;
    size_t r10;
    size_t r9;
    size_t r8;
    size_t rbp;
    size_t rbx;
    size_t rax;
    size_t rdi;
    size_t rdi2;
    size_t rip;
    size_t cs;
    size_t eflags;
} ExecutionContext;