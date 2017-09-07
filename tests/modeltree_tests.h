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

#include <QObject>

#include "../modeltree.h"
#include "../modeltreeproxy.h"

namespace Asn1Acn {
namespace Internal {
namespace Tests {

class ModelTreeTests
        : public QObject
        , public ModelTreeProxy
{
    Q_OBJECT

public:
    explicit ModelTreeTests(QObject *parent = 0);

private slots:
    void test_addAndRemoveProject();

    void test_getNonexistingNode();
    void test_getNodeFromNonexistingProject();
    void test_getFileListFromNonexisitngProject();
    void test_getFilesCntInNonexisitngProject();

    void test_modifiersCount();

    void test_addAndRemoveNodesWithinProject();

    void test_updateNode();

private:
    void addProjectNode(ModelTree *tree, const QString &projectName);
    void createNodeInProject(ModelTree *tree, const QString &project, const QString &path);
    bool nodeExistInProject(const ModelTree *tree, const QString &project, const QString &path);
};

} // namespace Tests
} // namespace Internal
} // namespace Asn1Acn