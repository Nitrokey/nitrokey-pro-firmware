
//#define DEFINE_CUSTOM_STACK_CHK_GUARD
#ifdef DEFINE_CUSTOM_STACK_CHK_GUARD

typedef long long int uintptr_t;
#define DISABLE_STACK_CHK_GUARD_RUNTIME_CONFIG
#define STACK_CHK_GUARD_VALUE   0xbeeffeed
#ifdef DISABLE_STACK_CHK_GUARD_RUNTIME_CONFIG
uintptr_t __stack_chk_guard = STACK_CHK_GUARD_VALUE;
#else
uintptr_t __stack_chk_guard = 0;

static void __attribute__((constructor,no_stack_protector)) __construct_stk_chk_guard(void);
__attribute__((weak)) uintptr_t __stack_chk_guard_init(void);
void __stack_chk_fail(void);


static void __attribute__((constructor,no_stack_protector)) __construct_stk_chk_guard(void)
{
    if(__stack_chk_guard == 0)
    {
        __stack_chk_guard = __stack_chk_guard_init();
    }
}

__attribute__((weak)) uintptr_t __stack_chk_guard_init(void)
{
    return STACK_CHK_GUARD_VALUE;
}
#endif // ! DISABLE_STACK_CHK_GUARD_RUNTIME_CONFIG


//uintptr_t __stack_chk_guard_init()
//{
//    return 0xbeeffeeda5a5a5a5;
//}

#endif