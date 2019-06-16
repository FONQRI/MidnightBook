#ifndef LIBRARYMANAGER_H
#define LIBRARYMANAGER_H

#include <map>
#include <vector>

#include <QAbstractListModel>
#include <QObject>

#include "src/cpp/models/Author.h"
#include "src/cpp/models/Book.h"
#include "src/cpp/storage/FileManager.h"

class BookDownloader;

class LibraryManager : public QAbstractListModel
{
public:
	explicit LibraryManager(QObject *parent = nullptr);

	enum BookRoles {
		Id = Qt::UserRole + 1,
		Version,
		Name,
		CoverImageUrl,
		CoverImagePath,
		Seasons,
		Summary
	};

	void appendAuthor(Author author);
	void fetchAuthorBooks(QString id);

private:
	std::map<QString, std::vector<Book>> libraries;
	BookDownloader *booksDownloader;
	std::vector<Author> authors;
	FileManager fileManager;
	std::vector<Book> bookList;

private:
	void readBooks();

	// QAbstractItemModel interface
public:
	int rowCount(const QModelIndex &parent) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QHash<int, QByteArray> roleNames() const override;
};

#endif // LIBRARYMANAGER_H
