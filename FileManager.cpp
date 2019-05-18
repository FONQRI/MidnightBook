#include "FileManager.h"

#include <QDebug>
#include <QDir>

FileManager::FileManager()
{}

void FileManager::createAuthorFolders(const QString &authorId)
{
	QDir dir(QDir::currentPath() + "/" + authorId);

	if (!dir.exists()) {
		dir.mkpath(QDir::currentPath() + "/" + authorId);
	}
}

void FileManager::saveBook(Book book)
{
	QDir dir(QDir::currentPath() + "/" + book.authorId);

	if (!dir.exists()) {
		dir.mkpath(QDir::currentPath() + "/" + book.authorId+"/"+book.id);
		dir.mkpath(QDir::currentPath() + "/" + book.authorId+"/"+book.id+"/seasons");
	}
}
