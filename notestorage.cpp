#include "notestorage.h"
#include "QDebug"

NoteStorage::NoteStorage(QObject *parent)
{

}

void NoteStorage::append(QString name, QString note)
{
    beginInsertRows(QModelIndex(), m_notes.size(), m_notes.size());
    m_notes.append(new NoteObject(name, note));
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
    qDebug() << "data retrieve" << index.row() << role;

    if (!index.isValid()) {
        return QVariant();
    }

    switch (role) {
    case NameRole:
        return QVariant(m_notes.at(index.row())->name());
    case NoteRole:
        return QVariant(m_notes.at(index.row())->note());
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
        m_notes[index.row()]->setName(value.toString());
        break;
    case NoteRole:
        m_notes[index.row()]->setNote(value.toString());
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
