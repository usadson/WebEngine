/**
 * Copyright (C) 2020 Tristan
 *
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
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
