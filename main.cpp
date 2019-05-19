#include <QDebug>
#include <QDir>
#include <QFile>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "BookDownloader.h"
#include "FileDownloader.h"
#include "FileManager.h"

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

	QGuiApplication app(argc, argv);

	BookDownloader booksDownloader;
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

	//	FileDownloader fd;
	//	QObject::connect(&fd, &FileDownloader::started,
	//					 [](QString fileName) { qDebug() << "Started : " + fileName; });
	//	QObject::connect(&fd, &FileDownloader::finished,
	//					 [](QString fileName) { qDebug() << "finished " + fileName; });
	//	fd.downloadFile(QDir::currentPath() + "/test",
	//					QUrl("https://cdn11.bigcommerce.com/s-oqm1pc/images/stencil/1280x1280/products/"
	//						 "1605/5510/peanut_cactus__37076.1522879200.jpg"));

	return app.exec();
}
