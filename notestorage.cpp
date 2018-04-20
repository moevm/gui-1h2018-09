#include "notestorage.h"
#include "QDebug"

#include <QFileDialog>
#include <QFileInfo>
#include <QStandardPaths>
#include <QRegExp>
#include <QTextEdit>
#include <QApplication>
#include <QWidget>
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
    restore();
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

void NoteStorage::swapLists() {

    int m_notesLength = m_notes.size();
    int filteredLength = filtered.size();

    for(int i = 0; i < m_notesLength; i++) {
        filtered.push_front(m_notes[i]);
    }

    for(int i = 0; i < filteredLength; i++) {
        m_notes.push_front(filtered.takeLast());
    }

    for(int i = 0; i < m_notesLength; i++) {
        m_notes.takeLast();
}
}


QList<QString> NoteStorage::pureData() const
{
    QWidget *parent = nullptr;
    QTextEdit *curr = new QTextEdit(parent);
    QList<QString> *notes = new QList<QString>;

    for(int i = 0; i < m_notes.size(); i++) {
        curr->setHtml(m_notes[i].note());
        QString currNote = curr->toPlainText();
        notes->push_front(currNote);


    }

    return *notes;
}


void NoteStorage::restore() {
    qDebug() << "Chistim filtered, apendim m_notes";
    for(int i = 0; i < filtered.size(); i++){
     m_notes.append(filtered[i]);
     qDebug() << filtered[i].name();
    }
     filtered.clear();
}

void NoteStorage::search(QString text)
{
    restore();
    QWidget *parent = nullptr;
    QTextEdit *editor = new QTextEdit(parent) ;
    QList<int> indexes;
    QRegExp templ(text);
    for(int i = 0; i < m_notes.size(); i++){
        qDebug() << "ELEMENT : > > >  " << m_notes[i].name();
        editor->setHtml(m_notes[i].note());
        qDebug() << "TEXT > > >  " << editor->toPlainText();
        int pos = templ.indexIn(editor->toPlainText());
        if(pos != -1) {
            qDebug() << "Match > > >  " << m_notes[i].name() << " " << m_notes.size();
            filtered.append(m_notes[i]);
            m_notes.removeAt(i);
            i--;


        }

    }
    /* swap lists */

    swapLists();

    /*----------*/
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
    for(int i = 0; i < filtered.size(); i++){
     m_notes.append(filtered[i]);
     qDebug() << filtered[i].note();
    }
    QString defaultName = "default.marxxlib";
    QString defaultFilePath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/" + defaultName;

    qDebug() << "Saving to default" << defaultFilePath;

    pack(defaultFilePath, false);
}
