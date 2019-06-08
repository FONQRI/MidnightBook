#ifndef AUTHOR_H
#define AUTHOR_H
#include <QString>
#include <QUrl>

struct Author
{
	QString name;
	QString id;
	QUrl photo;
};

#endif // AUTHOR_H
