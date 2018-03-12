#include "roomtreemodel.h"

RoomTreeModel::RoomTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{

    qDebug() << "model created";

    QVector<QVariant> rootData;
        rootData << "root node";

    rootItem = new RoomTreeItem(rootData);
    rootItem->insertChildren(rootItem->childCount(), 3, rootItem->columnCount());
    rootItem->child(0)->setData(0, "one sub 1");
    rootItem->child(1)->setData(0, "one sub 2");
    rootItem->child(2)->setData(0, "one sub 3");

    rootItem->child(1)->insertChildren(0, 2, rootItem->columnCount());
    rootItem->child(1)->child(0)->setData(0, "two sub 1");
    rootItem->child(1)->child(1)->setData(0, "two sub 2");


    qDebug() << "model creation finished";
}

RoomTreeModel::~RoomTreeModel()
{
    delete rootItem;
}

QHash<int, QByteArray> RoomTreeModel::roleNames() const
{
    qDebug() << "model: role names retrieve";

    QHash<int, QByteArray> result;
    result.insert(NameRole, QByteArrayLiteral("name"));
    result.insert(TypeRole, QByteArrayLiteral("type"));
    return result;
}

QModelIndex RoomTreeModel::index(int row, int column, const QModelIndex &parent)
            const
{

    if (!hasIndex(row, column, parent))
        return QModelIndex();

    RoomTreeItem *parentItem;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<RoomTreeItem*>(parent.internalPointer());

    RoomTreeItem *childItem = parentItem->child(row);
    if (childItem) {
        return createIndex(row, column, childItem);
    }

    else
        return QModelIndex();
}

QModelIndex RoomTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    RoomTreeItem *childItem = getItem(index);
    RoomTreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

int RoomTreeModel::rowCount(const QModelIndex &parent) const
{
    RoomTreeItem *parentItem;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = rootItem;
    else
        parentItem = static_cast<RoomTreeItem*>(parent.internalPointer());

    return parentItem->childCount();
}

int RoomTreeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return static_cast<RoomTreeItem*>(parent.internalPointer())->columnCount();
    else
        return rootItem->columnCount();
}

QVariant RoomTreeModel::data(const QModelIndex &index, int role = NameRole) const
{
    qDebug() << "model: data retrieve";

    if (index.isValid()) {
        RoomTreeItem *item = static_cast<RoomTreeItem*>(index.internalPointer());
        qDebug() << "data:" << item->data(index.column());

        role = NameRole;

        switch (role) {
            case NameRole:
                qDebug() << "returning name role";
                return QVariant(item->data(0));
            case TypeRole:
                return QVariant(item->data(1));
            default:
                break;
        }
    }

    return QVariant();
}

RoomTreeItem *RoomTreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        RoomTreeItem *item = static_cast<RoomTreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}

bool RoomTreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginInsertColumns(parent, position, position + columns - 1);
    success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

bool RoomTreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    RoomTreeItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, rootItem->columnCount());
    endInsertRows();

    return success;
}

bool RoomTreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    bool success;

    beginRemoveColumns(parent, position, position + columns - 1);
    success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0)
        removeRows(0, rowCount());

    return success;
}

bool RoomTreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    RoomTreeItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}

bool RoomTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    RoomTreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index);

    return result;
}
