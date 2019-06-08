#ifndef QDOWNLOADER_H
#define QDOWNLOADER_H

#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>
#include <QStringList>

class FileDownloader : public QObject
{
	Q_OBJECT
public:
	explicit FileDownloader(QObject *parent = nullptr);
	virtual ~FileDownloader();
	void downloadFile(QString path, QUrl url);

private:
	QNetworkAccessManager *manager;
	QNetworkReply *reply;
	QFile *file;

private slots:
	void onFinished(QNetworkReply *);
	void onReadyRead();
	void onReplyFinished();

signals:
	void started(QString fileName);
	void onDownloadProgress(qint64, qint64);
	void finished(QString fileName);
};

#endif // QDOWNLOADER_H
