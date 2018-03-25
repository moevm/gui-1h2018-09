#include "datalist.h"
#include <QDebug>
DataList::DataList(QObject *parent) : QObject(parent)
{
    mItems.append({QStringLiteral("Test value 1"), QStringLiteral("DUN 1")} );
    mItems.append({QStringLiteral("Test value 2"),QStringLiteral("DAN 2")} );
     mItems.append({QStringLiteral("Test value 3"),QStringLiteral("D3N 3")} );
}

QVector<DataItem> DataList::items() const
{
    return mItems;
}

bool DataList::setItemAt(int index, const DataItem &item)
{
    if(index < 0 || index >=mItems.size())
        return false;

    const DataItem &oldItem = mItems.at(index);
    if(item.textPane == oldItem.textPane && item.textDescription == oldItem.textDescription)
        return false;
    mItems[index] = item;
    return true;
}

void DataList::appendItem()
{
    emit preItemAppended();

    DataItem item;
    item.textDescription = "Tab_#id#";
    item.textPane = "Empty pane! Try to write smth";
    mItems.append(item);
qDebug() << item.textPane;
    emit postItemAppended();
}

void DataList::removeCompletedItems()
{
//    for(int i = 0; i < mItems.size(); i++){
//        if(mItems.at(i).done) {
//            emit preItemRemoved(i);

//            mItems.removeAt(i);

//            emit postItemRemoved();
//        }
//    }

}
