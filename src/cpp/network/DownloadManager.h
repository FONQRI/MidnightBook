#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <mutex>

#include <QObject>
#include <QUrl>

#include "src/cpp/models/Book.h"

class FileDownloader;
class BookDownloader;

class DownloadManager : public QObject
{
	Q_OBJECT
public:
	explicit DownloadManager(QObject *parent = nullptr);

	void appendFile(QString path, QUrl url);

	void testDownload();
	void start();

signals:
	void downloadStarted(QString fileName);
	void onDownloadProgress(qint64, qint64);
	void downloadFileFinished(QString fileName);
	void finished();
	void seasonsDownloadFinished(Book testBook);
	void booksFetchFinished(std::vector<Book> bookList);
	void bookFetchFinished(Book book);

public slots:

	void downloadSeasons(QString BookId, std::vector<int> seasonIds);
	std::vector<Book> fetchAuthorBooks(QString id);

private:
	void downloadFinished(QString fileName);

private:
	std::map<QUrl, QString> downloadList;
	void continueDownload();
	FileDownloader *fileDownloader;
	std::mutex fileMutex;
	bool isRunning{false};
	BookDownloader *bookDownloader;
};

#endif // DOWNLOADMANAGER_H
