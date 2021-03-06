/*
 * The MIT License (MIT)
 *
 * Copyright © 2015 Franklin "Snaipe" Mathieu <http://snai.pe/>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include <stdlib.h>
#include <stdbool.h>
#include <csptr/smart_ptr.h>

#include "criterion/types.h"
#include "criterion/options.h"
#include "process.h"
#include "event.h"
#include "posix-compat.h"

struct process {
    s_proc_handle *proc;
    FILE *in;
};

static s_proc_handle *g_current_proc;

void set_runner_process(void) {
    g_current_proc = get_current_process();
}

void unset_runner_process(void) {
    sfree(g_current_proc);
}

bool is_runner(void) {
    return is_current_process(g_current_proc);
}

static void close_process(void *ptr, UNUSED void *meta) {
    struct process *proc = ptr;
    fclose(proc->in);
    sfree(proc->proc);
}

struct event *worker_read_event(struct process *proc) {
    return read_event(proc->in);
}

void run_worker(struct worker_context *ctx) {
    fclose(stdin);
    g_event_pipe = pipe_out(ctx->pipe);

    ctx->func(ctx->test, ctx->suite);
    fclose(g_event_pipe);

    fflush(NULL); // flush all opened streams
    if (criterion_options.no_early_exit)
        return;
    _Exit(0);
}

struct process *spawn_test_worker(struct criterion_test *test,
                                  struct criterion_suite *suite,
                                  f_worker_func func) {
    smart s_pipe_handle *pipe = stdpipe();
    if (pipe == NULL)
        abort();

    g_worker_context = (struct worker_context) {
        .test = test,
        .suite = suite,
        .func = func,
        .pipe = pipe
    };
    s_proc_handle *proc = fork_process();
    if (proc == (void *) -1) {
        return NULL;
    } else if (proc == NULL) {
        run_worker(&g_worker_context);
        return NULL;
    }

    return unique_ptr(struct process,
            .value = { .proc = proc, .in = pipe_in(pipe) },
            .dtor  = close_process);
}

struct process_status wait_proc(struct process *proc) {
    int status;
    wait_process(proc->proc, &status);

    if (WIFEXITED(status))
        return (struct process_status) {
            .kind = EXIT_STATUS,
            .status = WEXITSTATUS(status)
        };

    if (WIFSIGNALED(status))
        return (struct process_status) {
            .kind = SIGNAL,
            .status = WTERMSIG(status)
        };

    return (struct process_status) { .kind = STOPPED };
}
