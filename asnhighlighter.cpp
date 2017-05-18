/****************************************************************************
**
** Copyright (C) 2017 N7 Mobile sp. z o. o.
** Contact: http://n7mobile.com/Space
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

#include "asnhighlighter.h"

#include <QTextDocument>

using namespace Asn1Acn::Internal;
using namespace TextEditor;

AsnHighlighter::AsnHighlighter(QTextDocument *document) :
    SyntaxHighlighter(document)
{
    static const QVector<TextStyle> categories({
        C_KEYWORD,
        C_TYPE,
        C_COMMENT,
        C_STRING,
    });
    setTextFormatCategories(categories);
}

void AsnHighlighter::highlightBlock(const QString &text)
{
    auto descriptions = m_helper.getTokensDescription(text.toStdString());
    for (auto description : descriptions)
        setFormat(description.position,
                  description.length,
                  formatForCategory(static_cast<int>(description.format)));
}
