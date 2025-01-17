/*
 * Copyright (C) 2021 Uniontech Software Technology Co., Ltd.
 *
 * Author:     liuzhangjian<liuzhangjian@uniontech.com>
 *
 * Maintainer: liuzhangjian<liuzhangjian@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "utils/specialtools.h"

#include <stubext.h>

#include <gtest/gtest.h>

#include <QTest>

using namespace GrandSearch;
TEST(SpecialToolsTest, ut_splitCommand)
{
    QString program;
    QStringList args;
    EXPECT_FALSE(SpecialTools::splitCommand("", program, args));
    EXPECT_FALSE(SpecialTools::splitCommand(" ", program, args));
    EXPECT_TRUE(SpecialTools::splitCommand("cmd test", program, args));
}

TEST(SpecialToolsTest, ut_getMimeType)
{
    QFileInfo info(QDir::homePath());
    auto type = SpecialTools::getMimeType(info);
    EXPECT_TRUE(type.name() == "inode/directory");

    QFileInfo fileInfo("/test.wps");
    type = SpecialTools::getMimeType(fileInfo);
    EXPECT_TRUE(!type.name().isEmpty());
}

TEST(SpecialToolsTest, ut_getJsonString)
{
    QJsonObject object {
        {"key1", "test"},
        {"key2", "test"}
    };

    EXPECT_TRUE(SpecialTools::getJsonString(&object, "").isEmpty());
    EXPECT_TRUE(SpecialTools::getJsonString(&object, "key1") == "test");
}

TEST(SpecialToolsTest, ut_getJsonArray)
{
     QJsonArray array = { "test" };
    QJsonObject object {
        {"key1", array},
        {"key2", "test"}
    };

    EXPECT_TRUE(SpecialTools::getJsonArray(&object, "").isEmpty());
    EXPECT_TRUE(!SpecialTools::getJsonArray(&object, "key1").isEmpty());
}

TEST(SpecialToolsTest, ut_getRecentlyUsedFiles)
{
    stub_ext::StubExt st;
    bool (QFile::*exists_addr)() const = &QFile::exists;
    st.set_lamda(exists_addr, [](){ return false; });
    EXPECT_TRUE(SpecialTools::getRecentlyUsedFiles().isEmpty());
    
    st.reset(exists_addr);
    EXPECT_NO_FATAL_FAILURE(SpecialTools::getRecentlyUsedFiles());
}

TEST(SpecialToolsTest, ut_isHiddenFile)
{
    QString path = QDir::homePath();
     QHash<QString, QSet<QString>> filters;
    EXPECT_NO_FATAL_FAILURE(SpecialTools::isHiddenFile(path, filters));
}
