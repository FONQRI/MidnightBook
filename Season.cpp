#include "Season.h"

Season::Season(int id, int version, QUrl coverImage, QString name, QString content)
	: m_id(id)
	, m_version(version)
	, m_coverImage(coverImage)
	, m_name(name)
	, m_content(content)
{}

Season::Season(QJsonObject obj)
{
	m_id = obj["id"].toInt();
	m_version = obj["version"].toInt();
	m_coverImage = obj["cover_image"].toString();
	m_coverImage = obj["coverImage_md5"].toString();
	m_name = obj["name"].toString();
	m_content = obj["content"].toString();
}

QString Season::json() const
{
	QJsonObject obj;
	obj.insert("id", m_id);
	obj.insert("version", m_version);
	obj.insert("name", m_name);
	obj.insert("cover_image", m_coverImage.url());
	obj.insert("coverImage_md5", m_coverImage_md5);
	obj.insert("shouldUpdate", m_shouldUpdate);
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

QUrl Season::coverImage() const
{
	return m_coverImage;
}

void Season::setCoverImage(const QUrl &coverImage)
{
	m_coverImage = coverImage;
}

QString Season::name() const
{
	return m_name;
}

void Season::setName(const QString &name)
{
	m_name = name;
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
