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

#pragma once

#include <memory>

#include <QMutex>
#include <QString>

#include "modeltreenode.h"
#include "parseddocument.h"

namespace Asn1Acn {
namespace Internal {

class ModelTreeProxy;

class ModelTree : public QObject
{
    Q_OBJECT

    ModelTree();
    ~ModelTree() = default;

    friend class ModelTreeProxy;

public:
    static ModelTree *instance();

    const ModelTreeNode::ModelTreeNodePtr getModelTreeRoot() const;

    ModelTreeNode::ModelTreeNodePtr getAnyNodeForFilepath(const QString &filePath) const;
    ModelTreeNode::ModelTreeNodePtr getNodeForFilepathFromProject(const QString &projectName, const QString &filePath) const;

    int getProjectFilesCnt(const QString &projectName) const;
    QStringList getFileListFromProject(const QString &projectName) const;

    bool isValid() const;

signals:
    void modelUpdated();
    void modelAboutToUpdate() const;

private:
    void addProjectNode(ModelTreeNode::ModelTreeNodePtr projectNode);
    void removeProjectNode(const QString &projectName);

    void addNodeToProject(const QString &projectName, ModelTreeNode::ModelTreeNodePtr node);
    void removeNodeFromProject(const QString &projectName, const QString &fileName);

    void treeAboutToChange();
    void treeChanged();

    void updateModelTreeNode(const QString &filePath, std::shared_ptr<ParsedDocument> document);

    ModelTreeNode::ModelTreeNodePtr m_treeRoot;

    mutable QMutex m_dataMutex;
    int m_modifiersCnt;
};

} // namespace Internal
} // namespace Asn1Acn
