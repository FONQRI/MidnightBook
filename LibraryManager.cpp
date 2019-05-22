#include "LibraryManager.h"

#include "BookDownloader.h"
#include "FileDownloader.h"
#include "FileManager.h"
#include "LibraryInfo.h"

LibraryManager::LibraryManager(FileManager &ifileManager, QObject *parent)
	: QObject(parent)
	, fileManager(ifileManager)
{}

void LibraryManager::appendAuthor(Author author)
{
	authors.push_back(author);
}

void LibraryManager::fetchAuthorBooks(QString id)
{
	booksDownloader->fetchBooks(id);
}
