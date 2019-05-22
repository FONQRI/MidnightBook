#include "Book.h"

Book::Book()
{}

void Book::setInfoObject(QJsonObject obj)
{
	m_id = obj["id"].toString();
	m_version = obj["version"].toInt();
	m_name = obj["name"].toString();
	m_coverImage = obj["cover_image"].toString();
}

void Book::setSeasonObject(QJsonObject obj)
{

	QJsonArray seasonsArray = obj["seasons"].toArray();
	for (auto const season : seasonsArray) {
		m_seasons.push_back(Season(season.toObject()));
	}
}

QString Book::json() const
{
	QJsonObject obj;
	obj.insert("id", m_id);
	obj.insert("version", m_version);
	obj.insert("name", m_name);
	obj.insert("cover_image", m_coverImage.url());
	obj.insert("shouldUpdate", m_shouldUpdate);

	return QJsonDocument(obj).toJson();
}

QString Book::id() const
{
	return m_id;
}

void Book::setId(const QString &id)
{
	m_id = id;
}

int Book::version() const
{
	return m_version;
}

void Book::setVersion(int version)
{
	m_version = version;
}

QString Book::name() const
{
	return m_name;
}

void Book::setName(const QString &name)
{
	m_name = name;
}

QUrl Book::coverImage() const
{
	return m_coverImage;
}

void Book::setCoverImage(const QUrl &coverImage)
{
	m_coverImage = coverImage;
}

bool Book::shouldUpdate() const
{
	return m_shouldUpdate;
}

void Book::setShouldUpdate(bool shouldUpdate)
{
	m_shouldUpdate = shouldUpdate;
}

const std::vector<Season> &Book::seasons() const
{
	return m_seasons;
}

void Book::setSeasons(const std::vector<Season> &seasons)
{
	m_seasons = seasons;
}

QString Book::authorId() const
{
	return m_authorId;
}

void Book::setAuthorId(const QString &authorId)
{
	m_authorId = authorId;
}
