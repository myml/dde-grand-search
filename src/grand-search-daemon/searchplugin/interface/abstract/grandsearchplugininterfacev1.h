/*
 * Copyright (C) 2021 Uniontech Software Technology Co., Ltd.
 *
 * Author:     zhangyu<zhangyub@uniontech.com>
 *
 * Maintainer: zhangyu<zhangyub@uniontech.com>
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
#ifndef GRANDSEARCHPLUGININTERFACEV1_H
#define GRANDSEARCHPLUGININTERFACEV1_H

#include <QObject>

//! 扩展搜索插件需实现的dbus接口规范,该文件仅定义接口用于生成dbus接口描述的xml文件。
//! 插件开发者可使用xml文件生成adaptor实现接口
//! 插件调用者可使用xml文件生成interface调用

namespace GrandSearch {
namespace SearchPluginV1 {

class GrandSearchPluginInterface : QObject
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "com.deepin.dde.GrandSearch.SearchPlugin")
public slots:
    QString Search(const QString &json);
    QString Action(const QString &json);
    bool Stop(const QString &json);
private:
    explicit GrandSearchPluginInterface(QObject *parent = nullptr);
};

}
}

#endif // GRANDSEARCHPLUGININTERFACEV1_H
