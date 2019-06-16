#ifndef BOOK_H
#define BOOK_H

#include <vector>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QUrl>

#include "Season.h"

class Book
{
public:
	Book();
	void setInfoObject(QJsonObject obj);
	void setSeasonObject(QJsonObject obj);

	QString json() const;

	QString authorId() const;
	void setAuthorId(const QString &authorId);

	QString id() const;
	void setId(const QString &id);

	int version() const;
	void setVersion(int version);

	QString name() const;
	void setName(const QString &name);

	//TODO add md5 for checking update
	QUrl coverImageUrl() const;
	void setCoverImageUrl(const QUrl &coverImageUrl);

	bool seasonsShouldUpdate() const;
	void setSeasonsShouldUpdate(bool seasonsShouldUpdate);

	const QList<Season *> &seasons() const;
	void setSeasons(const QList<Season *> seasons);

	bool operator<(const Book &book) const;

	QString coverImage_md5() const;
	void setCoverImage_md5(const QString &coverImage_md5);

	void write();

	QString summary() const;
	void setSummary(const QString &summary);

	QString CoverImagePath() const;
	void setCoverImagePath(const QString &CoverImagePath);

private:
	QString m_authorId;
	QString m_id;
	int m_version{0};
	QString m_name;
	QUrl m_coverImageUrl;
	QString m_CoverImagePath;
	QString m_coverImage_md5;
	QString m_summary;
	bool m_seasonsShouldUpdate{true};

	QList<Season *> m_seasons;
};

#endif // BOOK_H
