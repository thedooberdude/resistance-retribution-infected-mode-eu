#include <pspkernel.h>
#include <pspthreadman_kernel.h>
#include <pspmodulemgr_kernel.h>
#include <psputilsforkernel.h>
#include <string.h>

PSP_MODULE_INFO("RRInfectedEU", PSP_MODULE_KERNEL, 1, 0);
PSP_MAIN_THREAD_ATTR(0);

#define GAME_MODULE_NAME "Resistance"
#define TARGET_OFFSET 0x0008CCB4u
#define STARTUP_DELAY_US 8000000
#define MAX_MODULES 128

/*
 * Resistance: Retribution EU (UCES-01184)
 * Exact instructions observed at text_addr + 0x8CCB4.
 * The patch is refused unless all four words match.
 */
#define EXPECTED_WORD_0 0x3C0408DEu
#define EXPECTED_WORD_1 0x8C842838u
#define EXPECTED_WORD_2 0x30840001u
#define EXPECTED_WORD_3 0x10800003u

/* jr ra; li v0,1 */
#define PATCH_WORD_0 0x03E00008u
#define PATCH_WORD_1 0x34020001u

static int find_module_by_name(const char *name, SceKernelModuleInfo *out_info)
{
    SceUID ids[MAX_MODULES];
    int count = 0;
    int result;
    int i;

    result = sceKernelGetModuleIdList(ids, sizeof(ids), &count);
    if (result < 0)
        return result;

    if (count > MAX_MODULES)
        count = MAX_MODULES;

    for (i = 0; i < count; ++i) {
        SceKernelModuleInfo info;
        memset(&info, 0, sizeof(info));
        info.size = sizeof(info);

        if (sceKernelQueryModuleInfo(ids[i], &info) < 0)
            continue;

        if (strcmp(info.name, name) == 0) {
            *out_info = info;
            return 0;
        }
    }

    return -1;
}

static int patch_thread(SceSize args, void *argp)
{
    SceKernelModuleInfo game;
    volatile unsigned int *site;
    unsigned int seg_start;
    unsigned int seg_size;
    unsigned int seg_end;
    unsigned int target;

    (void)args;
    (void)argp;

    sceKernelDelayThread(STARTUP_DELAY_US);

    memset(&game, 0, sizeof(game));
    game.size = sizeof(game);

    if (find_module_by_name(GAME_MODULE_NAME, &game) < 0)
        return 0;

    seg_start = (unsigned int)game.segmentaddr[0];
    seg_size = (unsigned int)game.segmentsize[0];
    seg_end = seg_start + seg_size;
    target = (unsigned int)game.text_addr + TARGET_OFFSET;

    if (seg_size == 0u || seg_end < seg_start ||
        target < seg_start || target + 16u < target || target + 16u > seg_end)
        return 0;

    site = (volatile unsigned int *)target;

    if (site[0] != EXPECTED_WORD_0 ||
        site[1] != EXPECTED_WORD_1 ||
        site[2] != EXPECTED_WORD_2 ||
        site[3] != EXPECTED_WORD_3)
        return 0;

    site[0] = PATCH_WORD_0;
    site[1] = PATCH_WORD_1;

    sceKernelDcacheWritebackInvalidateRange((void *)site, 8);
    sceKernelIcacheInvalidateRange((void *)site, 8);

    return 0;
}

int module_start(SceSize args, void *argp)
{
    SceUID thid = sceKernelCreateThread(
        "RRInfectedEUPatch",
        patch_thread,
        0x18,
        0x2000,
        PSP_THREAD_ATTR_VFPU,
        NULL);

    if (thid >= 0)
        sceKernelStartThread(thid, args, argp);

    return 0;
}

int module_stop(SceSize args, void *argp)
{
    (void)args;
    (void)argp;
    return 0;
}
