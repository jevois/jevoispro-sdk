/*
 * Copyright (C) 2017 Wesion Co,. Ltd
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <command.h>
#include <asm/errno.h>
#include <malloc.h>
#include <linux/ctype.h>    /* isalpha, isdigit */
#include <linux/sizes.h>

#ifdef CONFIG_SYS_HUSH_PARSER
#include <cli_hush.h>
#endif

#define BOOTINI_MAGIC	"KHADAS-UBOOT-CONFIG"
#define SZ_BOOTINI		SZ_64K

/* Nothing to proceed with zero size string or comment.
 *
 * FIXME: Do we really need to strip the line start with '#' or ';',
 *        since any U-boot command does not start with punctuation character.
 */
static int valid_command(const char* p)
{
	char *q;

	for (q = (char*)p; *q; q++) {
		if (isblank(*q)) continue;
		if (isalnum(*q)) return 1;
		if (ispunct(*q))
			return (*q != '#') && (*q != ';');
	}

	return !(p == q);
}

/* Read boot.ini from FAT partition
 */
static char* read_cfgload(void)
{
	char cmd[128];
	unsigned long filesize;
	char *p;
	const char *partition_pairs[][2] = {
	    //partition		 boot.ini
		{"0:1", 		"/boot.ini"},
		{"0:1",			"/boot/boot.ini"},
		{"1:1", 		"/boot.ini"},
		{"1:1",			"/boot/boot.ini"},
		{"1:5", 		"/boot/boot.ini"}
	};
	int partition_array_len = sizeof(partition_pairs) / sizeof(partition_pairs[0]);
	int i = 0;

	p = (char *)simple_strtoul(getenv("loadaddr"), NULL, 16);
	if (NULL == p)
		p = (char *)CONFIG_SYS_LOAD_ADDR;

	for (i=0; i<partition_array_len; i++) {
		setenv("filesize", "0");
		printf("cfgload: reading %s from mmc %s ...\n", partition_pairs[i][1], partition_pairs[i][0]);
		sprintf(cmd, "load mmc %s 0x%p %s", partition_pairs[i][0], (void *)p, partition_pairs[i][1]);
		run_command(cmd, 0);

		filesize = getenv_ulong("filesize", 16, 0);
		if (0 != filesize)
			break;

		printf("cfgload: no %s or empty file on mmc %s\n", partition_pairs[i][1], partition_pairs[i][0]);
	}

	if (i == partition_array_len) {
		printf("cfgload: failed to read boot.ini on all partitions!\n");

		return NULL;
	}

	if (filesize > SZ_BOOTINI) {
		printf("cfgload: 'boot.ini' exceeds %d, size=%ld\n",
				SZ_BOOTINI, filesize);
		return NULL;
    }

	/* Terminate the read buffer with '\0' to be treated as string */
	*(char *)(p + filesize) = '\0';

	/* Scan MAGIC string, readed boot.ini must start with exact magic string.
	 * Otherwise, we will not proceed at all.
	 */
	while (*p) {
		char *s = strsep(&p, "\n");
		if (!valid_command(s))
			continue;

		/* MAGIC string is discovered, return the buffer address of next to
		 * proceed the commands.
		 */
		if (!strncmp(s, BOOTINI_MAGIC, sizeof(BOOTINI_MAGIC)))
			return memcpy(malloc(filesize), p, filesize);
	}

	printf("cfgload: MAGIC NAME, %s, is not found!!\n", BOOTINI_MAGIC);

	return NULL;
}

static int do_load_cfgload(cmd_tbl_t *cmdtp, int flag, int argc,
		char *const argv[])
{
	char *p;
	char cmd[64];

	printf("cfgload: start ...\n");

	p = read_cfgload();
	if (NULL == p)
		return 0;

	printf("cfgload: applying boot.ini...\n");

	sprintf(cmd, "script %p %ld 0", p, strlen(p));
	run_command(cmd, 0);

	return 0;
}

U_BOOT_CMD(
		cfgload,		1,		0,		do_load_cfgload,
		"read 'boot.ini' from FAT/EXT4 partition",
		"\n"
		"    - read boot.ini from the boot partition"
);
