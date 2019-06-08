#ifndef SEASON_H
#define SEASON_H

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QObject>
#include <QString>
#include <QUrl>

class Season : public QObject
{
	Q_OBJECT
public:
	Season(int id, int version, QUrl coverImage, QString name, QString content);
	Season(QJsonObject obj);
	Season(const Season &season);

	QString json() const;

	int id() const;
	void setId(int id);

	int version() const;
	void setVersion(int version);

	bool shouldUpdate() const;
	void setShouldUpdate(bool shouldUpdate);

	QUrl coverImage() const;
	void setCoverImage(const QUrl &coverImage);

	QString name() const;
	void setName(const QString &name);

	QString content() const;
	void setContent(const QString &content);

	bool operator<(const Season &season)
	{
		return this->version() < season.version();
	}

	QString coverImage_md5() const;
	void setCoverImage_md5(const QString &coverImage_md5);

private:
	int m_id{0};
	int m_version{0};
	QUrl m_coverImage;
	QString m_coverImage_md5;
	QString m_name;
	QString m_content;
	bool m_shouldUpdate{false};
};

#endif // SEASON_H
