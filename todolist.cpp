#include "todolist.h"
#include <QDebug>
ToDoList::ToDoList(QObject *parent) : QObject(parent)
{
    mItems.append({true, QStringLiteral("Test value 1"), QStringLiteral("DAUN")} );
    mItems.append({false, QStringLiteral("Test value 2"),QStringLiteral("DAU2N")} );
     mItems.append({false, QStringLiteral("Test value 3"),QStringLiteral("DAU3N")} );
}

QVector<ToDoItem> ToDoList::items() const
{
    return mItems;
}

bool ToDoList::setItemAt(int index, const ToDoItem &item)
{
    if(index < 0 || index >=mItems.size())
        return false;

    const ToDoItem &oldItem = mItems.at(index);
    if(item.done == oldItem.done && item.text == oldItem.text)
        return false;
    mItems[index] = item;
    return true;
}

void ToDoList::appendItem()
{
    emit preItemAppended();

    ToDoItem item;
    item.done = false;
    item.text = "Tab_#id#";
    item.textPane = "Empty pane! Try to write smth";
    mItems.append(item);
qDebug() << item.textPane;
    emit postItemAppended();
}

void ToDoList::removeCompletedItems()
{
    for(int i = 0; i < mItems.size(); i++){
        if(mItems.at(i).done) {
            emit preItemRemoved(i);

            mItems.removeAt(i);

            emit postItemRemoved();
        }
    }

}
