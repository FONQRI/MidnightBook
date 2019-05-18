#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Book.h"

class FileManager
{
	FileManager();

public:
	static void createAuthorFolders(const QString &authorId);
	static void saveBook(Book book);
};

#endif // FILEMANAGER_H
