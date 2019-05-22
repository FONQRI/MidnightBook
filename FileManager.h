#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Book.h"
#include "DownloadManager.h"

class FileManager : public QObject
{
	Q_OBJECT
public:
	FileManager();
	void createAuthorFolders(const QString &authorId);
	void saveBook(const Book &book);
	void updateBook(const Book &book);

signals:
	void updateUi();

private:
	void createBookFolders(const Book &book);

private:
	DownloadManager downloadManager;
};

#endif // FILEMANAGER_H
