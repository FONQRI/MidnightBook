#ifndef BOOKSINFO_H
#define BOOKSINFO_H

#include <QObject>
#include <QTimer>

#include "Book.h"

class QNetworkReply;

class DownloaderManager : public QObject
{
	Q_OBJECT
public:
	explicit DownloaderManager(QObject *parent = nullptr);

	QString serverUrl() const;
	void setServerUrl(const QString &serverUrl);

	QString authorId() const;
	void setAuthorId(const QString &authorId);

signals:

public slots:

	void fetchBooks();
	void fetchBook(QString id);
	void downloadSeasons(QString BookId, std::vector<int> seasonIds);

private:
	QTimer timerBook;
	QTimer timerBooks;
	QTimer timerSeasons;
	QNetworkReply *bookReply;
	QNetworkReply *booksReply;
	QNetworkReply *seasonsReply;

private:
	void requestBooksFinished(QNetworkReply *reply);
	void requestBookFinished(QNetworkReply *reply);
	void requestSeasonsFinished(QNetworkReply *reply);

	Book testBook;

	QString m_serverUrl;

	QString m_authorId{"FONQRI"};
};

#endif // BOOKSINFO_H
