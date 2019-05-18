#include <QDebug>
#include <QDir>
#include <QFile>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "DownloaderManager.h"
#include "FileManager.h"

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QGuiApplication app(argc, argv);

	DownloaderManager booksDownloader;
	booksDownloader.setServerUrl("http://localhost:8000");
	booksDownloader.fetchBooks();
	booksDownloader.fetchBook("book1.FONQRI.timestamp");
	booksDownloader.downloadSeasons("book1.FONQRI.timestamp", {1, 2});

	QQmlApplicationEngine engine;

	const QUrl url(QStringLiteral("qrc:/main.qml"));
	QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app,
					 [url](QObject *obj, const QUrl &objUrl) {
						 if (!obj && url == objUrl)
							 QCoreApplication::exit(-1);
					 },
					 Qt::QueuedConnection);

	engine.load(url);

	FileManager::createAuthorFolders(booksDownloader.authorId());

	return app.exec();
}
