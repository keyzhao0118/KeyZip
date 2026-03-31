#include "outstreamwrapper.h"
#include "filewritetask.h"

OutStreamWrapper::OutStreamWrapper(const QString& filePath, QThreadPool* pool)
	: m_ctx(std::make_shared<FileWriteContext>())
{
	auto* task = new FileWriteTask(filePath, m_ctx);
	pool->start(task);
}

OutStreamWrapper::~OutStreamWrapper()
{
	QMutexLocker lock(&m_ctx->mutex);
	m_ctx->noMoreData = true;
	m_ctx->dataAvailable.wakeOne();
}

STDMETHODIMP OutStreamWrapper::Write(const void* data, UInt32 size, UInt32* processedSize)
{
	QMutexLocker lock(&m_ctx->mutex);

	while (m_ctx->buffer.size() + static_cast<int>(size) > FileWriteContext::MAX_BUFFER_SIZE)
		m_ctx->spaceAvailable.wait(&m_ctx->mutex);

	m_ctx->buffer.append(static_cast<const char*>(data), size);
	m_ctx->dataAvailable.wakeOne();

	if (processedSize)
		*processedSize = size;

	return S_OK;
}
