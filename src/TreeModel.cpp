#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
//
// Created by Filip on 07.09.2022.
//

#include "TreeModel.h"
#include <algorithm>
#include <utility>


TreeModel::TreeModel(QObject *parent) : QAbstractItemModel(parent) {
    treeItem = std::make_unique<TreeItem>();
}

QModelIndex TreeModel::parent(const QModelIndex &child) const {
    auto parentItem = getItem(child)->getParent();

    if (parentItem == treeItem.get() || parentItem == nullptr)
        return {};

    return createIndex(parentItem->row(), 0, parentItem);
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const {
    auto item = getItem(parent)->getChild(row);
    if (item)
        return createIndex(row, column, item);

    return {};
}

TreeItem *TreeModel::getItem(const QModelIndex &index) const {
    if (index.isValid()) {
        auto item = static_cast<TreeItem *>(index.internalPointer());
        if (item)
            return item;
    }
    return treeItem.get();
}

int TreeModel::rowCount(const QModelIndex &parent) const {
    auto item = getItem(parent);
    return item->childCount();
}

int TreeModel::columnCount(const QModelIndex &parent) const {
    return columnTitles.size();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const {
    auto item = getItem(index);
    if (role == Qt::DecorationRole) {
        if (index.column() == 0)
            return item->icon();
    } else if (role == Qt::DisplayRole) {
        return item->data(index.column());
    }
    return {};
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return columnTitles[section];

    return {};
}

void TreeModel::addColumnTitle(const QString &name) {
    columnTitles.push_back(name);
}

void TreeModel::setColumnTitle(int index, const QString &name) {
    columnTitles[index] = name;
}

TreeItem *TreeModel::pushBackChild(const QIcon &icon, const std::vector<QVariant> &elements) {
    beginInsertRows({}, rowCount(), rowCount());
    auto elem = treeItem->pushBackChildren(TreeItem(icon, elements, treeItem.get()));
    endInsertRows();
    return elem;
}

TreeItem *TreeModel::pushBackChild(const std::vector<QVariant> &elements) {
    beginInsertRows({}, rowCount(), rowCount());
    auto elem = treeItem->pushBackChildren(TreeItem(elements, treeItem.get()));
    endInsertRows();
    return elem;
}

QVariant TreeModel::getColumn(const QString &columnName, const QItemSelectionModel *selectionModel) {
    auto sIndexes = selectionModel->selectedIndexes();
    if (!sIndexes.empty()) {
        return getColumn(columnName, static_cast<TreeItem *>(sIndexes[0].internalPointer()));
    }
    return {};
}

QVariant TreeModel::getColumn(const QString &columnName, const TreeItem *item) {
    auto iter = std::find(columnTitles.begin(), columnTitles.end(), columnName);
    if (iter == columnTitles.end()) {
        iter = std::find(hiddenColumnTitles.begin(), hiddenColumnTitles.end(), columnName);

        if (iter != hiddenColumnTitles.end()) {
            return item->data(iter - hiddenColumnTitles.begin() + columnTitles.size());
        }
    } else {
        return item->data(iter - columnTitles.begin());
    }
    return {};
}

void TreeModel::addHiddenColumnTitle(const QString &name) {
    hiddenColumnTitles.push_back(name);
}

void
TreeModel::setColumnsTitle(const std::vector<QString> &columnNames, const std::vector<QString> &hiddenColumnsNames) {
    columnTitles = columnNames;
    hiddenColumnTitles = hiddenColumnsNames;
}

bool TreeModel::removeRows(int row, int count, const QModelIndex &parent) {
    TreeItem *parentItem = getItem(parent);
    if (!parentItem)
        return false;

    beginRemoveRows(parent, row, row + count - 1);
    const bool success = parentItem->removeChildren(row, count);
    endRemoveRows();

    return success;
}


TreeItem::TreeItem(std::vector<QVariant> elements, TreeItem *parent) : parent(parent), elements(std::move(elements)) {

}

TreeItem::TreeItem(QIcon icon, std::vector<QVariant> elements, TreeItem *parent) : parent(parent),
                                                                                   elements(std::move(elements)),
                                                                                   iconVar(std::move(icon)) {

}

QVariant TreeItem::data(int column) const {
    if (column < 0 || column >= elements.size())
        return {};

    return elements[column];
}

TreeItem *TreeItem::pushBackChildren(const TreeItem &newChild) {
    childItems.push_back(newChild);
    return &childItems.back();
}

TreeItem *TreeItem::pushBackChildren(const std::vector<QVariant> &childElements) {
    childItems.emplace_back(childElements, this);
    return &childItems.back();
}

TreeItem *TreeItem::pushBackChildren(const QIcon &icon, const std::vector<QVariant> &childElements) {
    childItems.emplace_back(icon, childElements, this);
    return &childItems.back();
}

int TreeItem::childCount() const {
    return childItems.size();
}

int TreeItem::columnCount() const {
    return elements.size();
}

TreeItem *TreeItem::getParent() {
    return parent;
}

TreeItem *TreeItem::getChild(int index) {
    if (index < 0 || index >= childItems.size())
        return nullptr;

    auto iterator = childItems.begin();
    std::advance(iterator, index);
    return &*iterator;
}

int TreeItem::row() const {
    if (parent) {
        int i = 0;
        for (const auto &item: parent->childItems) {
            if (&item == this)
                return i;
        }
        ++i;
    }
    return 0;
}

QIcon TreeItem::icon() const {
    return iconVar;
}

void TreeItem::setIcon(const QIcon &icon) {
    iconVar = icon;
}

bool TreeItem::removeChildren(int position, int count) {
    if (position < 0 || position + count > childItems.size())
        return false;

    auto it = childItems.begin();
    auto it1 = childItems.begin();
    std::advance(it, position);
    std::advance(it1, position + count);
    childItems.erase(it, it1);
    return true;
}

#pragma clang diagnostic pop