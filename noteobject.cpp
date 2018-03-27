#include "noteobject.h"

NoteObject::NoteObject(QObject *parent)
{
    m_name = "";
    m_note = "";
}

NoteObject::NoteObject(QString name, QString note) : NoteObject()
{
    setName(name);
    setNote(note);
}

QDataStream &operator<<(QDataStream &out, const NoteObject &noteObject)
{
    out << noteObject.name() << noteObject.note();
    return out;
}

QDataStream &operator>>(QDataStream &in, NoteObject &noteObject)
{
    QString name;
    QString note;
    in >> name >> note;
    noteObject = NoteObject(name, note);
    return in;
}
