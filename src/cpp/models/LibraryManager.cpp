#include "LibraryManager.h"

#include "src/cpp/models/LibraryInfo.h"
#include "src/cpp/network/BookDownloader.h"
#include "src/cpp/network/FileDownloader.h"
#include "src/cpp/storage/FileManager.h"

LibraryManager::LibraryManager(QObject *parent)
{
	readBooks();
}

void LibraryManager::appendAuthor(Author author)
{
	authors.push_back(author);
}

void LibraryManager::fetchAuthorBooks(QString id)
{
	booksDownloader->fetchBooks(id);
}

void LibraryManager::readBooks()
{
	bookList = fileManager.readBooks(variables::authorId);
}

int LibraryManager::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);

	return static_cast<int>(bookList.size());
}

QVariant LibraryManager::data(const QModelIndex &index, int role) const
{
	if (!hasIndex(index.row(), index.column(), index.parent()))
		return {};
	if (static_cast<size_t>(index.row()) > bookList.size())
		return {};
	const auto &item = bookList.at(static_cast<size_t>(index.row()));
	if (role == BookRoles::Id) {
		return item.id();
	}
	if (role == BookRoles::Version) {
		return item.version();
	}
	if (role == BookRoles::Name) {
		qDebug() << __FUNCTION__ << __LINE__;
		return item.name();
	}
	if (role == BookRoles::CoverImageUrl) {
		qDebug() << __FUNCTION__ << __LINE__;
		qDebug() << "{FONQRI}:" << item.coverImageUrl();
		return item.coverImageUrl();
	}
	if (role == BookRoles::CoverImagePath) {
		qDebug() << __FUNCTION__ << __LINE__;
		qDebug() << "{FONQRI}:" << item.CoverImagePath();
		return item.CoverImagePath();
	}
	if (role == BookRoles::Summary) {
		return item.summary();
	}
	if (role == BookRoles::Seasons) {
		QList<QObject *> list;
		for (auto season : item.seasons()) {
			list.push_back(season);
		}
		return QVariant::fromValue(list);
	}

	return {};
}

QHash<int, QByteArray> LibraryManager::roleNames() const
{
	QHash<int, QByteArray> roles;
	//	enum BookRoles { Id = Qt::UserRole + 1, Version, Name, CoverImage, Seasons, Summary	 };

	roles[Id] = "id";
	roles[Version] = "version";
	roles[Name] = "name";
	roles[CoverImageUrl] = "coverImageUrl";
	roles[CoverImagePath] = "CoverImagePath";
	roles[Seasons] = "seasons";
	roles[Summary] = "summary";
	return roles;
}
