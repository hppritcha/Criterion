#ifndef POSIX_COMPAT_H_
# define POSIX_COMPAT_H_

#if defined(_WIN32) && !defined(__CYGWIN__)
# define VANILLA_WIN32
#endif

# if !defined(_POSIX_SOURCE)
#  define _POSIX_SOURCE 1
#  define TMP_POSIX
# endif
# include <stdio.h>
# ifdef TMP_POSIX
#  undef _POSIX_SOURCE
#  undef TMP_POSIX
# endif

# ifdef VANILLA_WIN32
#  define WEXITSTATUS(Status) (((Status) & 0xFF00) >> 8)
#  define WTERMSIG(Status)    ((Status) & 0x7F)
#  define WIFEXITED(Status)   (WTERMSIG(Status) == 0)
#  define WIFSIGNALED(Status) (((signed char) (WTERMSIG(Status) + 1) >> 1) > 0)
# else
#  include <sys/wait.h>
# endif

#include <criterion/types.h>

struct proc_handle;
typedef struct proc_handle s_proc_handle;

struct pipe_handle;
typedef struct pipe_handle s_pipe_handle;

struct worker_context {
    struct criterion_test *test;
    struct criterion_suite *suite;
    f_worker_func func;
    struct pipe_handle *pipe;
};

extern struct worker_context g_worker_context;

int resume_child(void);

s_pipe_handle *stdpipe();
FILE *pipe_in(s_pipe_handle *p);
FILE *pipe_out(s_pipe_handle *p);

s_proc_handle *fork_process();
void wait_process(s_proc_handle *handle, int *status);

s_proc_handle *get_current_process();
bool is_current_process(s_proc_handle *proc);

#endif /* !POSIX_COMPAT_H_ */
