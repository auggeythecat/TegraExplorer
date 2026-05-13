#include <bdk.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pika_config.h"
#include "../../gfx/gfx.h"

void pika_platform_enable_irq_handle(void) {
    return;
}

void pika_platform_disable_irq_handle(void) {
    return;
}
// TODO: figure out what to do here?


void pika_platform_printf(char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    gfx_vprintf(fmt, args);
    va_end(args);
}
int pika_platform_sprintf(char* buff, char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    s_vprintf(buff, fmt, args);
    va_end(args);
    return 0;
}
int pika_platform_vsprintf(char* buff, char* fmt, va_list args) {
    s_vprintf(buff, fmt, args);
    return 0;
}
int pika_platform_vsnprintf(char* buff,
                            u32 size,
                            const char* fmt,
                            va_list args) {
    gfx_printf("I don't want vsnprintf used!");
    return 0;
}
int pika_platform_snprintf(char* buff, u32 size, const char* fmt, ...) {
    gfx_printf("I don't want snprintf used!");
    return 0;
}

char* pika_platform_strdup(const char* src) {
    char* dst = (char*)malloc(strlen(src) + 1);
    if (dst) {
        strcpy(dst, src);
    }
    return dst;
}
u32 pika_platform_tick_from_millisecond(u32 ms) {
    return 0;
}

/* libc config */
void* pika_platform_malloc(u32 size) {
    return malloc(size);
}

void* pika_platform_realloc(void* ptr, u32 size) {
    return realloc(ptr, size);
}

void* pika_platform_calloc(u32 num, u32 size) {
    return calloc(num, size);
}

void pika_platform_free(void* ptr) {
    return free(ptr);
}

// TODO: I'd rather not if I can avoid it...
void* pika_platform_memset(void* mem, int ch, u32 size) {
    return memset(mem, ch, size);
}

void* pika_platform_memcpy(void* dir, const void* src, u32 size) {
    return memcpy(dir, src, size);
}

int pika_platform_memcmp(const void* s1, const void* s2, u32 n) {
    return memcmp(s1, s2, n);
}


void* pika_platform_memmove(void* s1, void* s2, u32 n) {
    return memmove(s1, s2, n);
}

/* pika memory pool config */
void pika_platform_wait(void) {
    bpmp_halt();
}

char pika_platform_getchar(void) {
    return '\0';
}

int pika_platform_putchar(char ch) {
    return 0;
}

// TODO: rewrite all file stuff and stop being stupid: https://github.com/pikasTech/PikaPython/blob/9bca87e6b44095d19e0140c6e7142a931ae0382e/package/pika_fatfs/pika_fatfs.C
FIL* pika_platform_fopen(const char* filename, const char* modes) {
    FIL* file = NULL;
    f_open(file, filename, *modes);
    return file;
}

int pika_platform_fclose(FIL* stream) {
    return f_close(stream);
}

u32 pika_platform_fwrite(const void* ptr, u32 size, u32 n, FIL* stream) {
    return f_write(stream, ptr, size, &n);
}

u32 pika_platform_fread(void* ptr, u32 size, u32 n, FIL* stream) {
    return f_read(stream, ptr, size, &n);
}

int pika_platform_fseek(FIL* stream, long offset, int whence) {
    return f_lseek(stream, offset);
}

long pika_platform_ftell(FIL* stream) {
    return f_tell(stream);
}

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
void pika_hook_instruct(void) {
    return;
}

bool pika_hook_arg_cache_filter(void* self) {
    return true;
}

void* pika_user_malloc(u32 size) {
    return pika_platform_malloc(size);
}

void pika_user_free(void* ptr, u32 size) {
    pika_platform_free(ptr);
}

// I'm only pretty sure I don't need this.
u8 pika_is_locked_pikaMemory(void) {
    return 0;
}


// TODO: What the hell am I going to to with this?
typedef struct pika_platform_timer {
    void* platform_data;
} pika_platform_timer_t;

void pika_platform_timer_init(pika_platform_timer_t* timer) {
    return;
}

void pika_platform_timer_cutdown(pika_platform_timer_t* timer, unsigned int timeout) {
    return;
}

char pika_platform_timer_is_expired(pika_platform_timer_t* timer) {
    return '\0';
}

int pika_platform_timer_remain(pika_platform_timer_t* timer) {
    return 0;
}

unsigned long pika_platform_timer_now(void) {
    return 0;
}

void pika_platform_timer_usleep(unsigned long usec) {
    return;
}

void pika_platform_reboot(void) {
    power_set_state(POWER_OFF_REBOOT);
}
