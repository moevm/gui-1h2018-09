#ifndef NOTESTORAGE_H
#define NOTESTORAGE_H

#include <QAbstractListModel>
#include <QDataStream>
#include <QFile>
#include "noteobject.h"

class NoteStorage : public QAbstractListModel
{
    Q_OBJECT

    QList<NoteObject> m_notes;

    void loadFromDefault();
public:
    NoteStorage(QObject *parent = 0);

    enum NoteRoles {
        NameRole = Qt::UserRole + 1,
        NoteRole
    };


    Q_INVOKABLE void append(QString name, QString note);
    Q_INVOKABLE void remove(int index);

    Q_INVOKABLE QHash<int, QByteArray> roleNames() const;
    Q_INVOKABLE int rowCount(const QModelIndex &parent) const;
    Q_INVOKABLE int rows() const;
    Q_INVOKABLE QVariant data(const QModelIndex &index, int role) const;
    Q_INVOKABLE bool setData(const QModelIndex &index, const QVariant &value, int role);
    Q_INVOKABLE Qt::ItemFlags flags(const QModelIndex &index) const;

    Q_INVOKABLE void pack(QString path, bool url);
    Q_INVOKABLE void packNote(int index, QString path, bool url);
    Q_INVOKABLE void unpack(QString path, bool url);
    Q_INVOKABLE void saveToDefault();
};

#endif // NOTESTORAGE_H
