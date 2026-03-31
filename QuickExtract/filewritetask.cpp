#include "filewritetask.h"
#include <QFile>

FileWriteTask::FileWriteTask(const QString& filePath, std::shared_ptr<FileWriteContext> ctx)
	: m_filePath(filePath)
	, m_ctx(std::move(ctx))
{
}

void FileWriteTask::run()
{
	QFile file(m_filePath);
	if (!file.open(QIODevice::WriteOnly))
		return;

	while (true)
	{
		QByteArray chunk;
		{
			QMutexLocker lock(&m_ctx->mutex);

			while (m_ctx->buffer.isEmpty() && !m_ctx->noMoreData)
				m_ctx->dataAvailable.wait(&m_ctx->mutex);

			if (m_ctx->buffer.isEmpty() && m_ctx->noMoreData)
				break;

			chunk.swap(m_ctx->buffer);
		}

		m_ctx->spaceAvailable.wakeOne();
		file.write(chunk);
	}

	file.close();
}
