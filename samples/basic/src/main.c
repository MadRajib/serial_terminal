/*
 * Serial Terminal - Basic Sample
 * 
 * Copyright (c) 2024 MadRajib
 * SPDX-License-Identifier: MIT
 * 
 * This sample demonstrates basic usage of the serial_terminal library.
 */

#include <zephyr/kernel.h>
#include <zephyr/console/console.h>
#include <zephyr/sys/printk.h>
#include <zephyr/sys/reboot.h>
#include <string.h>

/* Define macros required by serial_prompt.h */
#define PRINT(str)  printk("%s", str)
#define READ()      console_getchar()

/* Include the header-only library */
#include <serial_prompt.h>

/*
 * Command Handlers
 */

static int cmd_hello(int argc, char **argv)
{
	printk("\n╔════════════════════════════════════╗\n");
	printk("║  Hello from Serial Terminal!      ║\n");
	printk("╚════════════════════════════════════╝\n");
	printk("Board: %s\n", CONFIG_BOARD);
	printk("Zephyr: %s\n\n", KERNEL_VERSION_STRING);
	return 0;
}

static int cmd_echo(int argc, char **argv)
{
	if (argc < 2) {
		printk("Usage: echo <text>\n");
		return -1;
	}

	for (int i = 1; i < argc; i++) {
		printk("%s", argv[i]);
		if (i < argc - 1) {
			printk(" ");
		}
	}
	printk("\n");
	return 0;
}

static int cmd_add(int argc, char **argv)
{
	if (argc < 3) {
		printk("Usage: add <num1> <num2> [num3] ...\n");
		printk("Example: add 10 20 30\n");
		return -1;
	}

	int sum = 0;
	printk("Adding: ");
	for (int i = 1; i < argc; i++) {
		int num = atoi(argv[i]);
		sum += num;
		printk("%d ", num);
	}
	printk("\nSum = %d\n", sum);
	return 0;
}

static int cmd_mult(int argc, char **argv)
{
	if (argc != 3) {
		printk("Usage: mult <num1> <num2>\n");
		printk("Example: mult 12 5\n");
		return -1;
	}

	int a = atoi(argv[1]);
	int b = atoi(argv[2]);
	printk("%d × %d = %d\n", a, b, a * b);
	return 0;
}

static int cmd_uptime(int argc, char **argv)
{
	int64_t uptime_ms = k_uptime_get();
	int seconds = uptime_ms / 1000;
	int minutes = seconds / 60;
	int hours = minutes / 60;

	printk("Uptime: %02d:%02d:%02d (%lld ms)\n",
	       hours, minutes % 60, seconds % 60, uptime_ms);
	return 0;
}

static int cmd_sysinfo(int argc, char **argv)
{
	printk("\n=== System Information ===\n");
	printk("Board:          %s\n", CONFIG_BOARD);
	printk("Zephyr Version: %s\n", KERNEL_VERSION_STRING);
	printk("Uptime:         %lld ms\n", k_uptime_get());
	
	#ifdef CONFIG_HEAP_MEM_POOL_SIZE
	printk("Heap Size:      %d bytes\n", CONFIG_HEAP_MEM_POOL_SIZE);
	#endif
	
	printk("Main Stack:     %d bytes\n", CONFIG_MAIN_STACK_SIZE);
	printk("\n");
	return 0;
}

static int cmd_clear(int argc, char **argv)
{
	/* ANSI escape sequence to clear screen */
	printk("\033[2J\033[H");
	return 0;
}

static int cmd_reset(int argc, char **argv)
{
	printk("\n⚠️  System will reset in 2 seconds...\n");
	k_sleep(K_MSEC(2000));
	sys_reboot(SYS_REBOOT_COLD);
	return 0; /* Never reached */
}

/*
 * Command Registration
 */
COMMANDS(
	{"hello",   "Print hello message", cmd_hello},
	{"echo",    "Echo arguments: echo <text>", cmd_echo},
	{"add",     "Add numbers: add <n1> <n2> [n3]...", cmd_add},
	{"mult",    "Multiply: mult <n1> <n2>", cmd_mult},
	{"uptime",  "Show system uptime", cmd_uptime},
	{"sysinfo", "Display system information", cmd_sysinfo},
	{"clear",   "Clear the screen", cmd_clear},
	{"reset",   "Reset the system", cmd_reset}
);

/*
 * Main Application
 */
int main(void)
{
	/* Initialize console */
	console_init();
	
	/* Small delay to let console stabilize */
	k_sleep(K_MSEC(100));
	
	/* Print banner */
	printk("\n");
	printk("╔════════════════════════════════════════════════╗\n");
	printk("║     Serial Terminal for Zephyr RTOS           ║\n");
	printk("║     Board: %-35s ║\n", CONFIG_BOARD);
	printk("║     Version: 1.0.0                            ║\n");
	printk("╚════════════════════════════════════════════════╝\n");
	printk("\n");
	
	/* Main loop - periodically call serial_run() */
	while (1) {
		serial_run();
		k_msleep(10);  /* Small delay to prevent CPU hogging */
	}
	
	return 0;
}