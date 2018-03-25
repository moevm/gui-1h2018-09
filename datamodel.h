#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QAbstractListModel>


class DataList;

class DataModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(DataList *list READ list WRITE setList)

public:
    explicit DataModel(QObject *parent = nullptr);

    enum {
        DescriptionRole = Qt::UserRole,
        TextPane
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    virtual QHash<int,QByteArray> roleNames() const override;

    DataList *list() const;
    void setList(DataList *list);

private:
    DataList *mList;
};

#endif
