#include "FileManager.h"

#include <QDebug>
#include <QDir>
#include <QFile>

FileManager::FileManager()
{
	connect(&downloadManager, &DownloadManager::seasonsDownloadFinished, this,
			&FileManager::saveBook);
	connect(&downloadManager, &DownloadManager::booksFetchFinished,
			[this](std::vector<Book> bookList) {
				for (const auto &book : bookList) {
					saveBook(book);
				};
			});

	downloadManager.testDownload();
}

void FileManager::createAuthorFolders(const QString &authorId)
{
	QDir dir(QDir::currentPath() + "/" + authorId);

	if (!dir.exists()) {
		dir.mkpath(QDir::currentPath() + "/" + authorId);
	}
}

void FileManager::saveBook(const Book &book)
{
	//	createBookFolders(book);
	QDir authorDir(QDir::currentPath() + "/" + book.authorId());
	QDir bookDir(authorDir.path() + "/" + book.id());
	QDir bookSeasonsDir(bookDir.path() + "/seasons");
	qDebug() << authorDir.path();
	qDebug() << bookDir.path();
	qDebug() << bookSeasonsDir.path();
	// save book
	if (!bookDir.exists()) {
		bookDir.mkpath(bookDir.path());

		QFile bookFile(bookDir.path() + "/book.json");
		if (bookFile.open(QIODevice::WriteOnly)) {
			QTextStream out(&bookFile);
			out << book.json();
			downloadManager.appendFile(bookDir.path(), book.coverImage());

			bookFile.close();
		} else {

			// TODO P[1] raise error
		}

		for (const auto &season : book.seasons()) {
			// chech if season folder exists
			QString seasonPath = bookSeasonsDir.path() + "/" + QString::number(season.id());
			QDir dir(seasonPath);
			if (!dir.exists()) {
				dir.mkpath(seasonPath);
				QFile seasonFile(seasonPath + "/season.json");
				downloadManager.appendFile(seasonPath, season.coverImage());
				if (seasonFile.open(QIODevice::WriteOnly)) {
					QTextStream out(&seasonFile);
					out << season.json();
					seasonFile.close();

				} else {

					// TODO P[1] raise error
				}
			} else {
				// TODO P[0] update
			}
		}
		downloadManager.start();
	} else {
		updateBook(book);
	}
}

void FileManager::updateBook(const Book &book)
{
	// TODO implement update book and add md5 for images
}

void FileManager::createBookFolders(const Book &book)
{
	QDir dir(QDir::currentPath() + "/" + book.authorId());

	if (!dir.exists()) {
		dir.mkpath(QDir::currentPath() + "/" + book.authorId() + "/" + book.id());
		dir.mkpath(QDir::currentPath() + "/" + book.authorId() + "/" + book.id() + "/seasons");
	}
}
