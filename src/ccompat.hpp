#pragma once
/* C Compatability */

#include <vector>

namespace CCompat {

	extern const std::vector<const char *> errnoNames;

	const char *GetErrnoName(int error = 0);
	void CloseStandardIO();
}
