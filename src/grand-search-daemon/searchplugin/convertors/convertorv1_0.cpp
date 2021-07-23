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
#include "convertorv1_0.h"
#include "global/matcheditem.h"
#include "utils/utiltools.h"
#include "utils/searchpluginprotocol.h"

#include <QStringList>

#ifdef CURRENT_CONVERTOR_VERSION
# error "Redeclaration of CURRENT_CONVERTOR_VERSION."
#else
# define CURRENT_CONVERTOR_VERSION "1.0"
#endif

ConvertorV1_0::ConvertorV1_0()
{

}

ConvertorV1_0::~ConvertorV1_0()
{

}

AbstractConvertor *ConvertorV1_0::create()
{
    return new ConvertorV1_0();
}

QString ConvertorV1_0::version() const
{
    return CURRENT_CONVERTOR_VERSION;
}

QHash<QString, ConvertInterface> ConvertorV1_0::interfaces()
{
    QHash<QString, ConvertInterface> funcs;
    funcs.insert(PLUGININTERFACE_TYPE_SEARCH, &search);
    funcs.insert(PLUGININTERFACE_TYPE_RESULT, &result);
    funcs.insert(PLUGININTERFACE_TYPE_STOP, &stop);
    funcs.insert(PLUGININTERFACE_TYPE_ACTION, &action);

    return funcs;
}

int ConvertorV1_0::search(QJsonObject *json, void *info)
{
    Q_ASSERT(json);
    Q_ASSERT(info);

    QStringList *args = static_cast<QStringList *>(info);

    //只包含三个参数
    if (Q_UNLIKELY(args->size() != 3))
        return -1;

    QString ver = args->at(0);
    QString id = args->at(1);
    QString content = args->at(2);

    //版本不匹配
    if (Q_UNLIKELY(CURRENT_CONVERTOR_VERSION != ver))
        return -1;

    //输入值为空
    if (Q_UNLIKELY(id.isEmpty() || content.isEmpty()))
        return 1;

    QVariantMap values;
    values.insert(PLUGININTERFACE_PROTOCOL_VERSION, ver);
    values.insert(PLUGININTERFACE_PROTOCOL_MISSIONID, id);
    values.insert(PLUGININTERFACE_PROTOCOL_CONTENT, content);

    *json = QJsonObject::fromVariantMap(values);
    return 0;
}

int ConvertorV1_0::result(QJsonObject *json, void *info)
{
    Q_ASSERT(json);
    Q_ASSERT(info);

    auto ver = GrandSearch::UtilTools::getJsonString(json, PLUGININTERFACE_PROTOCOL_VERSION);
    if (CURRENT_CONVERTOR_VERSION != ver)
        return -1;

    auto missionID = GrandSearch::UtilTools::getJsonString(json, PLUGININTERFACE_PROTOCOL_MISSIONID);
    if (missionID.isEmpty())
        return 1;

    static int MaxItemCount = 100;
    int count = 0;
    QJsonArray contents = GrandSearch::UtilTools::getJsonArray(json, PLUGININTERFACE_PROTOCOL_CONTENT);
    GrandSearch::MatchedItemMap mContents;
    for (const QJsonValue &groupValue : contents) {

        //!前面组已经有100个项目了，不再继续解析
        if (count >= MaxItemCount)
            break;

        if (groupValue.isObject()) {
            auto group = groupValue.toObject();

            QString groupName = GrandSearch::UtilTools::getJsonString(&group, PLUGININTERFACE_PROTOCOL_GROUP);
            if (groupName.isEmpty())
                continue;

            QJsonArray itemsValue = GrandSearch::UtilTools::getJsonArray(&group, PLUGININTERFACE_PROTOCOL_ITEMS);
            GrandSearch::MatchedItems aGroup;

            for (const QJsonValue &itemValue : itemsValue) {
                //!当前组最大支持100个，超过不再解析
                if (count >= MaxItemCount)
                    break;

                if (itemValue.isObject()) {
                    auto item = itemValue.toObject();

                    GrandSearch::MatchedItem mItem;
                    mItem.item = GrandSearch::UtilTools::getJsonString(&item, PLUGININTERFACE_PROTOCOL_ITEM);
                    if (mItem.item.isEmpty())
                        continue;

                    mItem.name = GrandSearch::UtilTools::getJsonString(&item, PLUGININTERFACE_PROTOCOL_NAME);
                    if (mItem.name.isEmpty())
                        continue;

                    mItem.type = GrandSearch::UtilTools::getJsonString(&item, PLUGININTERFACE_PROTOCOL_TYPE);
                    if (mItem.type.isEmpty())
                        continue;

                    mItem.icon = GrandSearch::UtilTools::getJsonString(&item, PLUGININTERFACE_PROTOCOL_ICON);

                    aGroup.append(mItem);

                    count++;
                }
            }

            if (aGroup.isEmpty())
                continue;

            auto org = mContents.value(groupName);
            org.append(aGroup);
            mContents.insert(groupName, org);
        }
    }

    QVariantList *results = static_cast<QVariantList *>(info);
    results->append(QVariant::fromValue(missionID));
    results->append(QVariant::fromValue(mContents));

    return 0;
}

int ConvertorV1_0::stop(QJsonObject *json, void *info)
{
    Q_ASSERT(json);
    Q_ASSERT(info);

    QStringList *args = static_cast<QStringList *>(info);

    //只包含两个参数
    if (Q_UNLIKELY(args->size() != 2))
        return -1;

    QString ver = args->at(0);
    QString id = args->at(1);

    //版本不匹配
    if (Q_UNLIKELY(CURRENT_CONVERTOR_VERSION != ver))
        return -1;

    //输入值为空
    if (Q_UNLIKELY(id.isEmpty()))
        return 1;

    QVariantMap values;
    values.insert(PLUGININTERFACE_PROTOCOL_VERSION, ver);
    values.insert(PLUGININTERFACE_PROTOCOL_MISSIONID, id);

    *json = QJsonObject::fromVariantMap(values);
    return 0;
}

int ConvertorV1_0::action(QJsonObject *json, void *info)
{
    Q_ASSERT(json);
    Q_ASSERT(info);

    QStringList *args = static_cast<QStringList *>(info);

    //只包含三个参数
    if (Q_UNLIKELY(args->size() != 3))
        return -1;

    QString ver = args->at(0);
    QString action = args->at(1);
    QString item = args->at(2);

    //版本不匹配
    if (Q_UNLIKELY(CURRENT_CONVERTOR_VERSION != ver))
        return -1;

    //输入值为空
    if (Q_UNLIKELY(action.isEmpty() || item.isEmpty()))
        return 1;

    QVariantMap values;
    values.insert(PLUGININTERFACE_PROTOCOL_VERSION, ver);
    values.insert(PLUGININTERFACE_PROTOCOL_ACTION, action);
    values.insert(PLUGININTERFACE_PROTOCOL_ITEM, item);

    *json = QJsonObject::fromVariantMap(values);
    return 0;
}
