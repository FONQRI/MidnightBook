#include "FileDownloader.h"

#include <QDir>

FileDownloader::FileDownloader(QObject *parent)
	: QObject(parent)
{
	manager = new QNetworkAccessManager;
	connect(manager, &QNetworkAccessManager::finished, this, &FileDownloader::onFinished);
}

FileDownloader::~FileDownloader()
{
	manager->deleteLater();
}

void FileDownloader::downloadFile(QString path, QUrl url)
{
	QString saveFilePath;
	QFileInfo fileInfo(url.path());

	QDir dir;
	dir.mkpath(path);
	saveFilePath = QString(path + "/" + fileInfo.fileName());

	QNetworkRequest request;
	request.setUrl(QUrl(url));

	reply = manager->get(request);
	connect(reply, &QNetworkReply::downloadProgress, this, &FileDownloader::onDownloadProgress);
	connect(reply, &QNetworkReply::readyRead, this, &FileDownloader::onReadyRead);
	connect(reply, &QNetworkReply::finished, this, &FileDownloader::onReplyFinished);
	emit started(fileInfo.fileName());

	file = new QFile;
	file->setFileName(saveFilePath);
	file->open(QIODevice::WriteOnly);
}

//void FileDownloader::onDownloadProgress(qint64 bytesRead, qint64 bytesTotal)
//{
//	qDebug() << QString::number(bytesRead).toLatin1() + " - "
//					+ QString::number(bytesTotal).toLatin1();
//}

void FileDownloader::onFinished(QNetworkReply *reply)
{
	switch (reply->error()) {
	case QNetworkReply::NoError: {
		qDebug("file is downloaded successfully.");
	} break;
	default: {
		qDebug() << reply->errorString().toLatin1();
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
	QFileInfo fileInfo(file->fileName());
	emit finished(fileInfo.fileName());
}
