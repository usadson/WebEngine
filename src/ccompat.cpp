/**
 * Copyright (C) 2020 Tristan <tristan@thewoosh.me>
 *
 * All Rights Reserved.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
 */

#include "ccompat.hpp"

#include <vector>

#include <cstdint>
#include <cstdio>
#include <errno.h>

namespace CCompat {

	const char *
	GetErrnoName(int error) {
		if (error <= 0 || (uint32_t)error > errnoNames.size())
			error = errno;
		return errnoNames[error];
	}

	/**
	 * When using valgrind with the --track-fds=yes option, it notices these
	 * FILE's are still open. They will be closed by the OS after Valgrind and
	 * this function is there to suppress that error message.
	 */
	void CloseStandardIO() {
		fclose(stderr);
		fclose(stdin);
		fclose(stdout);
	}

	const std::vector<const char *> errnoNames = {
		"CCOMPATNOERR",
		"EPERM",
		"ENOENT",
		"ESRCH",
		"EINTR",
		"EIO",
		"ENXIO",
		"E2BIG",
		"ENOEXEC",
		"EBADF",
		"ECHILD",
		"EAGAIN",
		"ENOMEM",
		"EACCES",
		"EFAULT",
		"ENOTBLK",
		"EBUSY",
		"EEXIST",
		"EXDEV",
		"ENODEV",
		"ENOTDIR",
		"EISDIR",
		"EINVAL",
		"ENFILE",
		"EMFILE",
		"ENOTTY",
		"ETXTBSY",
		"EFBIG",
		"ENOSPC",
		"ESPIPE",
		"EROFS",
		"EMLINK",
		"EPIPE",
		"EDOM",
		"ERANGE",
		"EDEADLK",
		"ENAMETOOLONG",
		"ENOLCK",
		"ENOSYS",
		"ENOTEMPTY",
		"ELOOP",
		"EWOULDBLOCK",
		"ENOMSG",
		"EIDRM",
		"ECHRNG",
		"EL2NSYNC",
		"EL3HLT",
		"EL3RST",
		"ELNRNG",
		"EUNATCH",
		"ENOCSI",
		"EL2HLT",
		"EBADE",
		"EBADR",
		"EXFULL",
		"ENOANO",
		"EBADRQC",
		"EBADSLT",
		"EDEADLOCK",
		"EBFONT",
		"ENOSTR",
		"ENODATA",
		"ETIME",
		"ENOSR",
		"ENONET",
		"ENOPKG",
		"EREMOTE",
		"ENOLINK",
		"EADV",
		"ESRMNT",
		"ECOMM",
		"EPROTO",
		"EMULTIHOP",
		"EDOTDOT",
		"EBADMSG",
		"EOVERFLOW",
		"ENOTUNIQ",
		"EBADFD",
		"EREMCHG",
		"ELIBACC",
		"ELIBBAD",
		"ELIBSCN",
		"ELIBMAX",
		"ELIBEXEC",
		"EILSEQ",
		"ERESTART",
		"ESTRPIPE",
		"EUSERS",
		"ENOTSOCK",
		"EDESTADDRREQ",
		"EMSGSIZE",
		"EPROTOTYPE",
		"ENOPROTOOPT",
		"EPROTONOSUPPORT",
		"ESOCKTNOSUPPORT",
		"EOPNOTSUPP",
		"EPFNOSUPPORT",
		"EAFNOSUPPORT",
		"EADDRINUSE",
		"EADDRNOTAVAIL",
		"ENETDOWN",
		"ENETUNREACH",
		"ENETRESET",
		"ECONNABORTED",
		"ECONNRESET",
		"ENOBUFS",
		"EISCONN",
		"ENOTCONN",
		"ESHUTDOWN",
		"ETOOMANYREFS",
		"ETIMEDOUT",
		"ECONNREFUSED",
		"EHOSTDOWN",
		"EHOSTUNREACH",
		"EALREADY",
		"EINPROGRESS",
		"ESTALE",
		"EUCLEAN",
		"ENOTNAM",
		"ENAVAIL",
		"EISNAM",
		"EREMOTEIO",
		"EDQUOT",
		"ENOMEDIUM",
		"EMEDIUMTYPE",
		"ECANCELED",
		"ENOKEY",
		"EKEYEXPIRED",
		"EKEYREVOKED",
		"EKEYREJECTED",
		"EOWNERDEAD",
		"ENOTRECOVERABLE",
		"ERFKILL",
		"EHWPOISON",
		"ENOTSUP"
	};

}
