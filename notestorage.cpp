#include "notestorage.h"
#include "QDebug"

#include <QFileDialog>
#include <QFileInfo>
#include <QStandardPaths>

NoteStorage::NoteStorage(QObject *parent)
{
    loadFromDefault();
}

void NoteStorage::loadFromDefault()
{
    QString defaultName = "default.marxxlib";
    QString defaultFilePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + defaultName;
    QFileInfo check_file(defaultFilePath);

    qDebug() << "Checking file:" << defaultFilePath;

    if(check_file.exists() && check_file.isFile()) {
        unpack(defaultFilePath, false);
        qDebug() << "Resrored!";
    }
}

void NoteStorage::append(QString name, QString note)
{
    beginInsertRows(QModelIndex(), m_notes.size(), m_notes.size());
    m_notes.append(NoteObject(name, note));
    endInsertRows();
}

void NoteStorage::remove(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    m_notes.removeAt(index);
    endRemoveRows();
}

int NoteStorage::rows() const
{
    return m_notes.size();
}

QHash<int, QByteArray> NoteStorage::roleNames() const
{
    qDebug() << "RoleNames retrieve";
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[NoteRole] = "note";
    return roles;
}

int NoteStorage::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }

    return m_notes.size();
}

QVariant NoteStorage::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    switch (role) {
    case NameRole:
        return QVariant(m_notes.at(index.row()).name());
    case NoteRole:
        return QVariant(m_notes.at(index.row()).note());
    default:
        return QVariant();
    }
}

bool NoteStorage::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid()) {
        return false;
    }

    switch (role) {
    case NameRole:
        m_notes[index.row()].setName(value.toString());
        break;
    case NoteRole:
        m_notes[index.row()].setNote(value.toString());
        break;
    default:
        return false;
    }

    emit dataChanged(index, index, QVector<int>() << role);

    return true;
}

Qt::ItemFlags NoteStorage::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractListModel::flags(index) | Qt::ItemIsEditable;
}

void NoteStorage::pack(QString path, bool url = true)
{
    QString filePath = url ? QUrl(path).toLocalFile() : path;
    QFile file(filePath);

    file.open(QIODevice::WriteOnly);
    QDataStream packed(&file);

    int listSize = m_notes.size();
    packed << (qint32)listSize;
    foreach (NoteObject no, m_notes) {
        packed << no;
    }
}

void NoteStorage::packNote(int index, QString path, bool url = true)
{
    qDebug() << "Current index" << index;
    QString filePath = url ? QUrl(path).toLocalFile() : path;
    QFile file(filePath);
    file.open(QIODevice::WriteOnly);
    QDataStream packed(&file);

    int noteSize = sizeof(m_notes.at(index).note());
    packed << (qint32)noteSize;
    packed << m_notes.at(index).note();
}

void NoteStorage::unpack(QString path, bool url = true)
{
    QString filePath = url ? QUrl(path).toLocalFile() : path;

    QFile file(filePath);
    file.open(QIODevice::ReadOnly);
    QDataStream packed(&file);

    m_notes.clear();
    qint32 listSize = m_notes.size();
    packed >> listSize;
    for(int i = 0; i < listSize; ++i) {
        NoteObject no;
        packed >> no;
        m_notes.append(no);
    }
}

void NoteStorage::saveToDefault()
{
    QString defaultName = "default.marxxlib";
    QString defaultFilePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + defaultName;

    qDebug() << "Saving to default" << defaultFilePath;

    pack(defaultFilePath, false);
}
