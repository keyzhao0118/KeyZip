#pragma once

#include "filewritecontext.h"
#include <QRunnable>
#include <QString>
#include <memory>

class FileWriteTask : public QRunnable
{
public:
	FileWriteTask(const QString& filePath, std::shared_ptr<FileWriteContext> ctx);
	void run() override;

private:
	QString m_filePath;
	std::shared_ptr<FileWriteContext> m_ctx;
};
