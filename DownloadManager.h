#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <mutex>

#include <QObject>
#include <QUrl>

class FileDownloader;

class DownloadManager : public QObject
{
	Q_OBJECT
public:
	explicit DownloadManager(QObject *parent = nullptr);

	void appendFile(QString path, QUrl url);

signals:
	void downloadStarted(QString fileName);
	void downloadFinished(QString fileName);
	void onDownloadProgress(qint64, qint64);

public slots:

private:
	std::map<QUrl, QString> downloadList;
	void start();
	FileDownloader *fileDownloader;
	std::mutex fileMutex;
	bool isRunning{false};
};

#endif // DOWNLOADMANAGER_H
