#include "src/cpp/network/BookDownloader.h"

#include <iostream>

#include <QTimer>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

#include <QNetworkAccessManager>
#include <QNetworkReply>

#include "src/cpp/models/LibraryInfo.h"
#include <numeric>

BookDownloader::BookDownloader(QObject *parent)
	: QObject(parent)
{}

void BookDownloader::fetchBooks(QString authorId)
{
	timerBooks.start(30000);

	QNetworkAccessManager *manager = new QNetworkAccessManager();

	QNetworkRequest req;
	timerBooks.setSingleShot(true);

	req.setUrl(QUrl(serverUrl() + "/find"));
	req.setRawHeader("Accept-Encoding", "gzip, deflate");
	req.setRawHeader("Content-Type", "application/json");
	req.setRawHeader("client_key", variables::client_key);
	req.setRawHeader("username", authorId.toStdString().c_str());
	req.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));

	QJsonObject request_object;

	QJsonObject query_object;

	QJsonObject projection_object;
	projection_object.insert("_id", 0);
	projection_object.insert("seasons", 0);
	request_object.insert("query", query_object);
	request_object.insert("projection", projection_object);

	booksReply = manager->post(req, QJsonDocument(request_object).toJson());
	connect(&timerBook, &QTimer::timeout, [this]() {
		if (booksReply->isRunning()) {
			booksReply->abort();
		}
	});

	QObject::connect(manager, &QNetworkAccessManager::finished,
					 [this, authorId](QNetworkReply *reply) {
						 requestBooksFinished(reply, authorId);
					 });
}

void BookDownloader::fetchBook(QString id)
{

	timerBook.start(30000);

	QNetworkAccessManager *manager = new QNetworkAccessManager();

	QNetworkRequest req;
	timerBook.setSingleShot(true);

	req.setUrl(QUrl(serverUrl() + "/find_one"));
	req.setRawHeader("Accept-Encoding", "gzip, deflate");
	req.setRawHeader("Content-Type", "application/json");
	req.setRawHeader("client_key", variables::client_key);
	req.setRawHeader("username", variables::authorId);
	req.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));

	QJsonObject request_object;

	QJsonObject query_object;
	query_object.insert("id", id);

	QJsonObject projection_object;
	projection_object.insert("_id", 0);
	projection_object.insert("seasons.content", 0);
	request_object.insert("query", query_object);
	request_object.insert("projection", projection_object);

	bookReply = manager->post(req, QJsonDocument(request_object).toJson());
	connect(&timerBook, &QTimer::timeout, [this]() {
		if (bookReply->isRunning()) {
			bookReply->abort();
		}
	});

	QObject::connect(manager, &QNetworkAccessManager::finished, this,
					 &BookDownloader::requestBookFinished);
}

void BookDownloader::downloadSeasons(QString BookId, std::vector<int> seasonIds)
{

	timerSeasons.start(30000);

	QNetworkAccessManager *manager = new QNetworkAccessManager();

	QNetworkRequest req;
	timerSeasons.setSingleShot(true);

	req.setUrl(QUrl(serverUrl() + "/aggregate"));
	req.setRawHeader("Accept-Encoding", "gzip, deflate");
	req.setRawHeader("Content-Type", "application/json");
	req.setRawHeader("client_key", variables::client_key);
	req.setRawHeader("username", variables::authorId);
	req.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/json"));

	QJsonObject request_object;
	// example
	/*
	{
	"pipeline":[
	{"match":{
	"name":"book2"
	}},
	{"project":{
	"_id":0,
	  "seasons":{
	 "$filter": {
		   "input": "$seasons",
		   "as": "season",
		   "cond": { "$in": [ "$$season.id", [1,2] ] }
			}
		}
		  }
	}
	]
	}
	*/
	QJsonArray pipeline;
	QJsonObject match_object;
	match_object.insert("id", BookId);

	QJsonArray seasonIdsArray;
	for (int value : seasonIds) {
		seasonIdsArray.append(value);
	}
	QJsonArray in;
	in.append("$$season.id");
	in.append(seasonIdsArray);
	QJsonObject cond;
	cond.insert("$in", in);

	QJsonObject filter_object;
	filter_object.insert("input", "$seasons");
	filter_object.insert("as", "season");
	filter_object.insert("cond", cond);

	QJsonObject seasons_object;
	seasons_object.insert("$filter", filter_object);

	QJsonObject project_object;
	project_object.insert("_id", 0);
	project_object.insert("seasons", seasons_object);

	pipeline.append(QJsonObject{{"match", match_object}});
	pipeline.append(QJsonObject{{"project", project_object}});

	request_object.insert("pipeline", pipeline);

	seasonsReply = manager->post(req, QJsonDocument(request_object).toJson());
	connect(&timerSeasons, &QTimer::timeout, [this]() {
		if (seasonsReply->isRunning()) {
			seasonsReply->abort();
		}
	});

	connect(manager, &QNetworkAccessManager::finished,
			[this, BookId](QNetworkReply *reply) { requestSeasonsFinished(reply, BookId); });
}

void BookDownloader::requestBookFinished(QNetworkReply *reply)
{
	Book tempBook;

	if (reply->error() != QNetworkReply::NoError) {
		qDebug() << "Error:" << reply->errorString();
		return;
	}
	QByteArray buf = reply->readAll();
	tempBook.setAuthorId(authorId());
	// TODO get seasons json
	auto bookObject = QJsonDocument::fromJson(buf).object()["Response"].toObject();
	qDebug() << "FONQRI" << buf;
	tempBook.setInfoObject(bookObject);

	//	auto seaonsObject = bookObject["seasons"].toObject();
	tempBook.setSeasonObject(bookObject);
	// FIXME add seasons version vector in book and fill it here to can check season update
	//	auto  seasonsArray = seaonsObject["seasons"].toArray();

	emit bookFetchFinished(tempBook);
}

void BookDownloader::requestSeasonsFinished(QNetworkReply *reply, const QString &bookid)
{

	Book tempBook;
	tempBook.setId(bookid);
	tempBook.setAuthorId(variables::authorId);
	if (reply->error() != QNetworkReply::NoError) {
		qDebug() << "Error:" << reply->errorString();
		return;
	}
	QByteArray buf = reply->readAll();

	tempBook.setSeasonObject(QJsonDocument::fromJson(buf).object()["Response"].toObject());
	emit seasonsDownloadFinished(tempBook);
}

QString BookDownloader::authorId() const
{
	return m_authorId;
}

void BookDownloader::setAuthorId(const QString &authorId)
{
	m_authorId = authorId;
}

QString BookDownloader::serverUrl() const
{
	return m_serverUrl;
}

void BookDownloader::setServerUrl(const QString &serverUrl)
{
	m_serverUrl = serverUrl;
}
void BookDownloader::requestBooksFinished(QNetworkReply *reply, QString authorId)
{
	if (reply->error() != QNetworkReply::NoError) {
		qDebug() << "Error:" << reply->errorString();
		return;
	}
	QByteArray buf = reply->readAll();
	auto booksInfo = QJsonDocument::fromJson(buf).object()["Response"].toArray();

	std::vector<Book> bookList;
	for (auto bookInfo : booksInfo) {
		Book book;
		book.setInfoObject(bookInfo.toObject());
		qDebug() << __FUNCTION__ << " MD5 :" << book.coverImage_md5();
		book.setAuthorId(authorId);
		bookList.push_back(book);
	}

	emit booksFetchFinished(bookList);
	//	qDebug() << "____________________";
}
