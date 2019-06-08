#ifndef BOOKSINFO_H
#define BOOKSINFO_H

#include <QObject>
#include <QTimer>

#include "src/cpp/models/Book.h"

class QNetworkReply;

class BookDownloader : public QObject
{
	Q_OBJECT
public:
	explicit BookDownloader(QObject *parent = nullptr);

	QString serverUrl() const;
	void setServerUrl(const QString &serverUrl);

	QString authorId() const;
	void setAuthorId(const QString &authorId);

signals:
	void seasonsDownloadFinished(Book book);
	void booksFetchFinished(std::vector<Book> bookList);
	void bookFetchFinished(Book bookList);

public slots:

	void fetchBooks(QString authorId);
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
	void requestBooksFinished(QNetworkReply *reply, QString authorId);
	void requestBookFinished(QNetworkReply *reply);
	void requestSeasonsFinished(QNetworkReply *reply, const QString &bookid);

	QString m_serverUrl;

	QString m_authorId{"FONQRI"};
};

#endif // BOOKSINFO_H
