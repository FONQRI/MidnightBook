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
	void saveOrUpdateBook(Book book);
	void saveOrUpdateBookSeasons(Book book);
	void saveBook(Book book, const QDir &bookDir);
	void updateBook(Book book, const QDir &bookDir);
	void saveSeason(const Season &season, const QString &bookPath);
	void updateSeason(const Season &season, const QString &bookPath);
	void checkSeasonsUpdate(const Book &book, const QString &bookPath);
	void bookFetchFinished(Book book);

	bool bookNeedsUpdate(const Book &book, const QDir &bookDir);
signals:
	void updateUi();
	void raiseError(const QString &errorString);

private:
	void createBookFolders(const Book &book);
	std::optional<Book> readBook(const QString &bookPath);
	std::optional<Book> readBook(const QDir &bookDir);
	std::optional<Season> readSeason(const QDir &seasonDir);
	std::optional<Season> readSeason(const QString &seasonPath);
	QByteArray getMd5(const QByteArray &data);
	QString getImageMd5(QString path);
	bool removeImageFromDir(QString path);
	void updatePhotoMd5(const QString &filename);

private:
	DownloadManager downloadManager;
};

#endif // FILEMANAGER_H
