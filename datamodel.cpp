#include "datamodel.h"
#include "datalist.h"

DataModel::DataModel(QObject *parent)
    : QAbstractListModel(parent),
      mList(nullptr)
{
}

int DataModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !mList)
        return 0;

    return mList->items().size();
}

QVariant DataModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid() || !mList)
        return QVariant();

    const DataItem item =  mList->items().at(index.row());
    switch(role){
    case DescriptionRole:
        return QVariant(item.textDescription);
    case TextPane:
        return QVariant(item.textPane);
    }
    return QVariant();
}

bool DataModel::setData(const QModelIndex &index, const QVariant &value, int role)
{

    if(!mList)
        return false;

    DataItem item = mList->items().at(index.row());

    switch(role){
    case DescriptionRole:
        item.textDescription = value.toString();
        break;
    case TextPane:
        item.textPane = value.toString();
        break;
    }

    if (mList->setItemAt(index.row(), item)) {
        // FIXME: Implement me!
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags DataModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> DataModel::roleNames() const
{
    QHash<int, QByteArray> names;

    names[DescriptionRole] = "textDescription";
    names[TextPane] = "textPane";

    return names;
}

DataList *DataModel::list() const
{
    return mList;
}

void DataModel::setList(DataList *list)
{
    beginResetModel();

    if (mList)
        mList->disconnect(this);

    mList = list;

    if(mList) {
        connect(mList, &DataList::preItemAppended, this, [=]() {
            const int index = mList->items().size();
            beginInsertRows(QModelIndex(), index, index);
        });

        connect(mList, &DataList::postItemAppended, this, [=]() {
            endInsertRows();
        });

        connect(mList, &DataList::preItemRemoved, this, [=](int index) {
            beginRemoveRows(QModelIndex(), index, index);
        });
        connect(mList, &DataList::postItemRemoved, this, [=]() {
            endRemoveRows();
        });

        endResetModel();

    }
}
