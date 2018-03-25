#ifndef DATALIST_H
#define DATALIST_H

#include <QObject>
#include <QVector>

struct DataItem
{
    QString textDescription;
    QString textPane;
};


class DataList : public QObject
{
    Q_OBJECT
public:
    explicit DataList(QObject *parent = nullptr);

    QVector<DataItem> items() const;

    bool setItemAt(int index, const DataItem &item);

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int index);
    void postItemRemoved();

public slots:
    void appendItem();
    void removeCompletedItems();

private:
    QVector<DataItem> mItems;
};

#endif
