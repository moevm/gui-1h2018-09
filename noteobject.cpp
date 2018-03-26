#include "noteobject.h"

NoteObject::NoteObject(QObject *parent) : QObject(parent)
{
    m_name = "";
    m_note = "";
}

NoteObject::NoteObject(QString name, QString note) : NoteObject()
{
    setName(name);
    setNote(note);
}
