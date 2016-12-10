#ifndef __LINUX_COMPILER_H
#error "Please don't include <linux/compiler-gcc6.h> directly, include <linux/compiler.h> instead."
#endif

#define __used				__attribute__((__used__))
#define __must_check			__attribute__((warn_unused_result))
#define __compiler_offsetof(a, b)	__builtin_offsetof(a, b)

#define __cold			__attribute__((__cold__))

#define __UNIQUE_ID(prefix) __PASTE(__PASTE(__UNIQUE_ID_, prefix), __COUNTER__)

#ifndef __CHECKER__
# define __compiletime_warning(message) __attribute__((warning(message)))
# define __compiletime_error(message) __attribute__((error(message)))
#endif /* __CHECKER__ */

#define unreachable() __builtin_unreachable()

#define __noclone	__attribute__((__noclone__))

#define __visible __attribute__((externally_visible))

#define asm_volatile_goto(x...)	do { asm goto(x); asm (""); } while (0)

#ifdef CONFIG_ARCH_USE_BUILTIN_BSWAP
#define __HAVE_BUILTIN_BSWAP32__
#define __HAVE_BUILTIN_BSWAP64__
#define __HAVE_BUILTIN_BSWAP16__
#endif /* CONFIG_ARCH_USE_BUILTIN_BSWAP */

#define KASAN_ABI_VERSION 4
