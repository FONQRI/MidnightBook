#include <QDebug>
#include <QDir>
#include <QFile>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "src/cpp/models/LibraryManager.h"
#include "src/cpp/storage/FileManager.h"

int main(int argc, char *argv[])
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QCoreApplication::setApplicationName("Midnight Book");

	QGuiApplication app(argc, argv);

	QQmlApplicationEngine engine;

	const QUrl url(QStringLiteral("qrc:/src/qml/main.qml"));
	QObject::connect(&engine, &QQmlApplicationEngine::objectCreated, &app,
					 [url](QObject *obj, const QUrl &objUrl) {
						 if (!obj && url == objUrl)
							 QCoreApplication::exit(-1);
					 },
					 Qt::QueuedConnection);

	qmlRegisterType<LibraryManager>("fonqri.book.models", 1, 0, "BooksModel");
	engine.load(url);

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
