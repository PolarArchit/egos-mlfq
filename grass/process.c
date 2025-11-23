/*
 * (C) 2025, Cornell University
 * All rights reserved.
 *
 * Description: helper functions for process management
 */
static unsigned long long MLFQ_last_reset_time = 0;
#include "process.h"
#include <stddef.h>
#define MLFQ_NLEVELS          5
#define MLFQ_RESET_PERIOD     10000000    /* 10 seconds */
#define MLFQ_LEVEL_RUNTIME(x) (x + 1) * 100000 /* e.g., 100ms for level 0 */
extern struct process proc_set[MAX_NPROCESS + 1];

static void proc_set_status(int pid, enum proc_status status) {
    for (uint i = 0; i < MAX_NPROCESS; i++)
        if (proc_set[i].pid == pid) proc_set[i].status = status;
}

void proc_set_ready(int pid) { proc_set_status(pid, PROC_READY); }
void proc_set_running(int pid) { proc_set_status(pid, PROC_RUNNING); }
void proc_set_runnable(int pid) { proc_set_status(pid, PROC_RUNNABLE); }
void proc_set_pending(int pid) { proc_set_status(pid, PROC_PENDING_SYSCALL); }

int proc_alloc() {
    static uint curr_pid = 0;
    for (uint i = 0; i < MAX_NPROCESS; i++)
        if (proc_set[i].status == PROC_UNUSED) {
            proc_set[i].pid    = ++curr_pid;
            proc_set[i].status = PROC_LOADING;
            /* Student's code goes here (Preemptive Scheduler | System Call). */
            struct process* p = &proc_set[i];
            p->mlfq_level = 0;
            p->mlfq_remaining_time = MLFQ_LEVEL_RUNTIME(0);
            p->create_time = mtime_get();
            p->response_time = 0;
            p->cpu_time = 0;
            p->last_run_start_time = 0;
            p->interrupt_count = 0;
            /* Initialization of lifecycle statistics, MLFQ or process sleep. */

            /* Student's code ends here. */
            return curr_pid;
        }

    FATAL("proc_alloc: reach the limit of %d processes", MAX_NPROCESS);
}
static void stats(struct process *p) {
    if (p == NULL) return;

    ulonglong turnaround_time = mtime_get() - p->create_time;

    INFO("[PID %d Stats]", p->pid);

    INFO("  Turnaround Time: %d us", (int)turnaround_time);
    INFO("  CPU Time: %d us", (int)p->cpu_time);
    INFO("  Response Time: %d us", (int)p->response_time);
    INFO("  Interrupts: %d", p->interrupt_count);
    INFO("  Final MLFQ Level: %d", p->mlfq_level);
}
// void proc_free(int pid) {
//     /* Student's code goes here (Preemptive Scheduler). */

//     /* Print the lifecycle statistics of the terminated process or processes. */
//     if (pid != GPID_ALL) {
//         earth->mmu_free(pid);
//         proc_set_status(pid, PROC_UNUSED);
//     } else {
//         /* Free all user processes. */
//         for (uint i = 0; i < MAX_NPROCESS; i++)
//             if (proc_set[i].pid >= GPID_USER_START &&
//                 proc_set[i].status != PROC_UNUSED) {
//                 earth->mmu_free(proc_set[i].pid);
//                 proc_set[i].status = PROC_UNUSED;
//             }
//     }
//     /* Student's code ends here. */
// }
void proc_free(int pid) {
    /* Student's code goes here (Preemptive Scheduler). */

    /* Print the lifecycle statistics of the terminated process or processes. */
    if (pid != GPID_ALL) {
        struct process *p = NULL;
        for (int i = 0; i < MAX_NPROCESS; i++) {
            if (proc_set[i].pid == pid && proc_set[i].status != PROC_UNUSED) {
                p = &proc_set[i];
                break;
            }
        }
        if (p != NULL) {
            stats(p);
        }

        earth->mmu_free(pid);
        proc_set_status(pid, PROC_UNUSED);
    } else {
        /* Free all user processes. */
        for (uint i = 0; i < MAX_NPROCESS; i++) {
            if (proc_set[i].pid >= GPID_USER_START &&
                proc_set[i].status != PROC_UNUSED) {

                stats(&proc_set[i]);

                earth->mmu_free(proc_set[i].pid);
                proc_set[i].status = PROC_UNUSED;
            }
        }
    }
    /* Student's code ends here. */
}

void mlfq_update_level(struct process* p, ulonglong runtime) {
    /* Student's code goes here (Preemptive Scheduler). */
    p->mlfq_remaining_time -= runtime;
    if (p->mlfq_remaining_time <= 0) {
        if (p->mlfq_level < MLFQ_NLEVELS - 1) {
            p->mlfq_level++;
        }
        p->mlfq_remaining_time = MLFQ_LEVEL_RUNTIME(p->mlfq_level);
        printf("Updated to PID: %d LEVEL: %d: \n ",p->pid,p->mlfq_level);
    }

    /* Update the MLFQ-related fields in struct process* p after this
     * process has run on the CPU for another runtime microseconds. */

    /* Student's code ends here. */

}

void mlfq_reset_level() {
    /* Student's code goes here (Preemptive Scheduler). */
if (!earth->tty_input_empty()) {
        /* Reset the level of GPID_SHELL if there is pending keyboard input. */

        struct process *shell = NULL;
        for (int i = 0; i < MAX_NPROCESS; i++) {
            if (proc_set[i].pid == GPID_SHELL) {
                shell = &proc_set[i];
                break;
            }
        }
        if (shell != NULL && shell->status != PROC_UNUSED) {
            shell->mlfq_level = 0;
            shell->mlfq_remaining_time = MLFQ_LEVEL_RUNTIME(0);
        }
    }
    /* Reset the level of all processes every MLFQ_RESET_PERIOD microseconds. */
    ulonglong now = mtime_get();
    if (now - MLFQ_last_reset_time >= MLFQ_RESET_PERIOD) {
        for (int i = 0; i < MAX_NPROCESS; i++) {
            if (proc_set[i].status != PROC_UNUSED) {
                proc_set[i].mlfq_level = 0;
                proc_set[i].mlfq_remaining_time = MLFQ_LEVEL_RUNTIME(0);
                printf("RESET!!! now %d\n",(int)(now));
                MLFQ_last_reset_time = now;


            }
        }
    }
    /* Student's code ends here. */

}

void proc_sleep(int pid, uint usec) {
    /* Student's code goes here (System Call & Protection). */

    /* Update the sleep-related fields in the struct process for process pid. */

    /* Student's code ends here. */
}

void proc_coresinfo() {
    extern uint core_to_proc_idx[NCORES + 1];
    /* Student's code goes here (Multicore & Locks). */

    /* Print out the pid of the process running on each CPU core. */

    /* Student's code ends here. */
}