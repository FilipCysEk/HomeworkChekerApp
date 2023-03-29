//
// Created by Filip on 07.09.2022.
//

#ifndef CHECKER_TREEMODEL_H
#define CHECKER_TREEMODEL_H

#include <QAbstractItemModel>
#include <list>
#include <vector>
#include <QIcon>
#include <qitemselectionmodel.h>

class TreeItem {
public:
    explicit TreeItem(std::vector<QVariant> elements = {}, TreeItem *parent = nullptr);

    explicit TreeItem(QIcon icon, std::vector<QVariant> elements = {}, TreeItem *parent = nullptr);

    [[nodiscard]]
    QVariant data(int column) const;

    TreeItem *pushBackChildren(const TreeItem &newChild);

    TreeItem *pushBackChildren(const std::vector<QVariant> &childElements = {});

    TreeItem *pushBackChildren(const QIcon &icon, const std::vector<QVariant> &childElements = {});

    [[nodiscard]]
    int childCount() const;

    [[nodiscard]]
    int columnCount() const;

    [[nodiscard]]
    TreeItem *getParent();

    [[nodiscard]]
    TreeItem *getChild(int index);

    [[nodiscard]]
    int row() const;

//    [[nodiscard]]
//    int getIndexOfChild(const TreeItem *child) const;

    [[nodiscard]]
    QIcon icon() const;

    void setIcon(const QIcon &icon);

    bool removeChildren(int position, int count);

private:
    QIcon iconVar;
    TreeItem *parent = nullptr;
    std::vector<QVariant> elements;
    std::list<TreeItem> childItems;

};

class TreeModel : public QAbstractItemModel {
public:
    explicit TreeModel(QObject *parent = nullptr);

    [[nodiscard]]
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

    [[nodiscard]]
    QModelIndex parent(const QModelIndex &child) const override;

    [[nodiscard]]
    TreeItem *getItem(const QModelIndex &index) const;

    [[nodiscard]]
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    [[nodiscard]]
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    [[nodiscard]]
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    [[nodiscard]]
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void addColumnTitle(const QString &name);

    void addHiddenColumnTitle(const QString &name);

    void setColumnTitle(int index, const QString &name);

    void setColumnsTitle(const std::vector<QString> &columnNames, const std::vector<QString>& hiddenColumnsNames = {});

    QVariant getColumn(const QString &columnName, const TreeItem *item);

    QVariant getColumn(const QString &columnName, const QItemSelectionModel *selectionModel);

    TreeItem *pushBackChild(const std::vector<QVariant> &elements = {});

    TreeItem *pushBackChild(const QIcon &icon, const std::vector<QVariant> &elements = {});

    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

private:
    std::vector<QString> columnTitles;
    std::vector<QString> hiddenColumnTitles;
    std::unique_ptr<TreeItem> treeItem;
};

#endif //CHECKER_TREEMODEL_H
