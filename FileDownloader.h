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
	void downloadFile(QString path, QString url);

private:
	QNetworkAccessManager *manager;
	QNetworkReply *reply;
	QFile *file;

private slots:
	void onDownloadProgress(qint64, qint64);
	void onFinished(QNetworkReply *);
	void onReadyRead();
	void onReplyFinished();
};

#endif // QDOWNLOADER_H
