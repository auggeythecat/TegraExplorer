#include <bdk.h>
#include <string.h>


#include "../../gfx/gfx.h"

// void pika_platform_enable_irq_handle(void);
// void pika_platform_disable_irq_handle(void);
// TODO: figure out what to do here?


void pika_platform_printf(char* fmt, ...) {
    gfx_printf(fmt);
}
int pika_platform_sprintf(char* buff, char* fmt, ...) {
    gfx_printf("I don't want sprintf used!");
}
int pika_platform_vsprintf(char* buff, char* fmt, va_list args) {
    gfx_printf("I don't want vsprintf used!");
}
int pika_platform_vsnprintf(char* buff,
                            u32 size,
                            const char* fmt,
                            va_list args) {
    gfx_printf("I don't want vsnprintf used!");
}
int pika_platform_snprintf(char* buff, u32 size, const char* fmt, ...) {
    gfx_printf("I don't want snprintf used!");
}

char* pika_platform_strdup(const char* src) {
    char* dst = (char*)malloc(strlen(src) + 1);
    if (dst) {
        strcpy(dst, src);
    }
    return dst;
}
u32 pika_platform_tick_from_millisecond(u32 ms);

/* libc config */
void* pika_platform_malloc(u32 size) {
    return malloc(size);
}

// void* pika_platform_realloc(void* ptr, u32 size); //TODO: I'd rather not use realloc...
void* pika_platform_calloc(u32 num, u32 size) {
    return calloc(num, size);
}

void pika_platform_free(void* ptr) {
    return free(ptr);
}

// TODO: I'd rather not if I can avoid it...
// void* pika_platform_memset(void* mem, int ch, u32 size);
// void* pika_platform_memcpy(void* dir, const void* src, u32 size);
// int pika_platform_memcmp(const void* s1, const void* s2, u32 n);
// void* pika_platform_memmove(void* s1, void* s2, u32 n);

/* pika memory pool config */
void pika_platform_wait(void) {
    bpmp_halt();
}

/* support shell */
char pika_platform_getchar(void);
int pika_platform_putchar(char ch);

// TODO: file stuff
// FIL* pika_platform_fopen(const char* filename, const char* modes);
// int pika_platform_fclose(FIL* stream);
// u32 pika_platform_fwrite(const void* ptr,
//                             u32 size,
//                             u32 n,
//                             FIL* stream);
// u32 pika_platform_fread(void* ptr, u32 size, u32 n, FIL* stream);
// int pika_platform_fseek(FIL* stream, long offset, int whence);
// long pika_platform_ftell(FIL* stream);

void pika_platform_error_handle(void) {
    EPRINTF("PIKAPYTHON: Error has occured!");
}

void pika_platform_panic_handle(void) {
    EPRINTF("PIKAPYTHON: We can figure out panics later!");
}

s64 pika_platform_getTick(void) {
    return -1; // TODO: does the hekate bdk have a system tick?
}


void pika_platform_sleep_ms(u32 ms) {
    msleep(ms);
}

void pika_platform_sleep_s(u32 s) {
    msleep(s/1000);
}

// I'm only pretty sure I don't need these.
// void pika_hook_instruct(void);
// bool pika_hook_arg_cache_filter(void* self);

void* pika_user_malloc(u32 size) {
    return malloc(size);
}

void pika_user_free(void* ptr, u32 size) {
    free(ptr);
}

// I'm only pretty sure I don't need this.
// u8 pika_is_locked_pikaMemory(void);


// TODO: What the hell am I going to to with this?
// typedef struct pika_platform_timer {
//     void* platform_data;
// } pika_platform_timer_t;
//
// void pika_platform_timer_init(pika_platform_timer_t* timer);
// void pika_platform_timer_cutdown(pika_platform_timer_t* timer,
//                                  unsigned int timeout);
// char pika_platform_timer_is_expired(pika_platform_timer_t* timer);
// int pika_platform_timer_remain(pika_platform_timer_t* timer);
// unsigned long pika_platform_timer_now(void);
// void pika_platform_timer_usleep(unsigned long usec);

void pika_platform_reboot(void) {
    power_set_state(POWER_OFF_REBOOT);
}
