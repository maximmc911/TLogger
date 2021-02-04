#include "logger.h"

namespace logs {
	const log_flags::_end     end;
	const log_flags::_console no_console;

	TLog::TLog(QString logname)
		: m_console_blocked(false), logName(logname), savefile(NULL)
	{}

	TLog::~TLog()
	{}

	TLog fullLog("fullLog");
	TLog debugLog("debugLog");
	TLog errorLog("errorLog");
}

