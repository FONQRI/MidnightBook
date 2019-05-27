#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <optional>

#include <QDir>

#include "Book.h"
#include "DownloadManager.h"

class FileManager : public QObject
{
	Q_OBJECT
public:
	FileManager();
	void createAuthorFolders(const QString &authorId);
	void saveOrUpdateBook(const Book &book);
	void saveOrUpdateBookSeasons(const Book &book);
	void saveBook(const Book &book, const QDir &bookDir);
	void updateBook(const Book &book, const QDir &bookDir);
	void saveOrUpdateSeason(const Season &season, const QDir &bookDir);
	void saveSeason(const Season &season, const QString &seasonPath);
	void updateSeason(const Season &season, const QDir &seasonPath);
	void checkSeasonsUpdate(const Book &book, const QString &bookPath);

	bool bookNeedsUpdate(const Book &book, const QDir &bookDir);
signals:
	void updateUi();
	void raiseError(const QString &errorString);

private:
	void createBookFolders(const Book &book);
	std::optional<Book> readBook(const QDir &bookDir);
	QByteArray getMd5(const QByteArray &data);
	QString getImageMd5(QString path);
	bool removeImageMd5(QString path);

private:
	DownloadManager downloadManager;
};

#endif // FILEMANAGER_H
