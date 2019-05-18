#include "FileDownloader.h"

#include <QDir>

FileDownloader::FileDownloader(QObject *parent)
	: QObject(parent)
{
	manager = new QNetworkAccessManager;
}

FileDownloader::~FileDownloader()
{
	manager->deleteLater();
}

void FileDownloader::downloadFile(QString path, QString url)
{
	QString filePath = url;
	QString saveFilePath;
	QStringList filePathList = filePath.split('/');
	QString fileName = filePathList.at(filePathList.count() - 1);
	QDir dir;
	dir.mkpath(path);
	saveFilePath = QString(path + "/" + fileName);

	QNetworkRequest request;
	request.setUrl(QUrl(url));
	reply = manager->get(request);

	file = new QFile;
	file->setFileName(saveFilePath);
	file->open(QIODevice::WriteOnly);

	connect(reply, SIGNAL(downloadProgress(qint64, qint64)), this,
			SLOT(onDownloadProgress(qint64, qint64)));
	connect(manager, SIGNAL(finished(QNetworkReply *)), this, SLOT(onFinished(QNetworkReply *)));
	connect(reply, SIGNAL(readyRead()), this, SLOT(onReadyRead()));
	connect(reply, SIGNAL(finished()), this, SLOT(onReplyFinished()));
}

void FileDownloader::onDownloadProgress(qint64 bytesRead, qint64 bytesTotal)
{
	qDebug(QString::number(bytesRead).toLatin1() + " - " + QString::number(bytesTotal).toLatin1());
}

void FileDownloader::onFinished(QNetworkReply *reply)
{
	switch (reply->error()) {
	case QNetworkReply::NoError: {
		qDebug("file is downloaded successfully.");
	} break;
	default: {
		qDebug(reply->errorString().toLatin1());
	};
	}

	if (file->isOpen()) {
		file->close();
		file->deleteLater();
	}
}

void FileDownloader::onReadyRead()
{
	file->write(reply->readAll());
}

void FileDownloader::onReplyFinished()
{
	if (file->isOpen()) {
		file->close();
		file->deleteLater();
	}
}
