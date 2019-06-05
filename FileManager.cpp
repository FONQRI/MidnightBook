#include "FileManager.h"

#include <QCryptographicHash>
#include <QDebug>
#include <QDir>
#include <QFile>

FileManager::FileManager()
{

	//	 FIXME delete test
	//	QFile test(
	//		"/home/fonqri/Projects/build/build-MidnightBook-Desktop_Qt_5_12_3_GCC_64bit-Debug/FONQRI/"
	//		"book1.FONQRI.timestamp/Love Cocktails Black Strawberries Android Wallpaper.jpg");
	//	if (test.open(QIODevice::ReadOnly)) {
	//		qDebug() << "MD5 FIRST : " << QString(getMd5(test.readAll()));
	//	}
	//	test.close();

	// *******************************
	connect(this, &FileManager::raiseError, [](const QString &error) { qDebug() << error; });

	connect(&downloadManager, &DownloadManager::onDownloadProgress,
			[](qint64 current, qint64 total) { qDebug() << current << "/" << total; });

	connect(&downloadManager, &DownloadManager::seasonsDownloadFinished, this,
			&FileManager::saveOrUpdateBookSeasons);

	connect(&downloadManager, &DownloadManager::downloadFileFinished, this,
			&FileManager::updatePhotoMd5);

	connect(&downloadManager, &DownloadManager::booksFetchFinished,
			[this](std::vector<Book> bookList) {
				for (const auto &book : bookList) {
					saveOrUpdateBook(book);
				};
			});

	connect(&downloadManager, &DownloadManager::bookFetchFinished, this,
			&FileManager::bookFetchFinished);

	downloadManager.testDownload();
}

void FileManager::saveOrUpdateBook(Book book)
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
	} else {
		updateBook(book, bookDir);
	}
}

void FileManager::saveOrUpdateBookSeasons(Book book)
{
	// TODO check seasons version instde of books
	QDir dir(QDir::currentPath() + "/" + book.authorId() + "/" + book.id() + "/seasons");

	qDebug() << __FUNCTION__ << __LINE__;

	for (auto season : book.seasons()) {
		QString seasonPath{dir.path() + QString::number(season.id()) + "/season.json"};
		qDebug() << __FUNCTION__ << __LINE__
				 << QDir::currentPath() + "/" + book.authorId() + "/" + book.id();
		QFile seasonFile(seasonPath);
		if (seasonFile.exists()) {
			auto oldSeason{readSeason(seasonPath)};
			if (oldSeason->version() != season.version()) {
				seasonFile.remove();
				saveSeason(season, QDir::currentPath() + "/" + book.authorId() + "/" + book.id());
			}
		} else {
			saveSeason(season, QDir::currentPath() + "/" + book.authorId() + "/" + book.id());
		}
	}

	downloadManager.start();
}

void FileManager::saveBook(Book book, const QDir &bookDir)
{
	QFile bookFile(bookDir.path() + "/book.json");
	if (bookFile.open(QIODevice::WriteOnly)) {
		QTextStream out(&bookFile);
		out << book.json();
		bookFile.close();

		if (getImageMd5(bookDir.path()) != book.coverImage_md5()) {
			downloadManager.appendFile(bookDir.path(), book.coverImage());
		}

	} else {
		raiseError(bookFile.errorString());
	}

	//	for (const auto &season : book.seasons()) {
	//		saveSeason(season, bookDir.path());
	//	}

	downloadManager.start();
}

void FileManager::saveSeason(const Season &season, const QString &bookPath)
{

	QString seasonPath = bookPath + "/seasons/" + QString::number(season.id());
	QDir dir(seasonPath);
	dir.mkpath(seasonPath);

	QFile seasonFile(seasonPath + "/season.json");

	qDebug() << __FUNCTION__ << __LINE__ << " " << getImageMd5(seasonPath);
	qDebug() << __FUNCTION__ << __LINE__ << " " << season.coverImage_md5();
	if (getImageMd5(seasonPath) != season.coverImage_md5()) {
		removeImageFromDir(seasonPath);
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

void FileManager::updateSeason(const Season &season, const QString &bookPath)
{

	QFile seasonFile(bookPath + "/seasons/" + QString::number(season.id()) + "/season.json");

	if (seasonFile.open(QIODevice::WriteOnly)) {
		seasonFile.remove();
		seasonFile.close();
		saveSeason(season, bookPath);

	} else {

		raiseError(seasonFile.errorString());
	}
}

void FileManager::checkSeasonsUpdate(const Book &book, const QString &bookPath)
{}

void FileManager::bookFetchFinished(Book book)
{
	saveOrUpdateBook(book);
	std::vector<int> seasons;
	for (auto season : book.seasons()) {
		seasons.push_back(season.id());
	}
	qDebug() << __FUNCTION__ << __LINE__ << seasons;
	qDebug() << __FUNCTION__ << __LINE__ << book.id();
	downloadManager.downloadSeasons(book.id(), seasons);
}

bool FileManager::bookNeedsUpdate(const Book &book, const QDir &bookDir)
{
	// check  if version is old
	std::optional<Book> currentBook = readBook(bookDir);
	if (currentBook.has_value()) {
		return currentBook.value() < book;
	}
	return true;
}

void FileManager::updateBook(Book book, const QDir &bookDir)
{

	std::optional<Book> oldBook = readBook(bookDir);
	if (oldBook.has_value()) {
		if (oldBook->version() == book.version()) {
			if (getImageMd5(bookDir.path()) != book.coverImage_md5()) {
				removeImageFromDir(bookDir.path());
				book.setCoverImage_md5(oldBook->coverImage_md5());
				downloadManager.appendFile(bookDir.path(), book.coverImage());
			}
		}
		QFile oldBookFile(bookDir.path() + "/book.json");
		if (oldBookFile.open(QIODevice::WriteOnly) && oldBookFile.remove()) {
			saveBook(book, bookDir);
		} else {
			raiseError(oldBookFile.errorString());
		}
		return;
	} else {
		saveBook(book, bookDir);
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

std::optional<Book> FileManager::readBook(const QString &bookPath)
{
	std::optional<Book> book;
	QFile bookFile(bookPath + "/book.json");
	if (bookFile.open(QIODevice::ReadOnly)) {
		book = Book();
		book->setInfoObject(QJsonDocument::fromJson(bookFile.readAll()).object());

		//		QFileInfo fileInfo(bookPath);
		QDir seasonsDir(bookPath + "/seasons");
		//		bookDir.setFilter(QDir::Dirs);
		if (seasonsDir.exists()) {
			auto dirList = seasonsDir.entryInfoList(QDir::Dirs);
			//TODO use readSeason
			QJsonArray seasonsArray;
			for (auto info : dirList) {
				QFile seasonFile(info.filePath() + "/season.json");
				if (seasonFile.open(QIODevice::ReadOnly)) {
					seasonsArray.append(QJsonDocument::fromJson(seasonFile.readAll()).object());
				}
			}
			if (!seasonsArray.isEmpty()) {
				QJsonObject seasonsObj{{"seasons", seasonsArray}};
				book->setSeasonObject(seasonsObj);
			}
		}
	} else {
		raiseError(bookFile.errorString());
	}
	return book;
}

std::optional<Book> FileManager::readBook(const QDir &bookDir)
{
	return readBook(bookDir.path());
}

std::optional<Season> FileManager::readSeason(const QDir &seasonDir)
{
	return readSeason(seasonDir.path() + "/season.json");
}

std::optional<Season> FileManager::readSeason(const QString &seasonPath)
{
	std::optional<Season> season;

	QFile seasonFile(seasonPath);
	if (seasonFile.open(QIODevice::ReadOnly)) {
		season = Season(QJsonDocument::fromJson(seasonFile.readAll()).object());
	}
	return season;
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
			<< "*.jpeg"
			<< "*.bmp";
	QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);
	if (!fileInfoList.isEmpty()) {
		fileInfoList.first().fileName();
		qDebug() << "PATH " << fileInfoList.first().filePath();
		qDebug() << "NAME " << fileInfoList.first().fileName();
		QFile image(fileInfoList.first().filePath());
		image.open(QIODevice::ReadOnly);
		return getMd5(image.readAll());
	}
	return "";
}

bool FileManager::removeImageFromDir(QString path)
{
	qDebug() << __FUNCTION__ << __LINE__;
	QDir dir(path);
	QStringList filters;
	filters << "*.png"
			<< "*.jpg"
			<< "*.jpeg"
			<< "*.bmp";
	QFileInfoList fileInfoList = dir.entryInfoList(filters, QDir::Files | QDir::NoDotAndDotDot);

	qDebug() << __FUNCTION__ << __LINE__ << "size " << fileInfoList.size();
	if (!fileInfoList.isEmpty()) {
		bool temp = false;
		for (auto info : fileInfoList) {
			qDebug() << __FUNCTION__ << __LINE__ << info.filePath();
			QFile image(info.filePath());
			//		image.open(QIODevice::WriteOnly);
			bool temp = image.remove();
			qDebug() << __FUNCTION__ << __LINE__ << temp;
			qDebug() << __FUNCTION__ << __LINE__ << image.errorString();
		}

		return temp;
	}
	return false;
}

void FileManager::updatePhotoMd5(const QString &filename)
{
	qDebug() << __FUNCTION__ << __LINE__ << " " << filename;
	QFileInfo fileInfo(filename);
	QStringList pathSpilit = filename.split("/");

	qDebug() << __FUNCTION__ << __LINE__ << " " << pathSpilit.at(pathSpilit.size() - 2);
	QRegExp re("\\d*"); // a digit (\d), zero or more times (*)
	if (re.exactMatch(pathSpilit.at(pathSpilit.size() - 2))) {
		qDebug() << __FUNCTION__ << __LINE__ << " in season ";
		int seasonId = pathSpilit.at(pathSpilit.size() - 2).toInt();
		qDebug() << __FUNCTION__ << __LINE__ << " in season ";
		//		pathSpilit.erase(pathSpilit.end());
		qDebug() << __FUNCTION__ << __LINE__ << " in season ";
		QDir seasonsDir(fileInfo.path());
		qDebug() << __FUNCTION__ << __LINE__ << seasonsDir.path();
		seasonsDir.cd("../..");
		qDebug() << __FUNCTION__ << __LINE__ << seasonsDir.path();
		QString bookPath = seasonsDir.path();

		qDebug() << __FUNCTION__ << __LINE__ << bookPath;
		auto book = readBook(bookPath);
		qDebug() << __FUNCTION__ << __LINE__ << book->seasons().size();
		qDebug() << __FUNCTION__ << __LINE__ << "seasonId " << seasonId;
		auto season = book->seasons()[static_cast<size_t>(seasonId - 1)];
		qDebug() << __FUNCTION__ << __LINE__ << " " << seasonId;
		season.setCoverImage_md5(getImageMd5(fileInfo.path()));
		season.setShouldUpdate(false);
		qDebug() << __FUNCTION__ << __LINE__ << " " << book->seasons().size();
		updateSeason(season, bookPath);
	} else {
		auto book = readBook(fileInfo.path());

		qDebug() << fileInfo.path();
		book->setCoverImage_md5(getImageMd5(fileInfo.path()));
		updateBook(book.value(), fileInfo.path());
	}
}
