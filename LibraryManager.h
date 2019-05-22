#ifndef LIBRARYMANAGER_H
#define LIBRARYMANAGER_H

#include <map>
#include <vector>

#include <QObject>

#include "Author.h"
#include "Book.h"
#include "FileManager.h"

class BookDownloader;

class LibraryManager : public QObject
{
	Q_OBJECT
public:
	explicit LibraryManager(FileManager &ifileManager, QObject *parent = nullptr);

	void appendAuthor(Author author);
	void fetchAuthorBooks(QString id);
signals:

public slots:

private:
	std::map<QString, std::vector<Book>> libraries;
	BookDownloader *booksDownloader;
	std::vector<Author> authors;
	FileManager &fileManager;
};

#endif // LIBRARYMANAGER_H
