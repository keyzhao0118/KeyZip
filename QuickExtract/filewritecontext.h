#pragma once

#include <QMutex>
#include <QWaitCondition>
#include <QByteArray>

struct FileWriteContext
{
	QMutex mutex;
	QWaitCondition dataAvailable;
	QWaitCondition spaceAvailable;

	QByteArray buffer;
	bool noMoreData = false;

	static constexpr int MAX_BUFFER_SIZE = 1024 * 1024;
};
