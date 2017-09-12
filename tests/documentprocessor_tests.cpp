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

#include "documentprocessor_tests.h"

#include <QtTest>

#include "parseddocumentbuilderstub.h"

using namespace Asn1Acn::Internal;
using namespace Asn1Acn::Internal::Tests;

DocumentProcessorTests::DocumentProcessorTests(QObject *parent)
    : QObject(parent)
    , m_fileContent("Document content")
    , m_fileDir("/test/dir/")
    , m_docBuilderCreator([](const QList<DocumentSource> &documents)->ParsedDocumentBuilder *
                          { return new ParsedDocumentBuilderStub(documents); })
{
}

void DocumentProcessorTests::test_unstarted()
{
    DocumentProcessor *dp = new Asn1SccDocumentProcessor("ProjectName", m_docBuilderCreator);

    QCOMPARE(dp->state(), DocumentProcessor::State::Unfinished);

    const std::vector<std::unique_ptr<ParsedDocument>> results = dp->takeResults();
    QCOMPARE(results.size(), static_cast<size_t>(0));

    delete dp;
}

void DocumentProcessorTests::test_successful()
{
    const QString fileName("SUCCESS");
    const QString filePath = m_fileDir + fileName;

    DocumentProcessor *dp = new Asn1SccDocumentProcessor(m_projectName, m_docBuilderCreator);
    QSignalSpy spy(dp, &DocumentProcessor::processingFinished);

    dp->addToRun(filePath, m_fileContent);
    dp->run();

    examine(dp, spy, DocumentProcessor::State::Successful, fileName, filePath);

    delete dp;
}

void DocumentProcessorTests::test_error()
{
    const QString fileName("ERROR");
    const QString filePath = m_fileDir + fileName;

    DocumentProcessor *dp = new Asn1SccDocumentProcessor(m_projectName, m_docBuilderCreator);
    QSignalSpy spy(dp, &DocumentProcessor::processingFinished);

    dp->addToRun(filePath, m_fileContent);
    dp->run();

    examine(dp, spy, DocumentProcessor::State::Errored, fileName, filePath);

    delete dp;
}

void DocumentProcessorTests::test_failed()
{
    const QString fileName("FAILED");
    const QString filePath = m_fileDir + fileName;

    DocumentProcessor *dp = new Asn1SccDocumentProcessor(m_projectName, m_docBuilderCreator);
    QSignalSpy spy(dp, &DocumentProcessor::processingFinished);

    dp->addToRun(filePath, m_fileContent);
    dp->run();

    examine(dp, spy, DocumentProcessor::State::Failed, fileName, filePath);

    delete dp;
}

void DocumentProcessorTests::examine(DocumentProcessor *dp,
                                     const QSignalSpy &spy,
                                     const DocumentProcessor::State state,
                                     const QString &fileName,
                                     const QString &filePath) const
{
    QCOMPARE(dp->state(), state);
    QCOMPARE(spy.count(), 1);

    const QVariant signalArg = spy.at(0).at(0);
    QCOMPARE(signalArg.type(), QVariant::String);
    QCOMPARE(qvariant_cast<QString>(signalArg), m_projectName);

    const std::vector<std::unique_ptr<ParsedDocument>> results = dp->takeResults();
    QCOMPARE(results.size(), static_cast<size_t>(1));

    const DocumentSource resultInfo = results.at(0)->source();
    QCOMPARE(resultInfo.contents(), m_fileContent);
    QCOMPARE(resultInfo.fileName(), fileName);
    QCOMPARE(resultInfo.filePath(), filePath);
}
