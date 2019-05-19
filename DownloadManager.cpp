#include "DownloadManager.h"

#include "FileDownloader.h"

DownloadManager::DownloadManager(QObject *parent)
	: QObject(parent)
	, fileDownloader(new FileDownloader(this))
{
	connect(fileDownloader, &FileDownloader::started, this, &DownloadManager::downloadStarted);
	connect(fileDownloader, &FileDownloader::finished, this, &DownloadManager::downloadFinished);
	connect(fileDownloader, &FileDownloader::onDownloadProgress, this,
			&DownloadManager::onDownloadProgress);
}

void DownloadManager::appendFile(QString path, QUrl url)
{
	std::lock_guard<std::mutex> lg(fileMutex);
	downloadList.insert({url, path});
}

void DownloadManager::start()
{
	if (!isRunning) {
		std::lock_guard<std::mutex> lg(fileMutex);
		std::pair<QUrl, QString> file = *(downloadList.begin());
		downloadList.erase(file.first);
		downloadStarted(file.first.path());
		fileDownloader->downloadFile(file.second, file.first);
	}
}
