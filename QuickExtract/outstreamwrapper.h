#pragma once

#include <Common/MyCom.h>
#include <7zip/IStream.h>
#include "filewritecontext.h"
#include <QThreadPool>
#include <memory>

class OutStreamWrapper :
	public ISequentialOutStream,
	public CMyUnknownImp 
{
	Z7_COM_UNKNOWN_IMP_1(ISequentialOutStream)

public:
	OutStreamWrapper(const QString& filePath, QThreadPool* pool);
	~OutStreamWrapper();

	STDMETHOD(Write)(const void* data, UInt32 size, UInt32* processedSize) override;

private:
	std::shared_ptr<FileWriteContext> m_ctx;
};
