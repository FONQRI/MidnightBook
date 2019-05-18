#ifndef BOOK_H
#define BOOK_H

#include <QJsonArray>
#include <QJsonObject>
#include <QString>
#include <QUrl>

struct Season
{
	Season(int id, int version, QUrl coverImage, QString name, QString content)
		: id(id)
		, version(version)
		, coverImage(coverImage)
		, name(name)
		, content(content)
	{}
	Season(QJsonObject obj)
	{
		id = obj["id"].toInt();
		version = obj["version"].toInt();
		coverImage = obj["coverImage"].toString();
		name = obj["name"].toString();
		content = obj["content"].toString();
	}
	int id{0};
	int version{0};
	QUrl coverImage;
	QString name;
	QString content;
};

struct Book
{
	//  Book(BookInfo bookInfo)
	//	: bookInfo(bookInfo)
	//  {}
	Book()
	{}
	QString authorId;
	QString id;
	int version{0};
	QString name;
	QUrl coverImage;

	QList<Season> seasons;
	void setInfoObject(QJsonObject obj)
	{
		id = obj["id"].toString();
		version = obj["version"].toInt();
		name = obj["name"].toString();
		coverImage = obj["coverImage"].toString();
	}
	void setSeasonObject(QJsonObject obj)
	{

		QJsonArray seasonsArray = obj["seasons"].toArray();
		for (auto const season : seasonsArray) {
			seasons.append(Season(season.toObject()));
		}
	}
};

#endif // BOOK_H
