/* Target value mapping utilities needed by the simulator and gdb.  */
/* This file is machine generated by gentmap.c.  */

#include <errno.h>
#include <fcntl.h>
#include "ansidecl.h"
#include "gdb/callback.h"
#include "targ-vals.h"

/* syscall mapping table */
CB_TARGET_DEFS_MAP cb_init_syscall_map[] = {
#ifdef CB_SYS_argc
  { CB_SYS_argc, TARGET_SYS_argc },
#endif
#ifdef CB_SYS_argn
  { CB_SYS_argn, TARGET_SYS_argn },
#endif
#ifdef CB_SYS_argnlen
  { CB_SYS_argnlen, TARGET_SYS_argnlen },
#endif
#ifdef CB_SYS_argv
  { CB_SYS_argv, TARGET_SYS_argv },
#endif
#ifdef CB_SYS_argvlen
  { CB_SYS_argvlen, TARGET_SYS_argvlen },
#endif
#ifdef CB_SYS_chdir
  { CB_SYS_chdir, TARGET_SYS_chdir },
#endif
#ifdef CB_SYS_chmod
  { CB_SYS_chmod, TARGET_SYS_chmod },
#endif
#ifdef CB_SYS_close
  { CB_SYS_close, TARGET_SYS_close },
#endif
#ifdef CB_SYS_exit
  { CB_SYS_exit, TARGET_SYS_exit },
#endif
#ifdef CB_SYS_fstat
  { CB_SYS_fstat, TARGET_SYS_fstat },
#endif
#ifdef CB_SYS_getpid
  { CB_SYS_getpid, TARGET_SYS_getpid },
#endif
#ifdef CB_SYS_gettimeofday
  { CB_SYS_gettimeofday, TARGET_SYS_gettimeofday },
#endif
#ifdef CB_SYS_kill
  { CB_SYS_kill, TARGET_SYS_kill },
#endif
#ifdef CB_SYS_link
  { CB_SYS_link, TARGET_SYS_link },
#endif
#ifdef CB_SYS_lseek
  { CB_SYS_lseek, TARGET_SYS_lseek },
#endif
#ifdef CB_SYS_open
  { CB_SYS_open, TARGET_SYS_open },
#endif
#ifdef CB_SYS_read
  { CB_SYS_read, TARGET_SYS_read },
#endif
#ifdef CB_SYS_reconfig
  { CB_SYS_reconfig, TARGET_SYS_reconfig },
#endif
#ifdef CB_SYS_stat
  { CB_SYS_stat, TARGET_SYS_stat },
#endif
#ifdef CB_SYS_time
  { CB_SYS_time, TARGET_SYS_time },
#endif
#ifdef CB_SYS_times
  { CB_SYS_times, TARGET_SYS_times },
#endif
#ifdef CB_SYS_unlink
  { CB_SYS_unlink, TARGET_SYS_unlink },
#endif
#ifdef CB_SYS_utime
  { CB_SYS_utime, TARGET_SYS_utime },
#endif
#ifdef CB_SYS_write
  { CB_SYS_write, TARGET_SYS_write },
#endif
  { -1, -1 }
};

/* errno mapping table */
CB_TARGET_DEFS_MAP cb_init_errno_map[] = {
#ifdef E2BIG
  { E2BIG, TARGET_E2BIG },
#endif
#ifdef EACCES
  { EACCES, TARGET_EACCES },
#endif
#ifdef EADDRINUSE
  { EADDRINUSE, TARGET_EADDRINUSE },
#endif
#ifdef EADDRNOTAVAIL
  { EADDRNOTAVAIL, TARGET_EADDRNOTAVAIL },
#endif
#ifdef EAFNOSUPPORT
  { EAFNOSUPPORT, TARGET_EAFNOSUPPORT },
#endif
#ifdef EAGAIN
  { EAGAIN, TARGET_EAGAIN },
#endif
#ifdef EALREADY
  { EALREADY, TARGET_EALREADY },
#endif
#ifdef EBADF
  { EBADF, TARGET_EBADF },
#endif
#ifdef EBADMSG
  { EBADMSG, TARGET_EBADMSG },
#endif
#ifdef EBUSY
  { EBUSY, TARGET_EBUSY },
#endif
#ifdef ECANCELED
  { ECANCELED, TARGET_ECANCELED },
#endif
#ifdef ECHILD
  { ECHILD, TARGET_ECHILD },
#endif
#ifdef ECONNABORTED
  { ECONNABORTED, TARGET_ECONNABORTED },
#endif
#ifdef ECONNREFUSED
  { ECONNREFUSED, TARGET_ECONNREFUSED },
#endif
#ifdef ECONNRESET
  { ECONNRESET, TARGET_ECONNRESET },
#endif
#ifdef EDEADLK
  { EDEADLK, TARGET_EDEADLK },
#endif
#ifdef EDESTADDRREQ
  { EDESTADDRREQ, TARGET_EDESTADDRREQ },
#endif
#ifdef EDOM
  { EDOM, TARGET_EDOM },
#endif
#ifdef EDQUOT
  { EDQUOT, TARGET_EDQUOT },
#endif
#ifdef EEXIST
  { EEXIST, TARGET_EEXIST },
#endif
#ifdef EFAULT
  { EFAULT, TARGET_EFAULT },
#endif
#ifdef EFBIG
  { EFBIG, TARGET_EFBIG },
#endif
#ifdef EFTYPE
  { EFTYPE, TARGET_EFTYPE },
#endif
#ifdef EHOSTDOWN
  { EHOSTDOWN, TARGET_EHOSTDOWN },
#endif
#ifdef EHOSTUNREACH
  { EHOSTUNREACH, TARGET_EHOSTUNREACH },
#endif
#ifdef EIDRM
  { EIDRM, TARGET_EIDRM },
#endif
#ifdef EILSEQ
  { EILSEQ, TARGET_EILSEQ },
#endif
#ifdef EINPROGRESS
  { EINPROGRESS, TARGET_EINPROGRESS },
#endif
#ifdef EINTR
  { EINTR, TARGET_EINTR },
#endif
#ifdef EINVAL
  { EINVAL, TARGET_EINVAL },
#endif
#ifdef EIO
  { EIO, TARGET_EIO },
#endif
#ifdef EISCONN
  { EISCONN, TARGET_EISCONN },
#endif
#ifdef EISDIR
  { EISDIR, TARGET_EISDIR },
#endif
#ifdef ELOOP
  { ELOOP, TARGET_ELOOP },
#endif
#ifdef EMFILE
  { EMFILE, TARGET_EMFILE },
#endif
#ifdef EMLINK
  { EMLINK, TARGET_EMLINK },
#endif
#ifdef EMSGSIZE
  { EMSGSIZE, TARGET_EMSGSIZE },
#endif
#ifdef EMULTIHOP
  { EMULTIHOP, TARGET_EMULTIHOP },
#endif
#ifdef ENAMETOOLONG
  { ENAMETOOLONG, TARGET_ENAMETOOLONG },
#endif
#ifdef ENETDOWN
  { ENETDOWN, TARGET_ENETDOWN },
#endif
#ifdef ENETRESET
  { ENETRESET, TARGET_ENETRESET },
#endif
#ifdef ENETUNREACH
  { ENETUNREACH, TARGET_ENETUNREACH },
#endif
#ifdef ENFILE
  { ENFILE, TARGET_ENFILE },
#endif
#ifdef ENOBUFS
  { ENOBUFS, TARGET_ENOBUFS },
#endif
#ifdef ENODATA
  { ENODATA, TARGET_ENODATA },
#endif
#ifdef ENODEV
  { ENODEV, TARGET_ENODEV },
#endif
#ifdef ENOENT
  { ENOENT, TARGET_ENOENT },
#endif
#ifdef ENOEXEC
  { ENOEXEC, TARGET_ENOEXEC },
#endif
#ifdef ENOLCK
  { ENOLCK, TARGET_ENOLCK },
#endif
#ifdef ENOLINK
  { ENOLINK, TARGET_ENOLINK },
#endif
#ifdef ENOMEM
  { ENOMEM, TARGET_ENOMEM },
#endif
#ifdef ENOMSG
  { ENOMSG, TARGET_ENOMSG },
#endif
#ifdef ENOPROTOOPT
  { ENOPROTOOPT, TARGET_ENOPROTOOPT },
#endif
#ifdef ENOSPC
  { ENOSPC, TARGET_ENOSPC },
#endif
#ifdef ENOSR
  { ENOSR, TARGET_ENOSR },
#endif
#ifdef ENOSTR
  { ENOSTR, TARGET_ENOSTR },
#endif
#ifdef ENOSYS
  { ENOSYS, TARGET_ENOSYS },
#endif
#ifdef ENOTCONN
  { ENOTCONN, TARGET_ENOTCONN },
#endif
#ifdef ENOTDIR
  { ENOTDIR, TARGET_ENOTDIR },
#endif
#ifdef ENOTEMPTY
  { ENOTEMPTY, TARGET_ENOTEMPTY },
#endif
#ifdef ENOTRECOVERABLE
  { ENOTRECOVERABLE, TARGET_ENOTRECOVERABLE },
#endif
#ifdef ENOTSOCK
  { ENOTSOCK, TARGET_ENOTSOCK },
#endif
#ifdef ENOTSUP
  { ENOTSUP, TARGET_ENOTSUP },
#endif
#ifdef ENOTTY
  { ENOTTY, TARGET_ENOTTY },
#endif
#ifdef ENXIO
  { ENXIO, TARGET_ENXIO },
#endif
#ifdef EOPNOTSUPP
  { EOPNOTSUPP, TARGET_EOPNOTSUPP },
#endif
#ifdef EOVERFLOW
  { EOVERFLOW, TARGET_EOVERFLOW },
#endif
#ifdef EOWNERDEAD
  { EOWNERDEAD, TARGET_EOWNERDEAD },
#endif
#ifdef EPERM
  { EPERM, TARGET_EPERM },
#endif
#ifdef EPFNOSUPPORT
  { EPFNOSUPPORT, TARGET_EPFNOSUPPORT },
#endif
#ifdef EPIPE
  { EPIPE, TARGET_EPIPE },
#endif
#ifdef EPROTO
  { EPROTO, TARGET_EPROTO },
#endif
#ifdef EPROTONOSUPPORT
  { EPROTONOSUPPORT, TARGET_EPROTONOSUPPORT },
#endif
#ifdef EPROTOTYPE
  { EPROTOTYPE, TARGET_EPROTOTYPE },
#endif
#ifdef ERANGE
  { ERANGE, TARGET_ERANGE },
#endif
#ifdef EROFS
  { EROFS, TARGET_EROFS },
#endif
#ifdef ESPIPE
  { ESPIPE, TARGET_ESPIPE },
#endif
#ifdef ESRCH
  { ESRCH, TARGET_ESRCH },
#endif
#ifdef ESTALE
  { ESTALE, TARGET_ESTALE },
#endif
#ifdef ETIME
  { ETIME, TARGET_ETIME },
#endif
#ifdef ETIMEDOUT
  { ETIMEDOUT, TARGET_ETIMEDOUT },
#endif
#ifdef ETOOMANYREFS
  { ETOOMANYREFS, TARGET_ETOOMANYREFS },
#endif
#ifdef ETXTBSY
  { ETXTBSY, TARGET_ETXTBSY },
#endif
#ifdef EWOULDBLOCK
  { EWOULDBLOCK, TARGET_EWOULDBLOCK },
#endif
#ifdef EXDEV
  { EXDEV, TARGET_EXDEV },
#endif
  { 0, 0 }
};

/* open flags mapping table */
CB_TARGET_DEFS_MAP cb_init_open_map[] = {
#ifdef O_ACCMODE
  { O_ACCMODE, TARGET_O_ACCMODE },
#endif
#ifdef O_APPEND
  { O_APPEND, TARGET_O_APPEND },
#endif
#ifdef O_CREAT
  { O_CREAT, TARGET_O_CREAT },
#endif
#ifdef O_EXCL
  { O_EXCL, TARGET_O_EXCL },
#endif
#ifdef O_NOCTTY
  { O_NOCTTY, TARGET_O_NOCTTY },
#endif
#ifdef O_NONBLOCK
  { O_NONBLOCK, TARGET_O_NONBLOCK },
#endif
#ifdef O_RDONLY
  { O_RDONLY, TARGET_O_RDONLY },
#endif
#ifdef O_RDWR
  { O_RDWR, TARGET_O_RDWR },
#endif
#ifdef O_SYNC
  { O_SYNC, TARGET_O_SYNC },
#endif
#ifdef O_TRUNC
  { O_TRUNC, TARGET_O_TRUNC },
#endif
#ifdef O_WRONLY
  { O_WRONLY, TARGET_O_WRONLY },
#endif
  { -1, -1 }
};

