#include "roomtreeitem.h"
#include <QDebug>

RoomTreeItem::RoomTreeItem(const QVector<QVariant> &data, RoomTreeItem *parent)
{
    m_parentItem = parent;
    m_itemData = data;
}

RoomTreeItem::~RoomTreeItem()
{
    qDeleteAll(m_childItems);
}

void RoomTreeItem::appendChild(RoomTreeItem *item)
{
    m_childItems.append(item);
}

RoomTreeItem *RoomTreeItem::child(int row)
{
    return m_childItems.value(row);
}

int RoomTreeItem::childCount() const
{
    return m_childItems.count();
}

int RoomTreeItem::childNumber() const
{
    if (m_parentItem)
        return m_parentItem->m_childItems.indexOf(const_cast<RoomTreeItem*>(this));

    return 0;
}

int RoomTreeItem::columnCount() const
{
    return m_itemData.count();
}

QVariant RoomTreeItem::data(int column) const
{
    return m_itemData.value(column);
}

RoomTreeItem *RoomTreeItem::parent()
{
    return m_parentItem;
}

bool RoomTreeItem::insertChildren(int position, int count, int columns)
{
    if (position < 0 || position > m_childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVector<QVariant> data(columns);
        RoomTreeItem *item = new RoomTreeItem(data, this);
        m_childItems.insert(position, item);
    }

    return true;
}

bool RoomTreeItem::insertColumns(int position, int columns)
{
    if (position < 0 || position > m_itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        m_itemData.insert(position, QVariant());

    foreach (RoomTreeItem *child, m_childItems)
        child->insertColumns(position, columns);

    return true;
}

bool RoomTreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > m_childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete m_childItems.takeAt(position);

    return true;
}

bool RoomTreeItem::removeColumns(int position, int columns)
{
    if (position < 0 || position + columns > m_itemData.size())
        return false;

    for (int column = 0; column < columns; ++column)
        m_itemData.remove(position);

    foreach (RoomTreeItem *child, m_childItems)
        child->removeColumns(position, columns);

    return true;
}

bool RoomTreeItem::setData(int column, const QVariant &value)
{
    if (column < 0 || column >= m_itemData.size())
        return false;

    m_itemData[column] = value;
    return true;
}

