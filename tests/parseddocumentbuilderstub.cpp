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

#include "parseddocumentbuilderstub.h"

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::Tests;

ParsedDocumentBuilderStub::ParsedDocumentBuilderStub(const QHash<QString, DocumentSource> &documents)
    : m_rawDocuments(documents)
{
}

void ParsedDocumentBuilderStub::run()
{
    QString key = *m_rawDocuments.keyBegin();

    if (key == "ERROR") {
        emit errored();
    } else if (key == "FAILED") {
        emit failed();
    } else if (key == "SUCCESS") {
        auto modules = std::make_unique<Data::Modules>();
        std::unique_ptr<ParsedDocument> parsedDocument(new ParsedDocument(std::move(modules), m_rawDocuments.value(key)));
        m_parsedDocuments.push_back(std::move(parsedDocument));

        emit finished();
    }
}

std::vector<std::unique_ptr<ParsedDocument>> ParsedDocumentBuilderStub::takeDocuments()
{
    return std::move(m_parsedDocuments);
}

const std::vector<Data::ErrorMessage> &ParsedDocumentBuilderStub::errorMessages() const
{
    return m_errorMessages;
}
