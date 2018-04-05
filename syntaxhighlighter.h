#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QObject>
#include <QSyntaxHighlighter>
#include <string>
#include <QRegExp>
#include <regex>
#include <iostream>
using namespace std;

class SyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    explicit SyntaxHighlighter(QObject *parent = nullptr);
    string parseline(string& line);
    void highlightBlock(const QString &text);

private:

    QHash<QString, QString> patterns;

    void str_replace( string &s, string &search, string &replace );

    bool search_headers_style(string& stringToReturn, std::smatch& match);

    bool need_paragraph;
    bool current_paragraph;
    bool current_list;
signals:

public slots:
};

#endif // SYNTAXHIGHLIGHTER_H
