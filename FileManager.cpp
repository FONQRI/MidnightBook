#include "FileManager.h"

#include <QCryptographicHash>
#include <QDebug>
#include <QDir>
#include <QFile>

FileManager::FileManager()
{

	// FIXME delete test
	//	QFile test(
	//		"/home/fonqri/Projects/build/build-MidnightBook-Desktop_Qt_5_12_3_GCC_64bit-Debug/FONQRI/"
	//		"book2.FONQRI.timestamp/seasons/3/River Crossing Forest Bridge Mist Android Wallpaper.jpg");
	//	if (test.open(QIODevice::ReadOnly)) {
	//		qDebug() << "MD5 : " << QString(getMd5(test.readAll()));
	//	}
	//	test.close();

	// *******************************
	connect(this, &FileManager::raiseError, [](const QString &error) { qDebug() << error; });

	connect(&downloadManager, &DownloadManager::onDownloadProgress,
			[](qint64 current, qint64 total) { qDebug() << current << "/" << total; });

	connect(&downloadManager, &DownloadManager::seasonsDownloadFinished, this,
			&FileManager::saveOrUpdateBookSeasons);

	connect(&downloadManager, &DownloadManager::booksFetchFinished,
			[this](std::vector<Book> bookList) {
				for (const auto &book : bookList) {
					saveOrUpdateBook(book);
				};
			});

	connect(&downloadManager, &DownloadManager::bookFetchFinished, this,
			&FileManager::saveOrUpdateBook);

	downloadManager.testDownload();
}

void FileManager::createAuthorFolders(const QString &authorId)
{
	QDir dir(QDir::currentPath() + "/" + authorId);

	if (!dir.exists()) {
		dir.mkpath(QDir::currentPath() + "/" + authorId);
	}
}

void FileManager::saveOrUpdateBook(const Book &book)
{
	//	createBookFolders(book);
	QDir bookDir(QDir::currentPath() + "/" + book.authorId() + "/" + book.id());
	QFile bookFile(QDir::currentPath() + "/" + book.authorId() + "/" + book.id() + "/book.json");
	qDebug() << __FUNCTION__ << " " << __LINE__ << " " << bookDir.path();
	qDebug() << __FUNCTION__ << " " << __LINE__ << " " << book.id();

	// save book
	if (!bookFile.exists()) {
		if (bookDir.mkpath(bookDir.path())) {
			saveBook(book, bookDir);
		} else {
			raiseError("Can not create book directory!");
			return;
		}
	} else if (bookNeedsUpdate(book, bookDir)) {
		updateBook(book, bookDir);
	}
}

void FileManager::saveOrUpdateBookSeasons(const Book &book)
{
	QDir dir(QDir::currentPath() + "/" + book.authorId() + "/" + book.id());
	auto tempBook = readBook(dir);
	if (tempBook.has_value()) {
		// FIXME check seasons for update and compelete
		for (const auto &season : book.seasons()) {
			saveOrUpdateSeason(season, dir);
		}
	}
}

void FileManager::saveBook(const Book &book, const QDir &bookDir)
{
	QFile bookFile(bookDir.path() + "/book.json");
	if (bookFile.open(QIODevice::WriteOnly)) {
		QTextStream out(&bookFile);
		out << book.json();
		if (getImageMd5(bookDir.path()) != book.coverImage_md5()) {
			downloadManager.appendFile(bookDir.path(), book.coverImage());
		}

		bookFile.close();
	} else {
		raiseError(bookFile.errorString());
	}

	for (const auto &season : book.seasons()) {
		saveOrUpdateSeason(season, bookDir);
	}

	downloadManager.start();
}

void FileManager::saveOrUpdateSeason(const Season &season, const QDir &bookDir)
{
	// chech if season folder exists
	QString seasonPath = bookDir.path() + "/seasons/" + QString::number(season.id());
	qDebug() << seasonPath;
	QDir dir(seasonPath);
	if (!dir.exists()) {
		dir.mkpath(seasonPath);
		saveSeason(season, seasonPath);
	} else {
		// TODO P[0] update
		updateSeason(season, seasonPath);
	}
	downloadManager.start();
}

void FileManager::saveSeason(const Season &season, const QString &seasonPath)
{

	QFile seasonFile(seasonPath + "/season.json");

	if (getImageMd5(seasonPath) != season.coverImage_md5()) {
		downloadManager.appendFile(seasonPath, season.coverImage());
	}

	if (seasonFile.open(QIODevice::WriteOnly)) {
		QTextStream out(&seasonFile);
		out << season.json();
		seasonFile.close();

	} else {

		raiseError(seasonFile.errorString());
	}
}

void FileManager::updateSeason(const Season &season, const QDir &seasonPath)
{}

void FileManager::checkSeasonsUpdate(const Book &book, const QString &bookPath)
{}

bool FileManager::bookNeedsUpdate(const Book &book, const QDir &bookDir)
{
	// check  if version is old
	std::optional<Book> currentBook = readBook(bookDir);
	if (currentBook.has_value()) {
		return currentBook.value() < book;
	}
	return true;
}

void FileManager::updateBook(const Book &book, const QDir &bookDir)
{
	// TODO implement update book and add md5 for images
	// check if image needs update
	QString bookPath = bookDir.path() + "/book.json";
	std::optional<Book> oldBook = readBook(bookPath);

	QFile oldBookFile(bookPath);
	if (oldBookFile.open(QIODevice::WriteOnly) && !oldBookFile.remove()) {
		saveBook(book, bookDir);
	} else {
		raiseError(oldBookFile.errorString());
	}
}

void FileManager::createBookFolders(const Book &book)
{
	QDir dir(QDir::currentPath() + "/" + book.authorId());

	if (!dir.exists()) {
		dir.mkpath(QDir::currentPath() + "/" + book.authorId() + "/" + book.id());
		dir.mkpath(QDir::currentPath() + "/" + book.authorId() + "/" + book.id() + "/seasons");
	}
}

std::optional<Book> FileManager::readBook(const QDir &bookDir)
{
	std::optional<Book> book;
	QFile bookFile(bookDir.path() + "/book.json");
	if (bookFile.open(QIODevice::ReadOnly)) {
		book = Book();
		book->setInfoObject(QJsonDocument::fromJson(bookFile.readAll()).object());
	} else {
		raiseError(bookFile.errorString());
	}
	return book;
}

QByteArray FileManager::getMd5(const QByteArray &data)
{
	return QCryptographicHash::hash(data, QCryptographicHash::Md5).toHex();
}

QString FileManager::getImageMd5(QString path)
{
	QDir dir(path);
	QStringList filters;
	filters << "*.png"
			<< "*.jpg"
			<< "*.bmp";
	QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
	if (!fileInfoList.isEmpty()) {
		fileInfoList.first().fileName();
		QFile image(fileInfoList.first().fileName());
		image.open(QIODevice::ReadOnly);
		return getMd5(image.readAll());
	}
	return "";
}

bool FileManager::removeImageMd5(QString path)
{
	QDir dir(path);
	QStringList filters;
	filters << "*.png"
			<< "*.jpg"
			<< "*.bmp";
	QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
	if (!fileInfoList.isEmpty()) {
		fileInfoList.first().fileName();
		QFile image(fileInfoList.first().fileName());
		image.open(QIODevice::WriteOnly);
		return image.remove();
	}
	return false;
}
