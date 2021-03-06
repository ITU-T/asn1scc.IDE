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
#include "metadataparser_tests.h"

#include <QtTest>

using namespace Asn1Acn::Internal::Libraries::Tests;

MetadataParserTests::MetadataParserTests(QObject *parent)
    : QObject(parent),
      m_parsedData("BadName", "BadDesc")
{
}

void MetadataParserTests::test_emptyFile()
{
    parsingFails("");
}

void MetadataParserTests::test_malformedJson()
{
    parsingFails(R"( { "name": "xxxx", )");
}

void MetadataParserTests::test_wrongJsonType()
{
    parsingFails("[]");
}

void MetadataParserTests::test_emptyObject()
{
    parsingFails("{}");
}

void MetadataParserTests::test_emptyModule()
{
    parse(R"({"name": "SomeName", "description": "SomeDesc"})");

    QCOMPARE(m_parsedData.name(), QLatin1Literal("SomeName"));
    QCOMPARE(m_parsedData.description(), QLatin1Literal("SomeDesc"));
    QCOMPARE(m_parsedData.submodules().size(), 0);
}

void MetadataParserTests::test_emptySubmodule()
{
    parse(R"({
          "name": "SomeName",
          "submodules": [
              {
                "name": "SubmoduleName",
                "description": "SubmoduleDesc"
              }
            ]
          })");

    QCOMPARE(m_parsedData.submodules().size(), 1);
    const auto submodule = m_parsedData.submodules().at(0);
    QCOMPARE(submodule.name(), QLatin1Literal("SubmoduleName"));
    QCOMPARE(submodule.description(), QLatin1Literal("SubmoduleDesc"));
    QCOMPARE(submodule.elements().size(), 0);
}

void MetadataParserTests::test_emptyElement()
{
    parse(R"({
          "name": "SomeName",
          "submodules": [
              {
                "name": "SubmoduleName",
                "elements": [
                  {
                    "name": "ElementName",
                    "description": "ElementDesc"
                  }
                ]
              }
            ]
          })");

    QCOMPARE(m_parsedData.submodules().size(), 1);
    const auto submodule = m_parsedData.submodules().at(0);
    QCOMPARE(submodule.elements().size(), 1);
    const auto element = submodule.elements().at(0);
    QCOMPARE(element.name(), QLatin1Literal("ElementName"));
    QCOMPARE(element.description(), QLatin1Literal("ElementDesc"));
    QCOMPARE(element.asn1Files().size(), 0);
    QCOMPARE(element.conflicts().size(), 0);
    QCOMPARE(element.requirements().size(), 0);
}

void MetadataParserTests::test_completeElement()
{
    parse(R"({
          "name": "SomeName",
          "submodules": [
              {
                "name": "SubmoduleName",
                "elements": [
                  {
                    "name": "ElementName",
                    "asn1Files": ["f1", "f2"],
                    "conflicts": ["c1", "c2", "c3"],
                    "requires": ["r1"],
                    "imports": [
                      { "from": "X", "types": ["Y", "Z"] }
                    ]
                  }
                ]
              }
            ]
          })");

    QCOMPARE(m_parsedData.submodules().size(), 1);
    const auto submodule = m_parsedData.submodules().at(0);
    QCOMPARE(submodule.elements().size(), 1);
    const auto element = submodule.elements().at(0);
    QCOMPARE(element.asn1Files(), (QStringList{"f1", "f2"}));
    QCOMPARE(element.conflicts(), (QStringList{"c1", "c2", "c3"}));
    QCOMPARE(element.requirements(), (QStringList{"r1"}));
    QCOMPARE(element.imports().size(), 1);
    const auto import = element.imports().at(0);
    QCOMPARE(import.from(), QLatin1Literal("X"));
    QCOMPARE(import.types().size(), 2);
    QCOMPARE(import.types().at(0), QLatin1Literal("Y"));
    QCOMPARE(import.types().at(1), QLatin1Literal("Z"));
}

void MetadataParserTests::parsingFails(const QString &jsonData)
{
    MetadataParser parser(jsonData.toUtf8());
    try {
        parser.parse();
        QFAIL("Parsing should fail");
    }
    catch (const MetadataParser::Error &err) {
        QVERIFY(QLatin1String(err.what()).size() > 0);
    }
}

void MetadataParserTests::parse(const QString &jsonData)
{
    MetadataParser parser(jsonData.toUtf8());
    m_parsedData = parser.parse();
}
