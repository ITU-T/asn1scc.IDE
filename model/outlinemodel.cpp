/****************************************************************************
**
** Copyright (C) 2017 N7 Mobile sp. z o. o.
** Contact: http://n7mobile.pl/Space
**
** This file is part of ASN.1/ACN Plugin for QtCreator.
**
** Plugin was developed under a programme and funded by
** European Space Agency.
**
** This Plugin is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** This Plugin is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/
#include "outlinemodel.h"

#include "childrencountingvisitor.h"
#include "displayrolevisitor.h"
#include "childreturningvisitor.h"
#include "indexfindingvisitor.h"

using namespace Asn1Acn::Internal::Model;
using namespace Asn1Acn::Internal::Data;

OutlineModel::OutlineModel(QObject *parent)
    : QAbstractItemModel(parent)
    , m_root(nullptr)
{
    // TODO - connect to some global index store?
    /*
     connect(ModelTree::instance(), &ModelTree::modelAboutToUpdate,
            [this](){ beginResetModel(); });

    connect(ModelTree::instance(), &ModelTree::modelUpdated,
            [this](){ endResetModel(); });
     * */
}

OutlineModel::~OutlineModel()
{
}

QVariant OutlineModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    const auto node = dataNode(index);

    switch (role) {
    case Qt::DecorationRole:
        // TODO icons visitor
        return QVariant();
    case Qt::DisplayRole:
    case Qt::ToolTipRole:
        return node->valueFor<DisplayRoleVisitor>();
    }

    return QVariant();
}

Qt::ItemFlags OutlineModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
}

QVariant OutlineModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_UNUSED(section);
    Q_UNUSED(orientation);
    Q_UNUSED(role);

    return QVariant();
}

QModelIndex OutlineModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    const auto parentNode = dataNode(parent);
    if (parentNode == nullptr)
        return QModelIndex();

    const auto node = parentNode->valueFor<ChildReturningVisitor>(row);
    if (node == nullptr)
        return QModelIndex();

    return createIndex(row, column, node);
}

QModelIndex OutlineModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    const auto node = dataNode(index);
    const auto parent = node->parent(); // TODO should be an extension point with visitor for Types View

    if (parent == nullptr)
        return QModelIndex();

    return createIndex(parent->valueFor<IndexFindingVisitor>(node), 0, parent);
}

int OutlineModel::rowCount(const QModelIndex &parent) const
{
    if (parent.column() > 0)
        return 0;
    auto node = dataNode(parent);
    return node ? node->valueFor<ChildrenCountingVisitor>() : 0;
}

int OutlineModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

void OutlineModel::setRoot(const Node *root)
{
    beginResetModel();
    m_root = root;
    endResetModel();
}

const Node *OutlineModel::dataNode(const QModelIndex &index)
{
    return static_cast<Node*>(index.internalPointer());
}
