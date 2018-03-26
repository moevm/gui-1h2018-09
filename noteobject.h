#ifndef NOTEOBJECT_H
#define NOTEOBJECT_H

#include <QObject>

class NoteObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString note READ note WRITE setNote NOTIFY noteChanged)

    QString m_name, m_note;


public:
    explicit NoteObject(QObject *parent = nullptr);
    NoteObject(QString name, QString note);

    QString name() const { return m_name; }
    QString note() const { return m_note; }

    void setName(QString name) { if(m_name != name) { m_name = name; /*emit nameChanged();*/ } }
    void setNote(QString note) { if(m_note != note) { m_note = note; /*emit noteChanged();*/ } }

signals:

    void nameChanged();
    void noteChanged();

public slots:
};

#endif // NOTEOBJECT_H
