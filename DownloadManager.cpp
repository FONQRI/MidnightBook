#include "DownloadManager.h"

#include "Author.h"
#include "BookDownloader.h"
#include "FileDownloader.h"

DownloadManager::DownloadManager(QObject *parent)
	: QObject(parent)
	, fileDownloader(new FileDownloader(this))
	, bookDownloader(new BookDownloader(this))

{
	connect(fileDownloader, &FileDownloader::started, this, &DownloadManager::downloadStarted);
	connect(fileDownloader, &FileDownloader::finished, this, &DownloadManager::downloadFileFinished);
	connect(fileDownloader, &FileDownloader::finished, this, &DownloadManager::downloadFinished);
	connect(fileDownloader, &FileDownloader::onDownloadProgress, this,
			&DownloadManager::onDownloadProgress);

	connect(bookDownloader, &BookDownloader::seasonsDownloadFinished, this,
			&DownloadManager::seasonsDownloadFinished);
	connect(bookDownloader, &BookDownloader::booksFetchFinished, this,
			&DownloadManager::booksFetchFinished);

	connect(bookDownloader, &BookDownloader::booksFetchFinished,
			[](std::vector<Book> bookList) { qDebug() << "Book size " << bookList.size(); });
}

void DownloadManager::appendFile(QString path, QUrl url)
{
	std::lock_guard<std::mutex> lg(fileMutex);
	downloadList.insert({url, path});
}

void DownloadManager::testDownload()
{
	Author fonqriAuthor;
	fonqriAuthor.id = "FONQRI";
	fonqriAuthor.name = "بهنام صباغی";

	bookDownloader->setServerUrl("http://localhost:8000");
	bookDownloader->fetchBooks(fonqriAuthor.id);

	bookDownloader->fetchBook("book1.FONQRI.timestamp");
	bookDownloader->downloadSeasons("book1.FONQRI.timestamp", {1, 2});
}

std::vector<Book> DownloadManager::fetchAuthorBooks(QString id)
{}

void DownloadManager::downloadFinished(QString fileName)
{
	continueDownload();
}

void DownloadManager::start()
{
	if (!isRunning) {
		continueDownload();
	}
}

void DownloadManager::continueDownload()
{
	std::lock_guard<std::mutex> lg(fileMutex);
	if (downloadList.size() > 0) {
		isRunning = true;
		std::pair<QUrl, QString> file = *(downloadList.begin());
		downloadList.erase(file.first);
		downloadStarted(file.first.path());
		fileDownloader->downloadFile(file.second, file.first);
		qDebug() << "list size " << downloadList.size();
	} else {
		isRunning = false;
		emit finished();
	}
}
