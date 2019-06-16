#include "Season.h"
#include <QDebug>

//Season::Season(int id, int version, QUrl coverImage, QString name, QString content)
//	: m_id(id)
//	, m_version(version)
//	, m_coverImage(coverImage)
//	, m_name(name)
//	, m_content(content)
//{}

Season::Season(QJsonObject obj)
{
	m_id = obj["id"].toInt();
	m_version = obj["version"].toInt();
	m_coverImagePath = obj["cover_image_path"].toString();
	m_coverImageUrl = obj["cover_image_url"].toString();
	m_coverImage_md5 = obj["cover_image_md5"].toString();
	m_name = obj["name"].toString();
	m_summary = obj["summary"].toString();
	m_content = obj["content"].toString();
}

Season::Season(const Season &season)
{
	this->m_id = season.id();
	this->m_name = season.name();
	this->m_summary = season.summary();
	this->m_content = season.content();
	this->m_version = season.version();
	this->m_coverImagePath = season.coverImagePath();
	this->m_coverImageUrl = season.coverImageUrl();
	this->m_coverImage_md5 = season.coverImage_md5();
	this->m_shouldUpdate = season.shouldUpdate();
}

QString Season::json() const
{
	QJsonObject obj;
	obj.insert("id", m_id);
	obj.insert("version", m_version);
	obj.insert("name", m_name);
	obj.insert("cover_image_path", m_coverImagePath);
	obj.insert("cover_image_url", m_coverImageUrl.url());
	obj.insert("cover_image_md5", m_coverImage_md5);
	obj.insert("shouldUpdate", m_shouldUpdate);
	obj.insert("summary", m_summary);
	obj.insert("content", m_content);

	return QJsonDocument(obj).toJson();
}

int Season::id() const
{
	return m_id;
}

void Season::setId(int id)
{
	m_id = id;
}

int Season::version() const
{
	return m_version;
}

void Season::setVersion(int version)
{
	m_version = version;
}

bool Season::shouldUpdate() const
{
	return m_shouldUpdate;
}

void Season::setShouldUpdate(bool shouldUpdate)
{
	m_shouldUpdate = shouldUpdate;
}

QUrl Season::coverImageUrl() const
{
	return m_coverImageUrl;
}

void Season::setCoverImageUrl(const QUrl &coverImage)
{
	m_coverImageUrl = coverImage;
}

QString Season::name() const
{
	return m_name;
}

void Season::setName(const QString &name)
{
	m_name = name;
	emit nameChanged(m_name);
}

QString Season::content() const
{
	return m_content;
}

void Season::setContent(const QString &content)
{
	m_content = content;
}

QString Season::coverImage_md5() const
{
	return m_coverImage_md5;
}

void Season::setCoverImage_md5(const QString &coverImage_md5)
{
	m_coverImage_md5 = coverImage_md5;
}

QString Season::summary() const
{
	return m_summary;
}

void Season::setSummary(const QString &summary)
{
	m_summary = summary;
}

QString Season::coverImagePath() const
{
	return m_coverImagePath;
}

void Season::setCoverImagePath(const QString &coverImagePath)
{
	m_coverImagePath = coverImagePath;
}
